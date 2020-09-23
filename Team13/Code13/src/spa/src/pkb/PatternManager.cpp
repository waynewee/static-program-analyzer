#include "PatternManager.h"


void PatternManager::AddAssignPattern(STMT_IDX s, VAR_NAME v, TNode root) {
    assign_pattern_table_.Add(s, v, root);
}

STMT_IDX_SET PatternManager::GetAssignWithFullPattern(VAR_NAME v, EXPRESSION e) {
    return GetStmtOfMatchingAssignments(v, e, true, assign_pattern_table_.GetData());
}
STMT_IDX_SET PatternManager::GetAssignWithSubPattern(VAR_NAME v, EXPRESSION e) {
    return GetStmtOfMatchingAssignments(v, e, false, assign_pattern_table_.GetData());
}
STMT_VAR_PAIR_LIST PatternManager::GetAssignStmtVarPairWithFullPattern(VAR_NAME v, EXPRESSION e) {
    return GetStmtVarPairOfMatchingAssignments(v, e, true, assign_pattern_table_.GetData());
}

STMT_VAR_PAIR_LIST PatternManager::GetAssignStmtVarPairWithSubPattern(VAR_NAME v, EXPRESSION e) {
    return GetStmtVarPairOfMatchingAssignments(v, e, false, assign_pattern_table_.GetData());
}

STMT_IDX_SET PatternManager::GetStmtOfMatchingAssignments(VAR_NAME v, EXPRESSION e, bool is_full, EXPRESSION_TABLE *data) {
    if (v.empty() && e.empty()) {
        STMT_IDX_SET result = STMT_IDX_SET();
        for (auto tuple: *data) {
            result.insert(tuple.s);
        }
        return result;
    }else if (!v.empty() && e.empty()) {
        STMT_IDX_SET result = STMT_IDX_SET();
        for (auto tuple: *data) {
            if (tuple.v == v) {
                result.insert(tuple.s);
            }
        }
        return result;
    }else if (v.empty() && !e.empty()) {
        TNode* qroot = ParseExpression(e);
        STMT_IDX_SET result = STMT_IDX_SET();
        for (auto tuple: *data) {
            if (HasMatchingPattern(tuple.eroot, *qroot, is_full)) {
                result.insert(tuple.s);
            }
        }
        return result;
    } else {
        TNode* qroot = ParseExpression(e);
        STMT_IDX_SET result = STMT_IDX_SET();
        for (auto tuple: *data) {
            if (tuple.v == v && HasMatchingPattern(tuple.eroot, *qroot, is_full)) {
                result.insert(tuple.s);
            }
        }
        return result;
    }
}


STMT_VAR_PAIR_LIST PatternManager::GetStmtVarPairOfMatchingAssignments(
    VAR_NAME v, EXPRESSION e, bool is_full, EXPRESSION_TABLE *data) {
    if (v.empty() && e.empty()) {
        STMT_VAR_PAIR_LIST result = STMT_VAR_PAIR_LIST();
        for (auto tuple: *data) {
            STMT_VAR_PAIR pair = {tuple.s, tuple.v};
            result.push_back(pair);
        }
        return result;
    } else if (e.empty()) {
        STMT_VAR_PAIR_LIST result = STMT_VAR_PAIR_LIST();
        for (auto tuple: *data) {
            if (tuple.v == v) {
                STMT_VAR_PAIR pair = {tuple.s, tuple.v};
                result.push_back(pair);
            }
        }
        return result;
    } else if (v.empty()){
        TNode* qroot = ParseExpression(e);
        STMT_VAR_PAIR_LIST result = STMT_VAR_PAIR_LIST();
        for (auto tuple: *data) {
            if (HasMatchingPattern(tuple.eroot, *qroot, is_full)) {
                STMT_VAR_PAIR pair = {tuple.s, tuple.v};
                result.push_back(pair);
            }
        }
        return result;
    } else {
        STMT_VAR_PAIR_LIST result = STMT_VAR_PAIR_LIST();
        TNode* qroot = ParseExpression(e);
        for (auto tuple: *data) {
            if (tuple.v == v && HasMatchingPattern(tuple.eroot, *qroot, is_full)) {
                STMT_VAR_PAIR pair = {tuple.s, tuple.v};
                result.push_back(pair);
            }
        }
        return result;
    }
}

