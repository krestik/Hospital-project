#include "Utils.h"
#include <algorithm>
#include <cctype>

string trimWhitespace(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

bool isNumeric(const string& str) {
    if (str.empty()) return false;
    return all_of(str.begin(), str.end(), [](unsigned char c) { return ::isdigit(c); });
}

bool isValidDateFormat(const string& dateStr) {
    if (dateStr.length() != 10) return false;
    if (dateStr[2] != '.' || dateStr[5] != '.') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(static_cast<unsigned char>(dateStr[i]))) return false;
    }
    try {
        int day = stoi(dateStr.substr(0, 2));
        int month = stoi(dateStr.substr(3, 2));
        if (day < 1 || day > 31 || month < 1 || month > 12) {
            return false;
        }
    }
    catch (...) {
        return false;
    }
    return true;
}

string indent(int level) {
    return string(level * 4, ' ');
}

string escapeJsonString(const string& s) {
    string escaped;
    escaped.reserve(s.length());
    for (char c : s) {
        switch (c) {
        case '"':  escaped += "\\\""; break;
        case '\\': escaped += "\\\\"; break;
        default:   escaped += c; break;
        }
    }
    return escaped;
}

string unescapeJsonString(const string& s) {
    string unescaped;
    unescaped.reserve(s.length());
    for (size_t i = 0; i < s.length(); ++i) {
        if (s[i] == '\\' && i + 1 < s.length()) {
            switch (s[i + 1]) {
            case '"':  unescaped += '"'; i++; break;
            case '\\': unescaped += '\\'; i++; break;
            default:   unescaped += s[i]; break;
            }
        }
        else {
            unescaped += s[i];
        }
    }
    return unescaped;
}

string extractJsonStringValue(const string& json_segment, const string& key) {
    string key_marker = "\"" + key + "\":";
    size_t key_pos = json_segment.find(key_marker);
    if (key_pos == string::npos) return "";

    size_t value_start = json_segment.find('"', key_pos + key_marker.length());
    if (value_start == string::npos) return "";
    value_start++;

    size_t end_pos = string::npos;
    for (size_t i = value_start; i < json_segment.length(); ++i) {
        if (json_segment[i] == '"') {
            if (i == value_start || json_segment[i - 1] != '\\') {
                end_pos = i;
                break;
            }
        }
    }

    if (end_pos == string::npos) {
        cerr << "Предупреждение: не найдена закрывающая кавычка для ключа '" << key << "'" << endl;
        return "";
    }
    return unescapeJsonString(json_segment.substr(value_start, end_pos - value_start));
}

string extractJsonObjectContent(const string& json_segment, const string& key) {
    string key_marker = "\"" + key + "\":";
    size_t key_pos = json_segment.find(key_marker);
    if (key_pos == string::npos) return "";

    size_t start_brace = json_segment.find('{', key_pos + key_marker.length());
    if (start_brace == string::npos) return "";
    start_brace++;

    int brace_level = 1;
    size_t current_pos = start_brace;
    bool in_string = false;
    while (current_pos < json_segment.length() && brace_level > 0) {
        char current_char = json_segment[current_pos];
        if (current_char == '"' && (current_pos == 0 || json_segment[current_pos - 1] != '\\')) {
            in_string = !in_string;
        }
        else if (!in_string) {
            if (current_char == '{') brace_level++;
            else if (current_char == '}') brace_level--;
        }
        if (brace_level > 0) {
            current_pos++;
        }
    }

    if (brace_level != 0) {
        cerr << "Предупреждение: не найдена закрывающая скобка для объекта '" << key << "'" << endl;
        return "";
    }
    return json_segment.substr(start_brace, current_pos - start_brace);
}

string extractJsonArrayContent(const string& json_segment, const string& key) {
    string key_marker = "\"" + key + "\":";
    size_t key_pos = json_segment.find(key_marker);
    if (key_pos == string::npos) return "";

    size_t start_bracket = json_segment.find('[', key_pos + key_marker.length());
    if (start_bracket == string::npos) return "";
    start_bracket++;

    int bracket_level = 1;
    size_t current_pos = start_bracket;
    bool in_string = false;

    while (current_pos < json_segment.length() && bracket_level > 0) {
        char current_char = json_segment[current_pos];
        if (current_char == '"' && (current_pos == 0 || json_segment[current_pos - 1] != '\\')) {
            in_string = !in_string;
        }
        else if (!in_string) {
            if (current_char == '[') bracket_level++;
            else if (current_char == ']') bracket_level--;
        }
        if (bracket_level > 0) {
            current_pos++;
        }
    }
    if (bracket_level != 0) {
        cerr << "Предупреждение: не найдена закрывающая скобка для массива '" << key << "'" << endl;
        return "";
    }
    return json_segment.substr(start_bracket, current_pos - start_bracket);
}

void clearInputBuffer() {
    cin.ignore(INT_MAX, '\n');
}