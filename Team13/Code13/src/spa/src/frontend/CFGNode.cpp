#include <CFGNode.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

CFGNode::CFGNode(string val){
	val_ = val;
}

void CFGNode::AddSuccessor(CFGNode* successor_node) {
	successor_list_.push_back(successor_node);
}

string CFGNode::GetData() {
	return val_;
}

vector<CFGNode*> CFGNode::GetSuccessors() {
	return successor_list_;
}