#include "Diagnosis.h"
#include "Utils.h"
#include <sstream>
#include <algorithm>

Diagnosis::Diagnosis(const string& d, const string& desc) : date(d), description(desc) {}

string Diagnosis::getDate() const { return date; }
string Diagnosis::getDescription() const { return description; }

void Diagnosis::display() const {
    cout << "  - Дата диагноза: " << date << ", Описание: " << description << endl;
}

void Diagnosis::input() {
    string temp_date;
    while (true) {
        cout << "    Введите дату диагноза (ДД.ММ.ГГГГ): ";
        getline(cin >> ws, temp_date);
        temp_date = trimWhitespace(temp_date);
        if (isValidDateFormat(temp_date)) {
            date = temp_date;
            break;
        }
        cout << "    Ошибка: Некорректный формат даты. Пожалуйста, введите дату в формате ДД.ММ.ГГГГ.\n";
    }
    cout << "    Введите описание диагноза: ";
    getline(cin >> ws, description);
    description = trimWhitespace(description);
}

string Diagnosis::serialize(int level) const {
    stringstream ss;
    string current_indent = indent(level);
    string inner_indent = indent(level + 1);
    ss << "{\n";
    ss << inner_indent << "\"date\": \"" << escapeJsonString(date) << "\",\n";
    ss << inner_indent << "\"description\": \"" << escapeJsonString(description) << "\"\n";
    ss << current_indent << "}";
    return ss.str();
}

Diagnosis Diagnosis::deserialize(const string& json_str) {
    Diagnosis d;
    string content = json_str;
    content.erase(remove(content.begin(), content.end(), '\n'), content.end());
    content.erase(remove(content.begin(), content.end(), '\r'), content.end());
    content.erase(remove(content.begin(), content.end(), '\t'), content.end());

    if (!content.empty() && content.front() == '{' && content.back() == '}') {
        content = content.substr(1, content.length() - 2);
    }
    else if (!content.empty()) {
        cerr << "Предупреждение: Ожидался объект {...} в Diagnosis::deserialize, получено: " << content << endl;
    }
    d.date = extractJsonStringValue(content, "date");
    d.description = extractJsonStringValue(content, "description");
    if (!isValidDateFormat(d.date) && !d.date.empty()) {
        cerr << "Предупреждение: Загружена некорректная дата диагноза '" << d.date << "'" << endl;
        d.date = "00.00.0000";
    }
    return d;
}