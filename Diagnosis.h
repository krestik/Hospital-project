#ifndef DIAGNOSIS_H
#define DIAGNOSIS_H

#include <string>
#include <iostream>
#include "Utils.h"

using namespace std;

struct Diagnosis {
private:
    string date = "00.00.0000";
    string description = "Нет описания";

public:
    Diagnosis() = default;
    Diagnosis(const string& d, const string& desc);

    string getDate() const;
    string getDescription() const;

    void display() const;
    void input();
    string serialize(int level = 0) const;
    static Diagnosis deserialize(const string& json_str);
};

#endif // DIAGNOSIS_H