#include "MedicalRecord.h"
#include "Utils.h"
#include <algorithm>

MedicalRecord::MedicalRecord(const string& id, const string& admDate, const string& disDate, const string& nts)
    : recordId(id), admissionDate(admDate), dischargeDate(disDate), notes(nts) {
}

string MedicalRecord::getRecordId() const { return recordId; }
string MedicalRecord::getAdmissionDate() const { return admissionDate; }
string MedicalRecord::getDischargeDate() const { return dischargeDate; }
string MedicalRecord::getNotes() const { return notes; }
const vector<Diagnosis>& MedicalRecord::getDiagnoses() const { return diagnoses; }

void MedicalRecord::setRecordId(const string& id) { recordId = id; }
void MedicalRecord::setAdmissionDate(const string& date) { admissionDate = date; }
void MedicalRecord::setDischargeDate(const string& date) { dischargeDate = date; }
void MedicalRecord::setNotes(const string& nts) { notes = nts; }

void MedicalRecord::addDiagnosis(const Diagnosis& diag) {
    diagnoses.push_back(diag);
}

void MedicalRecord::inputDiagnosis() {
    cout << "  Добавление нового диагноза:\n";
    Diagnosis newDiag;
    newDiag.input();
    addDiagnosis(newDiag);
}

void MedicalRecord::input() {
    string temp_date;
    while (true) {
        cout << "  Введите дату поступления (ДД.ММ.ГГГГ): ";
        getline(cin >> ws, temp_date);
        temp_date = trimWhitespace(temp_date);
        if (isValidDateFormat(temp_date)) {
            admissionDate = temp_date;
            break;
        }
        cout << "  Ошибка: Некорректный формат даты. Пожалуйста, введите дату в формате ДД.ММ.ГГГГ.\n";
    }

    while (true) {
        cout << "  Введите дату выписки (ДД.ММ.ГГГГ, оставьте пустым если пациент еще в больнице): ";
        getline(cin, dischargeDate);
        dischargeDate = trimWhitespace(dischargeDate);

        if (dischargeDate.empty() || isValidDateFormat(dischargeDate)) break;
        cout << "  Ошибка: Некорректный формат даты. Оставьте поле пустым или введите дату в формате ДД.ММ.ГГГГ.\n";
    }

    cout << "  Введите общие заметки по случаю: ";
    getline(cin >> ws, notes);
    notes = trimWhitespace(notes);

    char addDiagChoice = 'н';
    cout << "  Добавить диагнозы сейчас? (д/н): ";
    cin >> addDiagChoice; clearInputBuffer();
    while (addDiagChoice == 'д' || addDiagChoice == 'Д') {
        inputDiagnosis();
        cout << "  Добавить еще один диагноз? (д/н): ";
        cin >> addDiagChoice; clearInputBuffer();
    }
}

void MedicalRecord::display() const {
    cout << "  ID Записи: " << recordId << endl;
    cout << "  Дата поступления: " << admissionDate << endl;
    if (!dischargeDate.empty()) {
        cout << "  Дата выписки: " << dischargeDate << endl;
    }
    cout << "  Заметки: " << (notes.empty() ? "(Нет)" : notes) << endl;
    cout << "  Диагнозы (" << diagnoses.size() << "):" << endl;
    if (diagnoses.empty()) {
        cout << "    (Нет диагнозов)" << endl;
    }
    else {
        for (const auto& diag : diagnoses) {
            diag.display();
        }
    }
}

MedicalRecord& MedicalRecord::operator=(const MedicalRecord& other) {
    if (this == &other) return *this;
    recordId = other.recordId;
    admissionDate = other.admissionDate;
    dischargeDate = other.dischargeDate;
    notes = other.notes;
    diagnoses = other.diagnoses;
    return *this;
}

