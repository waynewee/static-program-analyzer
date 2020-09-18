#ifndef _CODEEXTRACTOR_H_
#define _CODEEXTRACTOR_H_

#include <vector>

#include <Token.h>

using namespace std;

class CodeExtractor {
public:
	CodeExtractor(string file_n);
	
	string filename;
	
	string* Extract();
};

#endif