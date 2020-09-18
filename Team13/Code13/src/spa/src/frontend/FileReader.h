#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#include <vector>

#include <Token.h>

using namespace std;

class FileReader {
public:
	FileReader(string file_n);
	
	string filename;
	
	string* ReadFile();
};

#endif