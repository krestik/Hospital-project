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
        cout << "Введите ФИО: ";
        getline(cin >> ws, fullName);
        fullName = trimWhitespace(fullName);
        if (!fullName.empty()) break;
        cout << "Ошибка: ФИО не может быть пустым.\n";
    }
    while (true) {
        string temp_date;
        cout << "Введите Дату рождения (ДД.ММ.ГГГГ): ";
        getline(cin >> ws, temp_date);
        temp_date = trimWhitespace(temp_date);
        if (isValidDateFormat(temp_date)) {
            dateOfBirth = temp_date;
            break;
        }
        cout << "Ошибка: Некорректный формат даты. Пожалуйста, введите дату в формате ДД.ММ.ГГГГ.\n";
    }
}

void Person::display() const {
    cout << "ФИО: " << fullName << endl;
    cout << "Дата рождения: " << dateOfBirth << endl;
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