#include <fstream>
#include <iostream>

#include <CodeExtractor.h>

using namespace std;

CodeExtractor::CodeExtractor(string file_n) {
	filename = file_n;
}

string CodeExtractor::extract() {

	ifstream ifs(filename);
	string input;

	input.assign((istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>()));

	return input;
}