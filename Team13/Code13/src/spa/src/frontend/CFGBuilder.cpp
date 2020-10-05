#include <unordered_map>

#include <TNode.h>
#include <CFG.h>
#include <CFGBuilder.h>

using namespace std;

CFGBuilder::CFGBuilder(TNode* proc_node) {
	ast_root_ = proc_node;
	cfg_ = new CFG();
}

void CFGBuilder::BuildCFG() {

	TNode* first_stmt_list_node = ast_root_->GetChildrenVector().at(1);
	vector<TNode*> first_stmt_list = first_stmt_list_node->GetChildrenVector();

	for (TNode* child : first_stmt_list) {
		cout << child->getData() << endl;
	}


	TraverseAST(first_stmt_list);

	PrintCFG();
}

void CFGBuilder::TraverseAST(vector<TNode*> stmt_list) {

	vector<TNode*> stmt_list_filtered = FilterStmts(stmt_list);

	//add edge from every sibling to the next
	for (int i = 0; i < stmt_list_filtered.size() - 1; i++) {
		TNode* child = stmt_list_filtered.at(i);
		TNode* child_next = stmt_list_filtered.at(i + 1);

		cfg_->AddEdge(child->GetStmtIndex(), child_next->GetStmtIndex());

	}

	for (TNode* child : stmt_list_filtered) {
		for (TNode* g_node : child->GetChildrenVector()) {
			if (IsStmtList(g_node)) {

				vector<TNode*> g_stmt_list = g_node->GetChildrenVector();

				vector<TNode*> g_stmt_list_filtered = FilterStmts(g_stmt_list);

				if (g_stmt_list_filtered.size() > 0) {

					TNode* g_child_first = g_stmt_list_filtered.at(0);
					//add edge from parent to first child
					cfg_->AddEdge(child->GetStmtIndex(), g_child_first->GetStmtIndex());
				}

				TraverseAST(g_stmt_list);
			}
		}
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

void CFGBuilder::PrintCFG() {

	if (cfg_->HasEdge(6, 8)) {
		cout << "TRUE";
	}
	else {
		cout << "FALSE" << endl;
	}

}