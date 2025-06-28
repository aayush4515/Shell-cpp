#include <iostream>
#include <string>

using namespace std;

int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

  // stores the input command
  string input;

  // prompt
  cout << "$ ";

  // read the prompt
  getline(cin, input);

  // output as invalid command
  cout << input << ": command not found" << endl;

  // REPL
  main();


}
