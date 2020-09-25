#include <TNode.h>
#include <queue>

#include "ParserTestCases.h"

using namespace std;


template<typename TNode, typename... Args>
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

/*extern TestStruct ParserTest2 = {
	
};*/

