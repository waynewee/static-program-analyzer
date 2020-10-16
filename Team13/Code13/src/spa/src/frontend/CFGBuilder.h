#ifndef _CFGBUILDER_H_
#define _CFGBUILDER_H_

#include <vector>

#include "FrontendTypes.h"
#include <TNode.h>
#include "Token.h"
#include <CFG.h>

using namespace std;

class CFGBuilder {
public:

	TNode* ast_root_;
	CFG_LIST cfg_list_;

	CFGBuilder(TNode* root_node);
	CFG* BuildCFG(TNode* proc_node);
	CFG_LIST GetCFGs();
	void TraverseAST(vector<TNode*> stmt_list, CFG* cfg);
	bool IsStmt(TNode* ast_node);
	bool IsStmtList(TNode* ast_node);
	vector<TNode*> FilterStmts(vector<TNode*> stmt_list);
	vector<TNode*> FilterStmtsAndStmtLists(vector<TNode*> stmt_list);
	void GetLeafNodes(vector<TNode*>* leaf_nodes, TNode* root_node);

private:
	
};

#endif