EXPRESSION_TABLE* AssignPatternTable::GetData() {
    return data_;
}

void AssignPatternTable::Add(STMT_IDX s, VAR_NAME v, TNode root) {
    ASSIGN_PATTERN tuple = {s, v, root};
    data_->push_back(tuple);
//    //debug
//    std::cout << "pringing all tuples in AssignPatternTable: " << std::endl;
//    for (auto tuple: *data_) {
//        std::cout << "tuple: s = " << tuple.s << ", v = " << tuple.v << std::endl;
//    }
}
bool PatternManager::HasMatchingPattern(TNode root, TNode qroot, bool is_full) {
    if (is_full) {
        return AreTwoTreesEqual(root, qroot);
    } else {
        return HasSubMatchingPattern(root, qroot);
    }
}
bool PatternManager::HasSubMatchingPattern(TNode root, TNode qroot) {
    if (AreTwoTreesEqual(root, qroot)) {
        return true;
    }
    auto root_children = root.GetChildrenVector();
    auto root_children_size = root_children.size();
    if (root_children_size == 0) {
        return false;
    }
    for (int i = 0; i < root_children_size; i++) {
        if (AreTwoTreesEqual(*(root_children[i]), qroot)) {
            return true;
        }
    }
    for (int i = 0; i < root_children_size; i++) {
        if (HasSubMatchingPattern(*(root_children[i]), qroot)) {
            return true;
        }
    }
    return false;
}

bool PatternManager::AreTwoTreesEqual(TNode root, TNode query_root) {
    if (!AreTwoNodesEqual(root, query_root)) {
        return false;
    }
    auto root_children = root.GetChildrenVector();
    auto query_root_children = query_root.GetChildrenVector();
    auto root_children_size = root_children.size();
    auto query_root_children_size = query_root_children.size();
    if (root_children_size != query_root_children_size) {
        return false;
    }
    if (root_children_size <= 0) {
        return true;
    }
    for (int i = 0; i < root_children_size; i++) {
        if (!AreTwoTreesEqual(*(root_children[i]), *(query_root_children[i]))) {
            return false;
        }
    }
    return true;
}

bool PatternManager::AreTwoNodesEqual(TNode root, TNode query_root) {
    if (root.GetNodeType() == query_root.GetNodeType()) {
        switch (root.GetNodeType()) {
            case TNode::NODE_TYPE::expr:
                return root.GetOperator() == query_root.GetOperator();
            case TNode::NODE_TYPE::varName:
                return *(root.GetName()) == *(query_root.GetName());
            case TNode::NODE_TYPE::constValue:
                return root.GetConstValue() == query_root.GetConstValue();
            default:
                return false;
        }
    }
    return false;
}
//string PatternManager::RemoveWhiteSpace(EXPRESSION e) {
//    e.erase(std::remove(e.begin(), e.end(), ' '), e.end());
//    return e;
//}

TNode* PatternManager::ParseExpression(EXPRESSION s) {
    if (IsNumber(s)) {
        CONST_VALUE val = (CONST_VALUE) stoi(s);
        auto node = new TNode(TNode::NODE_TYPE::constValue, val);
        return node;
    }
    else {
        auto node = new TNode(TNode::NODE_TYPE::varName, s);
        return node;
    }
}


bool PatternManager::IsNumber(EXPRESSION s) {
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
void PatternManager::PrintTNode(TNode root) {
    switch (root.GetNodeType()) {
        case TNode::NODE_TYPE::expr:
            if (root.GetOperator() == TNode::OPERATOR::plus) {
                std::cout << "val = plus" << std::endl;
            }
            if (root.GetOperator() == TNode::OPERATOR::minus) {
                std::cout << "val = minus" << std::endl;
            }
            if (root.GetOperator() == TNode::OPERATOR::times) {
                std::cout << "val = times" << std::endl;
            }
            if (root.GetOperator() == TNode::OPERATOR::divide) {
                std::cout << "val = divide" << std::endl;
            }
            break;
        case TNode::NODE_TYPE::varName:
            std::cout << "val = " << *(root.GetName()) << std::endl;
            break;
        case TNode::NODE_TYPE::constValue:
            std::cout << "val = " << root.GetConstValue() << std::endl;
            break;
    }

}





