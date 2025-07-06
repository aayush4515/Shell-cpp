#ifndef _ECHOHELPERS_
#define _ECHOHELPERS_

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;

/*
    ---------- NOT USED ----------
*/

// bool isAdjacentQuoted (string str);
// bool hasConsecutiveSpaces(string str);
// bool isEmptyQuoted(string str, size_t& first, size_t& last);
// string handleSingleQuotes(string str);
// vector<string> tokenize(const string& line);
// bool isEscapedQuote(const string& s, size_t pos);
// bool isEscaped(const string& s, size_t pos);
// bool hasBackslashInsideDoubleQuotes(const string& raw);
// string processBackslashInsideDoubleQuotes(const string& raw);
// bool isSingleQuoted(string str);
// bool isDoubleQuoted(string str);


bool hasBackslashOutsideQuotes(const string& raw);                              // checks if a backslash lies outside quotes
string stripQuotesAndCollapse(const string& raw);                               // processes quotes for echo command
string processNonQuotedBackslashes(const string& raw);                          // processes backslash(es) outside of quotes

#endif