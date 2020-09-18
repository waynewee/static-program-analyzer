#include <fstream>
#include <iostream>

#include <CodeExtractor.h>

using namespace std;

CodeExtractor::CodeExtractor(string file_n) {
	filename = file_n;
}

string* CodeExtractor::Extract() {

	ifstream ifs(filename);
	string* input = new string();

	input->assign((istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>()));

	return input;
}