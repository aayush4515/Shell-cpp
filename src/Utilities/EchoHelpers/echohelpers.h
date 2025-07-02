#ifndef _ECHOHELPERS_
#define _ECHOHELPERS_

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;

// bool isSingleQuoted(string str);
// bool isDoubleQuoted(string str);
// bool isAdjacentQuoted (string str);
// bool hasConsecutiveSpaces(string str);
// bool isEmptyQuoted(string str, size_t& first, size_t& last);
// string handleSingleQuotes(string str);
// vector<string> tokenize(const string& line);

string stripQuotesAndCollapse(const string& raw);
string processNonQuotedBackslashes(const string& raw);

#endif