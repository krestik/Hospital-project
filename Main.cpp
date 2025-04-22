#define NOMINMAX
#include <windows.h>
#include <vector>
#include <iostream>
#include "Patient.h"
#include "PatientFileManager.h"
#include "Menu.h"

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector<Patient> patientDatabase;
    loadPatientsFromFile(patientDatabase);

    int choice = 0;
    do {
        displayMenu();
        choice = getUserChoice();

        switch (choice) {
        case 1: addPatient(patientDatabase); break;
        case 2: editPatient(patientDatabase); break;
        case 3: deletePatient(patientDatabase); break;
        case 4: displayPatientList(patientDatabase); break;
        case 5: cout << "Выход из программы...\n"; break;
        default: cout << "Неверный выбор.\n"; break;
        }

    } while (choice != 5);

    savePatientsToFile(patientDatabase);

    return 0;
}