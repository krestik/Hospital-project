#include "Menu.h"
#include "Utils.h"
#include <iostream>

void displayMenu() {
    cout << "\n--- Меню --- \n";
    cout << "1. Создание карточки пациента\n";
    cout << "2. Редактирование карточки пациента\n";
    cout << "3. Удаление карточки пациента\n";
    cout << "4. Вывести список всех пациентов\n";
    cout << "5. Выход\n";
    cout << "-------------\n";
    cout << "Выберите действие: ";
}

int getUserChoice() {
    int choice = 0;
    while (true) {
        cin >> choice;
        if (cin.fail()) {
            cout << "Ошибка: Введите число.\n";
            cin.clear(); clearInputBuffer();
            cout << "Выберите действие: ";
        }
        else {
            clearInputBuffer();
            if (choice >= 1 && choice <= 5) {
                return choice;
            }
            else {
                cout << "Ошибка: Введите число от 1 до 5.\n";
                cout << "Выберите действие: ";
            }
        }
    }
}

void displayPatientList(const vector<Patient>& patients) {
    cout << "\n--- Список пациентов --- \n";
    if (patients.empty()) {
        cout << "Список пуст.\n";
    }
    else {
        for (size_t i = 0; i < patients.size(); ++i) {
            cout << "--- Пациент " << i + 1 << " (Карта: " << patients[i].getMedicalCardNumber() << ") --- \n";
            cout << "ФИО: " << patients[i].getFullName() << endl;
            cout << "Дата рождения: " << patients[i].getDateOfBirth() << endl;
            cout << "Дата поступления: " << patients[i].getPatientRecord().getAdmissionDate() << endl;
            cout << "--------------------\n";
        }
        cout << "Для просмотра полной информации выберите редактирование.\n";
    }
    cout << "-----------------------\n";
}

int selectPatient(const vector<Patient>& patients) {
    if (patients.empty()) {
        cout << "Список пациентов пуст. Некого выбирать.\n";
        return -1;
    }
    displayPatientList(patients);
    int index = -1;
    int choice = 0;
    while (true) {
        cout << "Введите номер пациента для выбора (1-" << patients.size() << ") или 0 для отмены: ";
        cin >> choice;
        if (cin.fail()) {
            cout << "Ошибка: Введите число.\n";
            cin.clear(); clearInputBuffer();
        }
        else {
            clearInputBuffer();
            if (choice == 0) return -1;
            if (choice >= 1 && static_cast<size_t>(choice) <= patients.size()) {
                return choice - 1;
            }
            else {
                cout << "Ошибка: Неверный номер пациента.\n";
            }
        }
    }
}

void addPatient(vector<Patient>& patients) {
    cout << "\n--- Создание новой карточки пациента --- \n";
    Patient newPatient;
    string tempCardNumber;

    while (true) {
        cout << "Введите номер медицинской карты (только цифры): ";
        getline(cin >> ws, tempCardNumber);
        tempCardNumber = trimWhitespace(tempCardNumber);
        if (!isNumeric(tempCardNumber)) {
            cout << "Ошибка: Номер карты должен состоять только из цифр.\n";
            continue;
        }
        if (isMedicalCardNumberUnique(patients, tempCardNumber)) {
            newPatient.setMedicalCardNumber(tempCardNumber);
            break;
        }
        else {
            cout << "Ошибка: Пациент с таким номером медицинской карты уже существует.\n";
        }
    }

    newPatient.input();

    patients.push_back(newPatient);
    cout << "Пациент успешно добавлен!\n";
}

