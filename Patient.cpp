#include "Patient.h"
#include "Utils.h"
#include <algorithm>

Patient::Patient(const string& name, const string& dob, const string& cardNum, const MedicalRecord& record)
    : Person(name, dob), medicalCardNumber(cardNum), patientRecord(record) {
    if (patientRecord.getRecordId() == "Нет" && cardNum != "Нет") {
        patientRecord.setRecordId(cardNum);
    }
}

string Patient::getMedicalCardNumber() const { return medicalCardNumber; }
const MedicalRecord& Patient::getPatientRecord() const { return patientRecord; }
MedicalRecord& Patient::getPatientRecordEditable() { return patientRecord; }

void Patient::setMedicalCardNumber(const string& cardNum) {
    medicalCardNumber = cardNum;
    patientRecord.setRecordId(cardNum); 
}
void Patient::setPatientRecord(const MedicalRecord& record) {
    patientRecord = record;
}

void Patient::input() {
    Person::input();
    cout << "Введите номер медицинской карты: ";
    getline(cin >> ws, medicalCardNumber);
    medicalCardNumber = trimWhitespace(medicalCardNumber);
    patientRecord.setRecordId(this->medicalCardNumber);
    cout << "--- Ввод данных медицинской записи ---" << endl;
    patientRecord.input();
}

void Patient::input(const vector<Patient>& ) {
    Person::input(); 
    patientRecord.setRecordId(this->medicalCardNumber);
    cout << "--- Ввод данных медицинской записи ---" << endl;
    patientRecord.input(); 
}

void Patient::display() const {
    Person::display();
    cout << "Номер мед. карты: " << medicalCardNumber << endl;
    cout << "--- Медицинская запись ---" << endl;
    patientRecord.display();
}

bool Patient::operator==(const Patient& other) const {
    return medicalCardNumber == other.medicalCardNumber;
}
bool Patient::operator!=(const Patient& other) const {
    return !(*this == other);
}
Patient& Patient::operator=(const Patient& other) {
    if (this == &other) return *this;
    Person::operator=(other);
    medicalCardNumber = other.medicalCardNumber;
    patientRecord = other.patientRecord;
    return *this;
}

string Patient::serialize(int level) const {
    stringstream ss;
    string current_indent = indent(level);
    string inner_indent = indent(level + 1);

    ss << current_indent << "{\n";
    ss << inner_indent << "\"fullName\": \"" << escapeJsonString(fullName) << "\",\n";
    ss << inner_indent << "\"dateOfBirth\": \"" << escapeJsonString(dateOfBirth) << "\",\n";
    ss << inner_indent << "\"medicalCardNumber\": \"" << escapeJsonString(medicalCardNumber) << "\",\n";
    ss << inner_indent << "\"patientRecord\": " << patientRecord.serialize(level + 1) << "\n";
    ss << current_indent << "}";
    return ss.str();
}

Patient Patient::deserialize(const string& json_str) {
    Patient p;
    string content = json_str;
    content.erase(remove(content.begin(), content.end(), '\n'), content.end());
    content.erase(remove(content.begin(), content.end(), '\r'), content.end());
    content.erase(remove(content.begin(), content.end(), '\t'), content.end());

    if (content.empty() || content.front() != '{' || content.back() != '}') {
        throw runtime_error("Неверный формат строки пациента (отсутствуют {}): " + json_str);
    }
    content = content.substr(1, content.length() - 2);

    p.fullName = extractJsonStringValue(content, "fullName");
    p.dateOfBirth = extractJsonStringValue(content, "dateOfBirth");
    p.medicalCardNumber = extractJsonStringValue(content, "medicalCardNumber");

    if (!isValidDateFormat(p.dateOfBirth) && !p.dateOfBirth.empty()) {
        cerr << "Предупреждение: Загружена некорректная дата рождения '" << p.dateOfBirth << "' для пациента '" << p.fullName << "'." << endl;
        p.dateOfBirth = "00.00.0000";
    }
    if (!isNumeric(p.medicalCardNumber)) {
        cerr << "Предупреждение: Загружен некорректный номер карты '" << p.medicalCardNumber << "' для пациента '" << p.fullName << "'. Установлено значение 'Нет'." << endl;
        p.medicalCardNumber = "Нет"; 
    }


    string record_content = extractJsonObjectContent(content, "patientRecord");
    if (!record_content.empty()) {
        try {
            p.patientRecord = MedicalRecord::deserialize(record_content);
            if (p.patientRecord.getRecordId() != p.medicalCardNumber) {
                cerr << "Предупреждение: Скорректировано несовпадение ID записи (" << p.patientRecord.getRecordId()
                    << ") и номера карты (" << p.medicalCardNumber
                    << ") для пациента '" << p.getFullName() << "'." << endl;
                p.patientRecord.setRecordId(p.medicalCardNumber);
            }
        }
        catch (const exception& e) {
            cerr << " Ошибка разбора patientRecord для пациента '" << p.getFullName() << "': " << e.what() << endl;
            p.patientRecord = MedicalRecord();
            p.patientRecord.setRecordId(p.medicalCardNumber); 

        }
    }
    else {
        cerr << "Предупреждение: не найден объект 'patientRecord' для пациента '" << p.getFullName() << "'" << endl;
        p.patientRecord = MedicalRecord();
        p.patientRecord.setRecordId(p.medicalCardNumber);
    }
    return p;
}

bool isMedicalCardNumberUnique(const vector<Patient>& patients, const string& cardNumber, int ignoreIndex) {
    if (!isNumeric(cardNumber) || cardNumber == "Нет") return false; 
    for (size_t i = 0; i < patients.size(); ++i) {
        if (static_cast<int>(i) == ignoreIndex) continue;
        if (patients[i].getMedicalCardNumber() == cardNumber) return false;
    }
    return true;
}