#include <iostream>
#include <string>

using namespace std;

bool isSingleQuoted(string str);
bool isDoubleQuoted(string str);
bool isAdjacentQuoted (string str);
bool hasConsecutiveSpaces(string str);
bool isEmptyQuoted(string str, size_t& first, size_t& last);
string handleSingleQuotes(string str);