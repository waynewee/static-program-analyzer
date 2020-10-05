#ifndef _CFGNODE_H_
#define _CFGNODE_H_

#include <string>
#include <vector>

using namespace std;

class CFGNode {
public:
	CFGNode::CFGNode(string val);

	string val_;
	vector<CFGNode*> successor_list_;

	vector<CFGNode*> GetSuccessors();
	void AddSuccessor(CFGNode* successor_node);
	string GetData();

private:
};


#endif