string MedicalRecord::serialize(int level) const {
    stringstream ss;
    string current_indent = indent(level);
    string inner_indent = indent(level + 1);
    string array_indent = indent(level + 2);

    ss << "{\n";
    ss << inner_indent << "\"recordId\": \"" << escapeJsonString(recordId) << "\",\n";
    ss << inner_indent << "\"admissionDate\": \"" << escapeJsonString(admissionDate) << "\",\n";
    ss << inner_indent << "\"dischargeDate\": \"" << escapeJsonString(dischargeDate) << "\",\n";
    ss << inner_indent << "\"notes\": \"" << escapeJsonString(notes) << "\",\n";
    ss << inner_indent << "\"diagnoses\": [\n";
    for (size_t i = 0; i < diagnoses.size(); ++i) {
        ss << array_indent << diagnoses[i].serialize(level + 2);
        if (i < diagnoses.size() - 1) {
            ss << ",\n";
        }
        else {
            ss << "\n";
        }
    }
    ss << inner_indent << "]\n";
    ss << current_indent << "}";
    return ss.str();
}

MedicalRecord MedicalRecord::deserialize(const string& json_content) {
    MedicalRecord mr;
    if (json_content.empty() || json_content == "{}") return mr;

    string cleaned_content = json_content;
    cleaned_content.erase(remove(cleaned_content.begin(), cleaned_content.end(), '\n'), cleaned_content.end());
    cleaned_content.erase(remove(cleaned_content.begin(), cleaned_content.end(), '\r'), cleaned_content.end());
    cleaned_content.erase(remove(cleaned_content.begin(), cleaned_content.end(), '\t'), cleaned_content.end());

    mr.recordId = extractJsonStringValue(cleaned_content, "recordId");
    mr.admissionDate = extractJsonStringValue(cleaned_content, "admissionDate");
    mr.dischargeDate = extractJsonStringValue(cleaned_content, "dischargeDate");
    mr.notes = extractJsonStringValue(cleaned_content, "notes");

    if (!isValidDateFormat(mr.admissionDate) && !mr.admissionDate.empty()) {
        cerr << "Предупреждение: Загружена некорректная дата поступления '" << mr.admissionDate << "' для записи ID: " << mr.recordId << endl;
        mr.admissionDate = "00.00.0000";
    }
    if (!isValidDateFormat(mr.dischargeDate) && !mr.dischargeDate.empty()) {
        cerr << "Предупреждение: Загружена некорректная дата выписки '" << mr.dischargeDate << "' для записи ID: " << mr.recordId << endl;
        mr.dischargeDate = "";
    }

    string diagnoses_content = extractJsonArrayContent(cleaned_content, "diagnoses");
    if (!diagnoses_content.empty()) {
        size_t start_obj = 0;
        while (start_obj < diagnoses_content.length()) {
            size_t obj_start_pos = diagnoses_content.find('{', start_obj);
            if (obj_start_pos == string::npos) break;

            int brace_level = 0;
            size_t current_pos = obj_start_pos;
            string diag_json;
            bool in_string = false;
            do {
                if (current_pos >= diagnoses_content.length()) break;
                char current_char = diagnoses_content[current_pos];
                if (current_char == '"' && (current_pos == 0 || diagnoses_content[current_pos - 1] != '\\')) {
                    in_string = !in_string;
                }
                else if (!in_string) {
                    if (current_char == '{') brace_level++;
                    else if (current_char == '}') brace_level--;
                }
                current_pos++;
            } while (brace_level > 0 || in_string);

            if (brace_level == 0 && !in_string) {
                diag_json = diagnoses_content.substr(obj_start_pos, current_pos - obj_start_pos);
                try {
                    mr.diagnoses.push_back(Diagnosis::deserialize(diag_json));
                }
                catch (const exception& e) {
                    cerr << " Ошибка разбора диагноза: " << e.what() << " в строке: " << diag_json << endl;
                }
                start_obj = current_pos;
            }
            else {
                cerr << " Ошибка: не найдена закрывающая скобка или кавычка в массиве диагнозов: " << diagnoses_content.substr(obj_start_pos) << endl;
                break;
            }
        }
    }
    return mr;
}