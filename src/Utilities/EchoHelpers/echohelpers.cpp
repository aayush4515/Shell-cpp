#include "echohelpers.h"

// // checks if a string is within single quotes
// bool isSingleQuoted(string str) {
//     // size_t first = str.find('\'');
//     // if (first == string::npos) return false;

//     // size_t second = str.find('\'', first + 1);
//     // return second != string::npos;

//     if ((str.at(0) == '\'') && (str.at(str.length() - 1) == '\'')) {
//         return true;
//     }
//     return false;
// }
// // checks if a string is within double quotes
// bool isDoubleQuoted(string str) {
// //   size_t first = str.find('\"');
// //   if (first == string::npos) return false;

// //   size_t second = str.find('\"', first + 1);
// //   return second != string::npos;

//     if ((str.at(0) == '"') && (str.at(str.length() - 1) == '"')) {
//         return true;
//     }
//     return false;
// }


// // checks if a string is adjacent-quotes; works for both single and double quotes
// bool isAdjacentQuoted (string str) {
//   for (int i = 0; i < str.length() - 1; i++) {
//     if ( ((str.at(i) == '\'') && (str.at(i + 1) == '\'')) || ((str.at(i) == '\"') && (str.at(i + 1) == '\"')) ) {
//       return true;
//     }
//   }
//   return false;
// }

// bool hasConsecutiveSpaces(string str) {
//   if (count(str.begin(), str.end(), ' ') > 1) {
//     return true;
//   }
//   return false;
// }

// bool isEmptyQuoted(string str, size_t& first, size_t& last) {
//   first = str.find('\'');
//   last = str.find_last_of('\'');

//   if (last == first + 1) {
//     return true;
//   }
//   return false;
// }

// // handles various single-quoted string for 'echo' command
// string handleSingleQuotes(string str) {
//   size_t firstQuote = 0;
//   size_t lastQuote = 0;
//   string returnStr;
//   // enclosed within single-quotes; simple form: example: 'hello world'
//   // this also preserves any spaces between the qutoes
//   if ((str.at(0) == '\'') && str.at(str.length() - 1) == '\'') {
//     size_t first = str.find('\'');
//     size_t last = str.find('\'', first + 1);
//     returnStr = str.substr(first + 1, last - first - 1);
//   }
//   if (isEmptyQuoted(str, firstQuote, lastQuote)) {
//     cout << "Empty quoted" << endl;
//     // cout << "First Quote: " << firstQuote << endl;
//     // cout << "Last Quote " << lastQuote << endl;
//     returnStr = str.substr(0, firstQuote) + str.substr(lastQuote + 1, str.length() - lastQuote - 1);
//   }
//   // Adjacent quoted strings are concatenated. For example: 'hello''world' becomes helloworld
//   else if (isAdjacentQuoted(str)) {
//     size_t first = str.find('\'');
//     size_t second = str.find('\'', first + 1);
//     size_t third = str.find('\'', second + 1);
//     size_t last = str.find('\'', third + 1);
//     returnStr = str.substr(first + 1, second - first - 1) + str.substr(third + 1, last - third - 1);
//   }
//   return returnStr;
// }

// // handles various double-quoted string for 'echo' command
// // string handleDoubleQuotes(string str) {

// // }

// string processEchoArgument(const string& raw) {
//   string out;
//   bool  inSingle     = false;   // are we inside single quotes?
//   bool  lastWasSpace = false;   // to collapse consecutive blanks

//   for (char ch : raw) {
//       if (ch == '\'') {                 // toggle quote mode
//           inSingle = !inSingle;
//           continue;                     // don’t copy the quote itself
//       }

//       if (std::isspace(static_cast<unsigned char>(ch)) && !inSingle) {
//           if (lastWasSpace) continue;   // collapse spaces outside quotes
//           out.push_back(' ');
//           lastWasSpace = true;
//       } else {
//           out.push_back(ch);
//           lastWasSpace = false;
//       }
//   }

//   if (inSingle) {
//       std::cerr << "myshell: unmatched single quote\n";
//       return {};
//   }
//   return out;
// }

string stripQuotesAndCollapse(const string& raw)
{
    string out;
    char quote = '\0';          // '\0' = outside quotes; otherwise holds ' or "
    bool lastWasSpace = false;  // for collapsing blanks outside quotes

    for (char ch : raw) {
        // 1.  Opening / closing quote?
        if ((ch == '\'' || ch == '"')) {
            if (quote == '\0') {          // entering quoted section
                quote = ch;
                continue;                 // don’t copy the quote itself
            } else if (ch == quote) {     // leaving quoted section
                quote = '\0';
                continue;                 // don’t copy the quote either
            }
            // Different quote char inside current quotes → copy literally
        }

        // 2.  Whitespace outside quotes: collapse runs to a single space
        if (isspace(static_cast<unsigned char>(ch)) && quote == '\0') {
            if (lastWasSpace) continue;   // already added one space
            out.push_back(' ');
            lastWasSpace = true;
        } else {
            out.push_back(ch);
            lastWasSpace = false;
        }
    }

    if (quote != '\0')
        cerr << "myshell: unmatched " << quote << " quote\n";

    return out;
}

bool hasBackslashOutsideQuotes(const string& raw) {
    size_t posFirstSingleQuote = raw.find('\'') != string::npos ? raw.find('\'') : string::npos;
    size_t posFirstDoubleQuote = raw.find('"') != string::npos ? raw.find('"') : string::npos;
    size_t posFirstBackslash = raw.find('\\');

    // both single and double quotes exits
    if ((posFirstSingleQuote != string::npos) && (posFirstDoubleQuote != string::npos)) {
        // single quote comes before double quote
        if (posFirstSingleQuote < posFirstDoubleQuote) {
            if ((posFirstBackslash < posFirstSingleQuote)) {
                return true;
            }
        }
        // double quote comes before single quote
        else if (posFirstDoubleQuote < posFirstSingleQuote) {
            if ((posFirstBackslash < posFirstDoubleQuote)) {
                return true;
            }
        }
        return false;
    }
    return false;
}

string processNonQuotedBackslashes(const string& raw) {
    string out;

    for (char ch : raw) {
        // skip '\'
        //cout << "Current character: " << ch << endl;
        if (ch == '\\') {
            continue;
        }
        //cout << "Character after if-statement: " << ch << endl;
        // keep everything else in the string
        out.push_back(ch);
    }

    return out;
}