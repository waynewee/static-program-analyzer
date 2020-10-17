#include <TNode.h>
#include <queue>
#include <CFG.h>
#include <vector>

#include "TestCases.h"

using namespace std;


CFG_LIST buildSingleStmtTestCase() {
	CFG_LIST cfg_list;
	CFG* cfg_1 = new CFG();
	cfg_list.push_back(cfg_1);
	return cfg_list;
}

extern CFGTestCase SingleStmtTest = {
	new string("procedure SingleStmtTest {x = x + 1;}"),
	buildSingleStmtTestCase() };

CFG_LIST buildDoubleStmtTestCase() {
	CFG_LIST cfg_list;
	CFG* cfg_1 = new CFG();
	cfg_1->AddEdge(1, 2);
	cfg_list.push_back(cfg_1);
	return cfg_list;
}

extern CFGTestCase DoubleStmtTest = {
	new string("procedure DoubleStmtTest {x = x + 1; x = x + 2;}"),
	buildDoubleStmtTestCase() };

CFG_LIST buildIfStmtTestCase() {
	CFG_LIST cfg_list;
	CFG* cfg_1 = new CFG();

	cfg_1->AddEdge(1, 2);
	cfg_1->AddEdge(1, 3);

	cfg_list.push_back(cfg_1);
	return cfg_list;
}

extern CFGTestCase IfStmtTest = {
	new string("procedure IfStmt {"
	"if( x == 1) then {"
	"y = 1;"
	"}"
	"else {"
	"y = 2;}}"
	),
	buildIfStmtTestCase() };

CFG_LIST buildIfStmtWithExitTestCase() {
	CFG_LIST cfg_list;
	CFG* cfg_1 = new CFG();

	cfg_1->AddEdge(1, 2);
	cfg_1->AddEdge(1, 3);
	cfg_1->AddEdge(2, 4);
	cfg_1->AddEdge(3, 4);

	cfg_list.push_back(cfg_1);
	return cfg_list;
}

extern CFGTestCase IfStmtWithExitTest = {
	new string("procedure IfStmt {"
	"if( x == 1) then {"
	"y = 1;"
	"}"
	"else {"
	"y = 2;}"
	"y = 4;"
	"}"
	),
	buildIfStmtWithExitTestCase() };

CFG_LIST buildWhileStmtTestCase() {
	CFG_LIST cfg_list;
	CFG* cfg_1 = new CFG();

	cfg_1->AddEdge(1, 2);
	cfg_1->AddEdge(2, 1);

	cfg_list.push_back(cfg_1);
	return cfg_list;
}

extern CFGTestCase WhileStmtTest = {
	new string("procedure WhileStmt {"
	"while( x < 5 ){"
	"x = 1;"
	"}}"
	),
	buildWhileStmtTestCase() }; 

CFG_LIST buildWhileStmtWithExitTest() {
	CFG_LIST cfg_list;
	CFG* cfg_1 = new CFG();

	cfg_1->AddEdge(1, 2);
	cfg_1->AddEdge(2, 1);
	cfg_1->AddEdge(1, 3);

	cfg_list.push_back(cfg_1);
	return cfg_list;
}

extern CFGTestCase WhileStmtWithExitTest = {
	new string("procedure WhileStmt {"
	"while( x < 5 ){"
	"x = 1;"
	"}"
	"x = 2;}"
	),
	buildWhileStmtWithExitTest() };

CFG_LIST buildNestedIfWhileStmtTest() {
	CFG_LIST cfg_list;
	CFG* cfg_1 = new CFG();

	cfg_1->AddEdge(1, 2);
	cfg_1->AddEdge(1, 4);
	cfg_1->AddEdge(2, 3);
	cfg_1->AddEdge(3, 2);

	cfg_list.push_back(cfg_1);
	return cfg_list;
}

extern CFGTestCase NestedIfWhileStmtTest = {
	new string("procedure Example {"
	"if( x < 5 ) then {"
	"while( x < 5 ){"
	"x = 2;"
	"}"
	"} else {"
	"x = 2;"
	"}}"
	),
	buildNestedIfWhileStmtTest() };

