#include <iostream>
#include <algorithm>
#include <string>
using namespace std;
#include "UserInput.h"

string UserInput::getUserInput() {
	string user_input;
	cout << "           Enter your input : \n";
	getline(cin, user_input);
	// cout << "Your input is : " << user_input << endl;
	return user_input;
}

string UserInput::getUserInputAsLowercase() {
	return "";
}