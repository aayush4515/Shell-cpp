#ifndef _COMMANDS_
#define _COMMANDS_

#include <iostream>
#include <vector>
#include <string>
#include "../Helpers/helpers.h"
#include "../EchoHelpers/echohelpers.h"

using namespace std;

void pwd();                     // prints the current/working directory to stdout
void echo(string& input);       // echoes anything to stdout
void type(string& input);       // returns the command type
void cd(string& input);         // changes directory
void history();                 // displays command history
void history(int n);            // displays command history -> last 'n' lines

#endif