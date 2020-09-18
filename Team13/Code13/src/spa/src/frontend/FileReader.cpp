#include <fstream>
#include <iostream>

#include <FileReader.h>

using namespace std;

FileReader::FileReader(string file_n) {
	filename = file_n;
}

string* FileReader::ReadFile() {

	ifstream ifs(filename);
	string* input = new string();

	input->assign((istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>()));

	return input;
}