void editPatient(vector<Patient>& patients) {
    cout << "\n--- Редактирование карточки пациента --- \n";
    int index = selectPatient(patients);
    if (index == -1) {
        cout << "Редактирование отменено.\n";
        return;
    }

    Patient backupPatient = patients[index];
    Patient& patientToEdit = patients[index];

    cout << "\n--- Текущие данные пациента " << index + 1 << " --- \n";
    patientToEdit.display();
    cout << "---------------------------------\n";

    string tempInput;
    char editChoice;
    bool cardChanged = false;

    cout << "Редактировать основную информацию (ФИО, ДР, № карты)? (д/н): ";
    cin >> editChoice; clearInputBuffer();
    if (editChoice == 'д' || editChoice == 'Д') {
        cout << "Текущее ФИО: " << patientToEdit.getFullName() << "\nНовое ФИО (Enter для пропуска): ";
        getline(cin, tempInput);
        tempInput = trimWhitespace(tempInput);
        if (!tempInput.empty()) patientToEdit.setFullName(tempInput);
        else cout << "ФИО не изменено.\n";


        cout << "Текущая Дата рождения: " << patientToEdit.getDateOfBirth() << "\nНовая Дата рождения (ДД.ММ.ГГГГ, Enter для пропуска): ";
        getline(cin, tempInput);
        tempInput = trimWhitespace(tempInput);
        if (!tempInput.empty()) {
            if (isValidDateFormat(tempInput)) {
                patientToEdit.setDateOfBirth(tempInput);
            }
            else {
                cout << "Некорректный формат даты. Старое значение ДР сохранено.\n";
            }
        }
        else cout << "Дата рождения не изменена.\n";


        cout << "Текущий Номер мед. карты: " << patientToEdit.getMedicalCardNumber() << "\nНовый Номер мед. карты (только цифры, Enter для пропуска): ";
        getline(cin, tempInput);
        tempInput = trimWhitespace(tempInput);
        if (!tempInput.empty() && tempInput != patientToEdit.getMedicalCardNumber()) {
            if (!isNumeric(tempInput)) {
                cout << "Ошибка: Номер карты должен состоять только из цифр. Старое значение номера карты сохранено.\n";
            }
            else if (isMedicalCardNumberUnique(patients, tempInput, index)) {
                patientToEdit.setMedicalCardNumber(tempInput);
                cardChanged = true;
                cout << "Номер карты изменен.\n";
            }
            else {
                cout << "Ошибка: Пациент с номером карты '" << tempInput << "' уже существует. Старое значение номера карты сохранено.\n";
            }
        }
        else if (tempInput.empty()) {
            cout << "Номер карты не изменен.\n";
        }
    }

    cout << "Редактировать медицинскую запись (даты, заметки, диагнозы)? (д/н): ";
    cin >> editChoice; clearInputBuffer();
    if (editChoice == 'д' || editChoice == 'Д') {
        MedicalRecord& recordToEdit = patientToEdit.getPatientRecordEditable();

        if (cardChanged) {
            recordToEdit.setRecordId(patientToEdit.getMedicalCardNumber());
            cout << "  (ID медицинской записи обновлен в соответствии с новым номером карты)\n";
        }

        cout << "  Текущая дата поступления: " << recordToEdit.getAdmissionDate() << "\n  Новая дата поступления (ДД.ММ.ГГГГ, Enter для пропуска): ";
        getline(cin, tempInput);
        tempInput = trimWhitespace(tempInput);
        if (!tempInput.empty()) {
            if (isValidDateFormat(tempInput)) {
                recordToEdit.setAdmissionDate(tempInput);
            }
            else {
                cout << "  Некорректный формат даты. Старое значение даты поступления сохранено.\n";
            }
        }
        else cout << "  Дата поступления не изменена.\n";


        cout << "  Текущая дата выписки: " << recordToEdit.getDischargeDate() << "\n  Новая дата выписки (ДД.ММ.ГГГГ, Enter для пропуска, 'сброс' для удаления): ";
        getline(cin, tempInput);
        tempInput = trimWhitespace(tempInput);
        if (!tempInput.empty()) {
            if (tempInput == "сброс") {
                recordToEdit.setDischargeDate("");
                cout << "  Дата выписки удалена.\n";
            }
            else if (isValidDateFormat(tempInput)) {
                recordToEdit.setDischargeDate(tempInput);
            }
            else {
                cout << "  Некорректный формат даты. Старое значение даты выписки сохранено.\n";
            }
        }
        else cout << "  Дата выписки не изменена.\n";


        cout << "  Текущие заметки: " << (recordToEdit.getNotes().empty() ? "(Нет)" : recordToEdit.getNotes()) << "\n  Новые заметки (Enter для пропуска): ";
        getline(cin, tempInput);
        tempInput = trimWhitespace(tempInput);
        if (!tempInput.empty()) recordToEdit.setNotes(tempInput);
        else cout << "  Заметки не изменены.\n";


        cout << "  Редактировать диагнозы? (д/н): ";
        cin >> editChoice; clearInputBuffer();
        if (editChoice == 'д' || editChoice == 'Д') {
            cout << "  Текущие диагнозы:\n";
            const auto& diagnoses = recordToEdit.getDiagnoses();
            if (diagnoses.empty()) cout << "    (Нет)\n";
            else for (const auto& d : diagnoses) d.display();

            cout << "  Добавить новые диагнозы?\n";
            char addDiagChoice = 'д';
            while (addDiagChoice == 'д' || addDiagChoice == 'Д') {
                recordToEdit.inputDiagnosis();
                cout << "    Добавить еще один? (д/н): ";
                cin >> addDiagChoice; clearInputBuffer();
            }
        }
    }
    else if (cardChanged) {
        patientToEdit.getPatientRecordEditable().setRecordId(patientToEdit.getMedicalCardNumber());
    }


    cout << "\n--- Предварительный результат редактирования --- \n";
    patientToEdit.display();
    cout << "----------------------------------------------\n";

    char confirmation;
    cout << "Сохранить внесенные изменения? (д/н): ";
    cin >> confirmation; clearInputBuffer();

    if (confirmation == 'д' || confirmation == 'Д' || confirmation == 'y' || confirmation == 'Y') {
        cout << "Данные пациента успешно обновлены!\n";
    }
    else {
        patients[index] = backupPatient;
        cout << "Редактирование отменено. Восстановлены исходные данные.\n";
    }
}

void deletePatient(vector<Patient>& patients) {
    cout << "\n--- Удаление карточки пациента --- \n";
    int index = selectPatient(patients);
    if (index == -1) {
        cout << "Удаление отменено.\n";
        return;
    }

    cout << "\n--- Выбран пациент для удаления --- \n";
    patients[index].display();
    cout << "---------------------------------\n";

    char confirmation;
    cout << "Вы уверены, что хотите удалить этого пациента? (д/н): ";
    cin >> confirmation; clearInputBuffer();

    if (confirmation == 'д' || confirmation == 'Д' || confirmation == 'y' || confirmation == 'Y') {
        patients.erase(patients.begin() + index);
        cout << "Пациент успешно удален!\n";
    }
    else {
        cout << "Удаление отменено.\n";
    }
}