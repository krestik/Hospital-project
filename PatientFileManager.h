#ifndef PATIENT_FILE_MANAGER_H
#define PATIENT_FILE_MANAGER_H

#include <vector>
#include <string>
#include "Patient.h"

using namespace std;

const string FILENAME = "patients.txt";

void savePatientsToFile(const vector<Patient>& patients);
void loadPatientsFromFile(vector<Patient>& patients);

#endif // PATIENT_FILE_MANAGER_H