#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"
#include "MedicalRecord.h"
#include <string>
#include <vector> 
#include <stdexcept>
#include <sstream>

using namespace std;

class Patient : public Person {
private:
    string medicalCardNumber = "Нет";
    MedicalRecord patientRecord;

public:
    Patient() = default;
    Patient(const string& name, const string& dob, const string& cardNum, const MedicalRecord& record = MedicalRecord());

    string getMedicalCardNumber() const;
    const MedicalRecord& getPatientRecord() const;
    MedicalRecord& getPatientRecordEditable();

    void setMedicalCardNumber(const string& cardNum);
    void setPatientRecord(const MedicalRecord& record);

    void input(); 
    void input(const vector<Patient>& patients); 

    void display() const override;

    bool operator==(const Patient& other) const;
    bool operator!=(const Patient& other) const;
    Patient& operator=(const Patient& other);

    string serialize(int level = 0) const;
    static Patient deserialize(const string& json_str);
};

bool isMedicalCardNumberUnique(const vector<Patient>& patients, const string& cardNumber, int ignoreIndex = -1);


#endif // PATIENT_H