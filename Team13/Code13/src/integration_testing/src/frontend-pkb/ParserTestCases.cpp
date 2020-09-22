#include <TNode.h>
#include <queue>

#include "ParserTestCases.h"

using namespace std;


template<typename TNode*, typename... Args>
TNode* AddChildren(TNode* parent, std::vector<TNode*> children) {
	for (TNode* child : children) {
		parent->AddChild(child);
	}
	return parent;
}


TNode* buildAssgnTestCase1() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "AssgnTest1");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);
	
	TNode* stmtNode = new TNode(TNode::assignStmt, 1);
	TNode* varNodeName = new TNode(TNode::varName, "x");
	TNode* constNode1 = new TNode(TNode::constValue, (double) 5);
	TNode* plusNode = new TNode(TNode::expr, TNode::OPERATOR::plus);
	TNode* constNode2 = new TNode(TNode::varName, "y");

	plusNode->AddChild(constNode2);
	plusNode->AddChild(constNode1);

	stmtNode->AddChild(varNodeName);
	stmtNode->AddChild(plusNode);
	
	stmtListNode->AddChild(stmtNode);

	return mainPrg;
}

extern TestCase AssgnTest1 = {
	new string("procedure AssgnTest1 {"
	"x = y + 5;}")	
	, buildAssgnTestCase1()};

TNode* buildAssgnTestCase2() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "AssgnTest2");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);

	TNode* stmtNode = new TNode(TNode::assignStmt, 1);
	TNode* varNodeName = new TNode(TNode::varName, "x");
	TNode* constNode1 = new TNode(TNode::constValue, (double) 7);

	stmtNode->AddChild(varNodeName);
	stmtNode->AddChild(constNode1);

	stmtListNode->AddChild(stmtNode);

	return mainPrg;
}

extern TestCase AssgnTest2 = {
	new string("procedure AssgnTest2 {"
	"x = 7;}")
	, buildAssgnTestCase2() };

TNode* buildAssgnTestCase3() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "AssgnTest3");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);

	TNode* stmtNode = new TNode(TNode::assignStmt, 1);
	TNode* varNode1 = new TNode(TNode::varName, "x");
	TNode* varNode2 = new TNode(TNode::varName, "y");

	stmtNode->AddChild(varNode1);
	stmtNode->AddChild(varNode2);

	stmtListNode->AddChild(stmtNode);

	return mainPrg;
}

extern TestCase AssgnTest3 = {
	new string("procedure AssgnTest3 {"
	"x = y;}")
	, buildAssgnTestCase3() };

TNode* buildAssgnTestCase4() {
	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	TNode* procNode = new TNode(TNode::NODE_TYPE::procedure);

	mainPrg->AddChild(procNode);

	TNode* procNameNode = new TNode(TNode::NODE_TYPE::procName, "AssgnTest4");

	TNode* stmtListNode = new TNode(TNode::stmtList);

	procNode->AddChild(procNameNode);
	procNode->AddChild(stmtListNode);

	TNode* stmtNode = new TNode(TNode::assignStmt, 1);

	// x = y + 5 * (a + b) - c - d * d;}
	TNode* resNode = new TNode(TNode::varName, "x");
	TNode* constNode = new TNode(TNode::constValue, (double) 5);
	TNode* aNode = new TNode(TNode::varName, "a");
	TNode* bNode = new TNode(TNode::varName, "b");
	TNode* yNode = new TNode(TNode::varName, "y");

	TNode* abPlusNode = new TNode(TNode::expr, TNode::OPERATOR::plus);
	abPlusNode->AddChild(aNode);
	abPlusNode->AddChild(bNode);

	TNode* ab5multNode = new TNode(TNode::expr, TNode::OPERATOR::times);
	ab5multNode->AddChild(constNode);
	ab5multNode->AddChild(abPlusNode);

	TNode* yPlusExprNode = new TNode(TNode::expr, TNode::OPERATOR::plus);
	yPlusExprNode->AddChild(yNode);
	yPlusExprNode->AddChild(ab5multNode);

	TNode* cNode = new TNode(TNode::varName, "c");
	TNode* cMinusNode = new TNode(TNode::expr, TNode::OPERATOR::minus);
	cMinusNode->AddChild(yPlusExprNode);
	cMinusNode->AddChild(cNode);

	TNode* dNode1 = new TNode(TNode::varName, "d");
	TNode* dNode2 = new TNode(TNode::varName, "d");
	TNode* ddMultNode = new TNode(TNode::expr, TNode::OPERATOR::times);
	ddMultNode->AddChild(dNode1);
	ddMultNode->AddChild(dNode2);

	TNode* rootMinusNode = new TNode(TNode::expr, TNode::OPERATOR::minus);
	rootMinusNode->AddChild(cMinusNode);
	rootMinusNode->AddChild(ddMultNode);

	stmtNode->AddChild(resNode);
	stmtNode->AddChild(rootMinusNode);

	stmtListNode->AddChild(stmtNode);

	return mainPrg;
}


extern TestCase AssgnTest4 = {
	new string("procedure AssgnTest4 {"
	"x = y + 5 * (a + b) - c - d * d;}")
	, buildAssgnTestCase4() };

