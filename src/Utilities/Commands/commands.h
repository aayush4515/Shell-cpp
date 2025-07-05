#ifndef _COMMANDS_
#define _COMMANDS_

#include <iostream>
#include <vector>
#include <string>
#include "../Helpers/helpers.h"
#include "../EchoHelpers/echohelpers.h"

using namespace std;

void pwd();
void echo(string& input);
void type(string& input);
void cd(string& input);
void history();

#endif