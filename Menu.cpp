#include "Menu.h"
#include "Utils.h"
#include <iostream>

void displayMenu() {
    cout << "\n--- ���� --- \n";
    cout << "1. �������� �������� ��������\n";
    cout << "2. �������������� �������� ��������\n";
    cout << "3. �������� �������� ��������\n";
    cout << "4. ������� ������ ���� ���������\n";
    cout << "5. �����\n";
    cout << "-------------\n";
    cout << "�������� ��������: ";
}

int getUserChoice() {
    int choice = 0;
    while (true) {
        cin >> choice;
        if (cin.fail()) {
            cout << "������: ������� �����.\n";
            cin.clear(); clearInputBuffer();
            cout << "�������� ��������: ";
        }
        else {
            clearInputBuffer();
            if (choice >= 1 && choice <= 5) {
                return choice;
            }
            else {
                cout << "������: ������� ����� �� 1 �� 5.\n";
                cout << "�������� ��������: ";
            }
        }
    }
}

void displayPatientList(const vector<Patient>& patients) {
    cout << "\n--- ������ ��������� --- \n";
    if (patients.empty()) {
        cout << "������ ����.\n";
    }
    else {
        for (size_t i = 0; i < patients.size(); ++i) {
            cout << "--- ������� " << i + 1 << " (�����: " << patients[i].getMedicalCardNumber() << ") --- \n";
            cout << "���: " << patients[i].getFullName() << endl;
            cout << "���� ��������: " << patients[i].getDateOfBirth() << endl;
            cout << "���� �����������: " << patients[i].getPatientRecord().getAdmissionDate() << endl;
            cout << "--------------------\n";
        }
        cout << "��� ��������� ������ ���������� �������� ��������������.\n";
    }
    cout << "-----------------------\n";
}

int selectPatient(const vector<Patient>& patients) {
    if (patients.empty()) {
        cout << "������ ��������� ����. ������ ��������.\n";
        return -1;
    }
    displayPatientList(patients);
    int index = -1;
    int choice = 0;
    while (true) {
        cout << "������� ����� �������� ��� ������ (1-" << patients.size() << ") ��� 0 ��� ������: ";
        cin >> choice;
        if (cin.fail()) {
            cout << "������: ������� �����.\n";
            cin.clear(); clearInputBuffer();
        }
        else {
            clearInputBuffer();
            if (choice == 0) return -1;
            if (choice >= 1 && static_cast<size_t>(choice) <= patients.size()) {
                return choice - 1;
            }
            else {
                cout << "������: �������� ����� ��������.\n";
            }
        }
    }
}

void addPatient(vector<Patient>& patients) {
    cout << "\n--- �������� ����� �������� �������� --- \n";
    Patient newPatient;
    string tempCardNumber;

    while (true) {
        cout << "������� ����� ����������� ����� (������ �����): ";
        getline(cin >> ws, tempCardNumber);
        tempCardNumber = trimWhitespace(tempCardNumber);
        if (!isNumeric(tempCardNumber)) {
            cout << "������: ����� ����� ������ �������� ������ �� ����.\n";
            continue;
        }
        if (isMedicalCardNumberUnique(patients, tempCardNumber)) {
            newPatient.setMedicalCardNumber(tempCardNumber);
            break;
        }
        else {
            cout << "������: ������� � ����� ������� ����������� ����� ��� ����������.\n";
        }
    }

    newPatient.input();

    patients.push_back(newPatient);
    cout << "������� ������� ��������!\n";
}

