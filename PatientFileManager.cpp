#include "PatientFileManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

void savePatientsToFile(const vector<Patient>& patients) {
    ofstream outFile(FILENAME);
    if (!outFile) {
        cerr << "Ошибка: Не удалось открыть файл '" << FILENAME << "' для записи!\n";
        return;
    }
    try {
        outFile << "[\n";
        for (size_t i = 0; i < patients.size(); ++i) {
            outFile << patients[i].serialize(1);
            if (i < patients.size() - 1) {
                outFile << ",\n";
            }
            else {
                outFile << "\n";
            }
        }
        outFile << "]\n";
        cout << "Данные пациентов сохранены в файл '" << FILENAME << "'" << endl;
    }
    catch (const exception& e) {
        cerr << "Ошибка при сохранении данных: " << e.what() << endl;
    }
}

void loadPatientsFromFile(vector<Patient>& patients) {
    ifstream inFile(FILENAME);
    if (!inFile) return;

    stringstream buffer;
    buffer << inFile.rdbuf();
    string fileContent = buffer.str();
    inFile.close();

    patients.clear();

    size_t start_array = fileContent.find('[');
    size_t end_array = fileContent.rfind(']');

    if (start_array == string::npos || end_array == string::npos || start_array >= end_array) {
        if (!fileContent.empty() && fileContent.find_first_not_of(" \t\n\r") != string::npos) {
            cerr << "Ошибка: Неверный формат файла (отсутствуют '[' или ']'). Файл: " << FILENAME << endl;
        }
        return;
    }

    string arrayContent = fileContent.substr(start_array + 1, end_array - start_array - 1);

    size_t current_pos = 0;
    int patient_count = 0;
    vector<string> loadedCardNumbers;

    while (current_pos < arrayContent.length()) {
        size_t obj_start = arrayContent.find('{', current_pos);
        if (obj_start == string::npos) break;

        int brace_level = 0;
        size_t obj_end = string::npos;
        bool in_string = false;
        for (size_t i = obj_start; i < arrayContent.length(); ++i) {
            char current_char = arrayContent[i];
            if (current_char == '"' && (i == obj_start || arrayContent[i - 1] != '\\')) {
                in_string = !in_string;
            }
            else if (!in_string) {
                if (current_char == '{') brace_level++;
                else if (current_char == '}') brace_level--;
            }

            if (brace_level == 0 && !in_string && arrayContent[i] == '}') {
                obj_end = i; break;
            }
        }

        if (obj_end == string::npos) {
            cerr << "Ошибка: Не найдена закрывающая скобка для объекта пациента, начиная с позиции " << obj_start << " в файле." << endl;
            break;
        }

        string patient_str = arrayContent.substr(obj_start, obj_end - obj_start + 1);
        patient_count++;
        try {
            Patient tempPatient = Patient::deserialize(patient_str);
            bool unique = true;
            if (tempPatient.getMedicalCardNumber() == "Нет") { 
                cerr << "Ошибка загрузки: Некорректный номер карты ('Нет') для пациента '" << tempPatient.getFullName() << "' в файле. Запись пропущена." << endl;
                unique = false;
            }
            else {
                for (const string& loadedNum : loadedCardNumbers) {
                    if (tempPatient.getMedicalCardNumber() == loadedNum) {
                        cerr << "Ошибка загрузки: Дублирующийся номер карты '" << tempPatient.getMedicalCardNumber()
                            << "' для пациента '" << tempPatient.getFullName() << "' в файле. Запись пропущена." << endl;
                        unique = false;
                        break;
                    }
                }
            }


            if (unique) {
                patients.push_back(tempPatient);
                loadedCardNumbers.push_back(tempPatient.getMedicalCardNumber());
            }

        }
        catch (const exception& e) {
            cerr << "Критическая ошибка загрузки объекта пациента " << patient_count << ": " << e.what() << ". Объект: \"" << patient_str << "\"" << endl;
            cerr << "Объект пропущен." << endl;
        }
        current_pos = obj_end + 1;
    }

    if (patient_count > 0 && !patients.empty()) {
        cout << "Данные пациентов загружены из файла '" << FILENAME << "'" << endl;
    }
    else if (patient_count > 0 && patients.empty()) {
        cerr << "Предупреждение: Файл '" << FILENAME << "' прочитан, но не удалось загрузить ни одного пациента (проверьте ошибки выше)." << endl;
    }
    else if (arrayContent.find_first_not_of(" \t\n\r") != string::npos) {
        cerr << "Предупреждение: Файл '" << FILENAME << "' не содержит корректных объектов пациентов внутри массива." << endl;
    }
}