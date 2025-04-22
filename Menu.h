#ifndef MENU_H
#define MENU_H

#include <vector>
#include "Patient.h"

using namespace std;

void displayMenu();
int getUserChoice();
void displayPatientList(const vector<Patient>& patients);
int selectPatient(const vector<Patient>& patients);
void addPatient(vector<Patient>& patients);
void editPatient(vector<Patient>& patients);
void deletePatient(vector<Patient>& patients);

#endif // MENU_H