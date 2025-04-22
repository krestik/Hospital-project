#include "Person.h"
#include "Utils.h"

Person::Person(const string& name, const string& dob)
    : fullName(name), dateOfBirth(dob) {
}
string Person::getFullName() const { return fullName; }
string Person::getDateOfBirth() const { return dateOfBirth; }

void Person::setFullName(const string& name) { fullName = name; }
void Person::setDateOfBirth(const string& dob) { dateOfBirth = dob; }

void Person::input() {
    while (true) {
        cout << "������� ���: ";
        getline(cin >> ws, fullName);
        fullName = trimWhitespace(fullName);
        if (!fullName.empty()) break;
        cout << "������: ��� �� ����� ���� ������.\n";
    }
    while (true) {
        string temp_date;
        cout << "������� ���� �������� (��.��.����): ";
        getline(cin >> ws, temp_date);
        temp_date = trimWhitespace(temp_date);
        if (isValidDateFormat(temp_date)) {
            dateOfBirth = temp_date;
            break;
        }
        cout << "������: ������������ ������ ����. ����������, ������� ���� � ������� ��.��.����.\n";
    }
}

void Person::display() const {
    cout << "���: " << fullName << endl;
    cout << "���� ��������: " << dateOfBirth << endl;
}

bool Person::operator==(const Person& other) const {
    return fullName == other.fullName;
}

Person& Person::operator=(const Person& other) {
    if (this == &other) return *this;
    fullName = other.fullName;
    dateOfBirth = other.dateOfBirth;
    return *this;
}

ostream& operator<<(ostream& os, const Person& p) {
    p.display();
    return os;
}