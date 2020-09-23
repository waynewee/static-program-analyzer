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
                std::cout << "they match!" << std::endl;
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
    return is_full?HasFullMatchingPattern(root, qroot): HasSubMatchingPattern(root, qroot);
}
bool PatternManager::HasSubMatchingPattern(TNode root, TNode qroot) {
    vector<TNode*> children = root.GetChildrenVector();
    if (!children.empty()) {
        for (TNode* child: children) {
            if (HasSubMatchingPattern(*child, qroot)) {
                return true;
            };
        }
    } else {
        if (IsMatchingEntRefTNode(root, qroot)) {
            return true;
        }
    }
    return false;
}
bool PatternManager::HasFullMatchingPattern(TNode root, TNode qroot) {
    vector<TNode*> children = root.GetChildrenVector();
    for (auto child: children) {
        if (IsMatchingEntRefTNode(root, qroot)) {
            return true;
        }
    }
    return false;
}
bool PatternManager::IsMatchingEntRefTNode(TNode root, TNode qroot) {
    if (qroot.GetNodeType() == root.GetNodeType()) {
        if (root.GetNodeType() == TNode::NODE_TYPE::varName) {
            if (*(qroot.GetName()) == *(root.GetName())) {
                return true;
            }
        } else if (root.GetNodeType() == TNode::NODE_TYPE::constValue) {
            if ((int) qroot.GetConstValue() == (int) root.GetConstValue()) {
                return true;
            }
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
        TNode* node = new TNode(TNode::NODE_TYPE::constValue);
        node->SetValue(val);
        return node;
    }
    else {
        return new TNode(TNode::NODE_TYPE::varName, s);
    }
}


bool PatternManager::IsNumber(EXPRESSION s) {
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}





