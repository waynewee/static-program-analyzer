#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
typedef short PROC;

class TNode;

class VAR_TABLE;  // no need to #include "VAR_TABLE.h" as all I need is pointer

class PKB {
public:
	static VAR_TABLE* varTable;
	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST (PROC p);

};