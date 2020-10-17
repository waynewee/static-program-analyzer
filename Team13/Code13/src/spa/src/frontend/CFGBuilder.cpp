#include <unordered_map>

#include <TNode.h>
#include <CFG.h>
#include <CFGBuilder.h>

using namespace std;

CFGBuilder::CFGBuilder(TNode* root_node) {
	for (TNode* proc_node : root_node->GetChildrenVector()) {
		cfg_list_.push_back(BuildCFG(proc_node));
	}
}

CFG_LIST CFGBuilder::GetCFGs() {
	return cfg_list_;
}

CFG* CFGBuilder::BuildCFG(TNode* proc_node) {

	CFG* cfg = new CFG();

	TNode* first_stmt_list_node = proc_node->GetChildrenVector().at(1);
	vector<TNode*> first_stmt_list = first_stmt_list_node->GetChildrenVector();

	TraverseAST(first_stmt_list, cfg);

	return cfg;
}

void CFGBuilder::TraverseAST(vector<TNode*> stmt_list, CFG* cfg) {

	vector<TNode*> stmt_list_filtered = FilterStmts(stmt_list);

	//add edge from every sibling to the next
	for (int i = 0; i < stmt_list_filtered.size() - 1; i++) {
		TNode* child = stmt_list_filtered.at(i);
		TNode* child_next = stmt_list_filtered.at(i + 1);

		cfg->AddEdge(child->GetStmtIndex(), child_next->GetStmtIndex());

	}

	int i = 0;

	for (TNode* child : stmt_list_filtered) {
		TNode* child_next = NULL;
		if (i + 1 < stmt_list_filtered.size()) {
			child_next = stmt_list_filtered.at(i + 1);
		}
		for (TNode* g_node : child->GetChildrenVector()) {
			if (IsStmtList(g_node)) {

				vector<TNode*> g_stmt_list = g_node->GetChildrenVector();

				vector<TNode*> g_stmt_list_filtered = FilterStmts(g_stmt_list);

				if (g_stmt_list_filtered.size() > 0) {

					TNode* g_child_first = g_stmt_list_filtered.at(0);
					//add edge from parent to first child
					cfg->AddEdge(child->GetStmtIndex(), g_child_first->GetStmtIndex());

					TNode* g_child_last = g_stmt_list_filtered.at(g_stmt_list_filtered.size() - 1);
					
					if (child->GetNodeType() == TNode::NODE_TYPE::whileStmt) {
						
						//initialise vector pointer to leaf_nodes
						vector<TNode*>* leaf_nodes = new vector<TNode*>{};

						GetLeafNodes(leaf_nodes, g_child_last);

						for (TNode* leaf : *leaf_nodes) {
							cfg->AddEdge(leaf->GetStmtIndex(), child->GetStmtIndex());
						}


					}

					if (child->GetNodeType() == TNode::NODE_TYPE::ifStmt && child_next != NULL) {
						cfg->AddEdge(g_child_last->GetStmtIndex(), child_next->GetStmtIndex());
						cfg->RemoveEdge(child->GetStmtIndex(), child_next->GetStmtIndex());
					}
				}

				TraverseAST(g_stmt_list, cfg);
			}
		}

		i += 1;
	}


}

bool CFGBuilder::IsStmtList(TNode* ast_node) {
	return ast_node->GetNodeType() == TNode::NODE_TYPE::stmtList;
}

bool CFGBuilder::IsStmt(TNode* ast_node) {
	TNode::NODE_TYPE node_type = ast_node->GetNodeType();

	return(
		node_type == TNode::NODE_TYPE::assignStmt
		|| node_type == TNode::NODE_TYPE::callStmt
		|| node_type == TNode::NODE_TYPE::ifStmt
		|| node_type == TNode::NODE_TYPE::printStmt
		|| node_type == TNode::NODE_TYPE::whileStmt
		|| node_type == TNode::NODE_TYPE::readStmt
		);
}

vector<TNode*> CFGBuilder::FilterStmts(vector<TNode*> stmt_list) {
	
	vector<TNode*> filtered_list;
	
	for (TNode* stmt : stmt_list) {
		if (IsStmt(stmt)) {
			filtered_list.push_back(stmt);
		}
	}

	return filtered_list;
}

vector<TNode*> CFGBuilder::FilterStmtsAndStmtLists(vector<TNode*>stmt_list) {
	vector<TNode*> filtered_list;

	for (TNode* stmt : stmt_list) {
		if (IsStmt(stmt) || IsStmtList(stmt)) {
			filtered_list.push_back(stmt);
		}
	}

	return filtered_list;
}

void CFGBuilder::GetLeafNodes(vector<TNode*>* leaf_nodes, TNode* root_node) {

	if (root_node->GetNodeType() == TNode::NODE_TYPE::whileStmt 
	|| FilterStmtsAndStmtLists(root_node->GetChildrenVector()).size() == 0) {
		leaf_nodes->push_back(root_node);
	}
	else {

		vector<TNode*> children = root_node->GetChildrenVector();

		vector<TNode*> filtered_children = FilterStmtsAndStmtLists(children);

		for (TNode* stmt : filtered_children) {

			vector<TNode*> g_children = stmt->GetChildrenVector();

			TNode* last_child = g_children.at(g_children.size() - 1);

			if (last_child->GetNodeType() != TNode::NODE_TYPE::whileStmt) {
				GetLeafNodes(leaf_nodes, last_child);
			}

		}


	}

}