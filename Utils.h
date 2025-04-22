#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <climits>

using namespace std;

string trimWhitespace(const string& str);
bool isNumeric(const string& str);
bool isValidDateFormat(const string& dateStr);
string indent(int level);
string escapeJsonString(const string& s);
string unescapeJsonString(const string& s);
string extractJsonStringValue(const string& json_segment, const string& key);
string extractJsonObjectContent(const string& json_segment, const string& key);
string extractJsonArrayContent(const string& json_segment, const string& key);
void clearInputBuffer();

#endif // UTILS_H