void editPatient(vector<Patient>& patients) {
    cout << "\n--- �������������� �������� �������� --- \n";
    int index = selectPatient(patients);
    if (index == -1) {
        cout << "�������������� ��������.\n";
        return;
    }

    Patient backupPatient = patients[index];
    Patient& patientToEdit = patients[index];

    cout << "\n--- ������� ������ �������� " << index + 1 << " --- \n";
    patientToEdit.display();
    cout << "---------------------------------\n";

    string tempInput;
    char editChoice;
    bool cardChanged = false;

    cout << "������������� �������� ���������� (���, ��, � �����)? (�/�): ";
    cin >> editChoice; clearInputBuffer();
    if (editChoice == '�' || editChoice == '�') {
        cout << "������� ���: " << patientToEdit.getFullName() << "\n����� ��� (Enter ��� ��������): ";
        getline(cin, tempInput);
        tempInput = trimWhitespace(tempInput);
        if (!tempInput.empty()) patientToEdit.setFullName(tempInput);
        else cout << "��� �� ��������.\n";


        cout << "������� ���� ��������: " << patientToEdit.getDateOfBirth() << "\n����� ���� �������� (��.��.����, Enter ��� ��������): ";
        getline(cin, tempInput);
        tempInput = trimWhitespace(tempInput);
        if (!tempInput.empty()) {
            if (isValidDateFormat(tempInput)) {
                patientToEdit.setDateOfBirth(tempInput);
            }
            else {
                cout << "������������ ������ ����. ������ �������� �� ���������.\n";
            }
        }
        else cout << "���� �������� �� ��������.\n";


        cout << "������� ����� ���. �����: " << patientToEdit.getMedicalCardNumber() << "\n����� ����� ���. ����� (������ �����, Enter ��� ��������): ";
        getline(cin, tempInput);
        tempInput = trimWhitespace(tempInput);
        if (!tempInput.empty() && tempInput != patientToEdit.getMedicalCardNumber()) {
            if (!isNumeric(tempInput)) {
                cout << "������: ����� ����� ������ �������� ������ �� ����. ������ �������� ������ ����� ���������.\n";
            }
            else if (isMedicalCardNumberUnique(patients, tempInput, index)) {
                patientToEdit.setMedicalCardNumber(tempInput);
                cardChanged = true;
                cout << "����� ����� �������.\n";
            }
            else {
                cout << "������: ������� � ������� ����� '" << tempInput << "' ��� ����������. ������ �������� ������ ����� ���������.\n";
            }
        }
        else if (tempInput.empty()) {
            cout << "����� ����� �� �������.\n";
        }
    }

    cout << "������������� ����������� ������ (����, �������, ��������)? (�/�): ";
    cin >> editChoice; clearInputBuffer();
    if (editChoice == '�' || editChoice == '�') {
        MedicalRecord& recordToEdit = patientToEdit.getPatientRecordEditable();

        if (cardChanged) {
            recordToEdit.setRecordId(patientToEdit.getMedicalCardNumber());
            cout << "  (ID ����������� ������ �������� � ������������ � ����� ������� �����)\n";
        }

        cout << "  ������� ���� �����������: " << recordToEdit.getAdmissionDate() << "\n  ����� ���� ����������� (��.��.����, Enter ��� ��������): ";
        getline(cin, tempInput);
        tempInput = trimWhitespace(tempInput);
        if (!tempInput.empty()) {
            if (isValidDateFormat(tempInput)) {
                recordToEdit.setAdmissionDate(tempInput);
            }
            else {
                cout << "  ������������ ������ ����. ������ �������� ���� ����������� ���������.\n";
            }
        }
        else cout << "  ���� ����������� �� ��������.\n";


        cout << "  ������� ���� �������: " << recordToEdit.getDischargeDate() << "\n  ����� ���� ������� (��.��.����, Enter ��� ��������, '�����' ��� ��������): ";
        getline(cin, tempInput);
        tempInput = trimWhitespace(tempInput);
        if (!tempInput.empty()) {
            if (tempInput == "�����") {
                recordToEdit.setDischargeDate("");
                cout << "  ���� ������� �������.\n";
            }
            else if (isValidDateFormat(tempInput)) {
                recordToEdit.setDischargeDate(tempInput);
            }
            else {
                cout << "  ������������ ������ ����. ������ �������� ���� ������� ���������.\n";
            }
        }
        else cout << "  ���� ������� �� ��������.\n";


        cout << "  ������� �������: " << (recordToEdit.getNotes().empty() ? "(���)" : recordToEdit.getNotes()) << "\n  ����� ������� (Enter ��� ��������): ";
        getline(cin, tempInput);
        tempInput = trimWhitespace(tempInput);
        if (!tempInput.empty()) recordToEdit.setNotes(tempInput);
        else cout << "  ������� �� ��������.\n";


        cout << "  ������������� ��������? (�/�): ";
        cin >> editChoice; clearInputBuffer();
        if (editChoice == '�' || editChoice == '�') {
            cout << "  ������� ��������:\n";
            const auto& diagnoses = recordToEdit.getDiagnoses();
            if (diagnoses.empty()) cout << "    (���)\n";
            else for (const auto& d : diagnoses) d.display();

            cout << "  �������� ����� ��������?\n";
            char addDiagChoice = '�';
            while (addDiagChoice == '�' || addDiagChoice == '�') {
                recordToEdit.inputDiagnosis();
                cout << "    �������� ��� ����? (�/�): ";
                cin >> addDiagChoice; clearInputBuffer();
            }
        }
    }
    else if (cardChanged) {
        patientToEdit.getPatientRecordEditable().setRecordId(patientToEdit.getMedicalCardNumber());
    }


    cout << "\n--- ��������������� ��������� �������������� --- \n";
    patientToEdit.display();
    cout << "----------------------------------------------\n";

    char confirmation;
    cout << "��������� ��������� ���������? (�/�): ";
    cin >> confirmation; clearInputBuffer();

    if (confirmation == '�' || confirmation == '�' || confirmation == 'y' || confirmation == 'Y') {
        cout << "������ �������� ������� ���������!\n";
    }
    else {
        patients[index] = backupPatient;
        cout << "�������������� ��������. ������������� �������� ������.\n";
    }
}

void deletePatient(vector<Patient>& patients) {
    cout << "\n--- �������� �������� �������� --- \n";
    int index = selectPatient(patients);
    if (index == -1) {
        cout << "�������� ��������.\n";
        return;
    }

    cout << "\n--- ������ ������� ��� �������� --- \n";
    patients[index].display();
    cout << "---------------------------------\n";

    char confirmation;
    cout << "�� �������, ��� ������ ������� ����� ��������? (�/�): ";
    cin >> confirmation; clearInputBuffer();

    if (confirmation == '�' || confirmation == '�' || confirmation == 'y' || confirmation == 'Y') {
        patients.erase(patients.begin() + index);
        cout << "������� ������� ������!\n";
    }
    else {
        cout << "�������� ��������.\n";
    }
}