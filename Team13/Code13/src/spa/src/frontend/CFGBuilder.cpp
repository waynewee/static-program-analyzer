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

	int i = 0;

	for (TNode* child : stmt_list_filtered) {
		for (TNode* g_node : child->GetChildrenVector()) {
			if (IsStmtList(g_node)) {

				vector<TNode*> g_stmt_list = g_node->GetChildrenVector();

				vector<TNode*> g_stmt_list_filtered = FilterStmts(g_stmt_list);

				if (g_stmt_list_filtered.size() > 0) {

					TNode* g_child_first = g_stmt_list_filtered.at(0);
					//add edge from parent to first child
					cfg_->AddEdge(child->GetStmtIndex(), g_child_first->GetStmtIndex());

					TNode* g_child_last = g_stmt_list_filtered.at(g_stmt_list_filtered.size() - 1);
					
					if (child->GetNodeType() == TNode::NODE_TYPE::whileStmt) {
						cfg_->AddEdge(g_child_last->GetStmtIndex(), child->GetStmtIndex());
					}

					if (child->GetNodeType() == TNode::NODE_TYPE::ifStmt && i < stmt_list_filtered.size() - 1) {
						TNode* child_next = stmt_list_filtered.at(i + 1);
						cfg_->AddEdge(g_child_last->GetStmtIndex(), child_next->GetStmtIndex());
						//cfg_->RemoveEdge(child->GetStmtIndex(), child_next->GetStmtIndex());
					}
				}

				TraverseAST(g_stmt_list);
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

void CFGBuilder::PrintCFG() {

	/*std::unordered_map<STMT_IDX, STMT_IDX_SET*> data_;
	bool AddEdge(STMT_IDX s1, STMT_IDX s2);
	STMT_IDX_SET GetAllEdges(STMT_IDX s1);
	bool HasEdge(STMT_IDX s1, STMT_IDX s2);*/

	unordered_map<STMT_IDX, STMT_IDX_SET*> m = cfg_->data_;

	string edge = "----->";

	for (auto const& pair : m) {
		for (STMT_IDX idx : *pair.second) {
			cout << pair.first;
			cout << edge;
			cout << idx << endl;
		}

		cout << endl;
	}
	

}