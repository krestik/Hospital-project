#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>

using namespace std;

class Person {
protected:
    string fullName = "Неизвестно";
    string dateOfBirth = "00.00.0000";

public:
    Person() = default;
    Person(const string& name, const string& dob);
    virtual ~Person() = default;

    string getFullName() const;
    string getDateOfBirth() const;

    void setFullName(const string& name);
    void setDateOfBirth(const string& dob);

    virtual void input();
    virtual void display() const;
    virtual bool operator==(const Person& other) const;
    Person& operator=(const Person& other);

    friend ostream& operator<<(ostream& os, const Person& p);
};

#endif // PERSON_H