#include <iostream>
#include <string>

using namespace std;

int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

  string input;

  while (true) {
    // prompt
    cout << "$ ";

    // read the prompt
    getline(cin, input);

    // output as invalid command
    cout << input << ": command not found" << endl;

    // set input to null before reading another input
    input = "";
  }

}
