#include <iostream>
#include <string>

using namespace std;

int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

  string input;

  while (true) {
    cout << "$ ";

    getline(cin, input);
    cout << input << ": command not found" << endl;
    input = "";
  }

}