CFG_LIST buildNestedWhileIfStmtTest() {
	CFG_LIST cfg_list;
	CFG* cfg_1 = new CFG();

	cfg_1->AddEdge(1, 2);
	cfg_1->AddEdge(2, 3);
	cfg_1->AddEdge(2, 4);
	cfg_1->AddEdge(3, 1);
	cfg_1->AddEdge(4, 1);

	cfg_list.push_back(cfg_1);
	return cfg_list;
}

extern CFGTestCase NestedWhileIfStmtTest = {
	new string("procedure Example {"
	"while ( x < 5 ) {"
	"if ( x < 3 ) then {"
	"x = 2;"
	"} else {"
	"x=4;"
	"}}}"
	),
	buildNestedWhileIfStmtTest() };

CFG_LIST buildMultipleNestedIfTest() {
	CFG_LIST cfg_list;
	CFG* cfg_1 = new CFG();

	cfg_1->AddEdge(1, 2);
	cfg_1->AddEdge(1, 7);
	cfg_1->AddEdge(2, 3);
	cfg_1->AddEdge(2, 6);
	cfg_1->AddEdge(3, 4);
	cfg_1->AddEdge(3, 5);

	cfg_list.push_back(cfg_1);
	return cfg_list;
}

extern CFGTestCase MultipleNestedIfTest = {
	new string("procedure Example {"
	"if( x == 1) then {"
	"if( x == 2) then {"
	"if (x == 3) then {"
	"x = 4;"
	"} else {"
	"x = 5;"
	"}} else {"
	"x = 6;"
	"}} else {"
	"x = 7;"
	"}}"
	),
	buildMultipleNestedIfTest() };

CFG_LIST buildMultipleNestedWhileTest() {
	CFG_LIST cfg_list;
	CFG* cfg_1 = new CFG();

	cfg_1->AddEdge(1, 2);
	cfg_1->AddEdge(2, 1);
	cfg_1->AddEdge(2, 3);
	cfg_1->AddEdge(3, 2);
	cfg_1->AddEdge(3, 4);
	cfg_1->AddEdge(4, 3);

	cfg_list.push_back(cfg_1);
	return cfg_list;
}

extern CFGTestCase MultipleNestedWhileTest = {
	new string("procedure Example {"
	"while (x ==1){"
	"while (x == 2){"
	"while (x ==3){"
	"x=4;"
	"}}}}"
	),
	buildMultipleNestedWhileTest() };

CFG_LIST buildMultipleNestedIfWhileTest() {
	CFG_LIST cfg_list;
	CFG* cfg_1 = new CFG();

	cfg_1->AddEdge(1, 2);
	cfg_1->AddEdge(1, 6);
	cfg_1->AddEdge(2, 3);
	cfg_1->AddEdge(2, 7);
	cfg_1->AddEdge(3, 4);
	cfg_1->AddEdge(3, 5);
	cfg_1->AddEdge(4, 2);
	cfg_1->AddEdge(5, 2);
	cfg_1->AddEdge(6, 7);

	cfg_list.push_back(cfg_1);
	return cfg_list;
}

extern CFGTestCase MultipleNestedIfWhileTest = {
	new string("procedure Example {"
	"if ( x < 5 ) then {"
	"while ( y < 4) {"
	"if ( x < 3) then {"
	"x = 2; } else {"
	"x = 3; }}}"
	"else { x = 2; }"
	"x = 5;"
	"}"
	),
	buildMultipleNestedIfWhileTest() };

CFG_LIST buildMultipleProcTest() {
	CFG_LIST cfg_list;
	CFG* cfg_1 = new CFG();
	CFG* cfg_2 = new CFG();
	CFG* cfg_3 = new CFG();

	cfg_1->AddEdge(1, 2);
	cfg_2->AddEdge(3, 4);
	cfg_3->AddEdge(5, 6);

	cfg_list.push_back(cfg_1);
	cfg_list.push_back(cfg_2);
	cfg_list.push_back(cfg_3);
	
	return cfg_list;
}

extern CFGTestCase MultipleProcTest = {
	new string("procedure One {"
	"x = 1;"
	"y = 2;"
	"}"
	"procedure One {"
	"x = 1;"
	"y = 2;"
	"}"
	"procedure One {"
	"x = 1;"
	"y = 2;"
	"}"
	),
	buildMultipleProcTest() };