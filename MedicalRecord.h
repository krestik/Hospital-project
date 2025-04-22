#ifndef MEDICALRECORD_H
#define MEDICALRECORD_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "Diagnosis.h"
#include "Utils.h"

using namespace std;

class MedicalRecord {
private:
    string recordId = "Нет";
    string admissionDate = "00.00.0000";
    string dischargeDate = "";
    string notes = "Нет заметок";
    vector<Diagnosis> diagnoses;

public:
    MedicalRecord() = default;
    MedicalRecord(const string& id, const string& admDate, const string& disDate, const string& nts);

    string getRecordId() const;
    string getAdmissionDate() const;
    string getDischargeDate() const;
    string getNotes() const;
    const vector<Diagnosis>& getDiagnoses() const;

    void setRecordId(const string& id);
    void setAdmissionDate(const string& date);
    void setDischargeDate(const string& date);
    void setNotes(const string& nts);

    void addDiagnosis(const Diagnosis& diag);
    void inputDiagnosis();
    void input();
    void display() const;
    MedicalRecord& operator=(const MedicalRecord& other);

    string serialize(int level = 0) const;
    static MedicalRecord deserialize(const string& json_content);
};

#endif // MEDICALRECORD_H