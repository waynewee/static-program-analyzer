#include "PatternManager.h"

void PatternManager::AddAssignPattern(STMT_IDX s, VAR_NAME v, TNode root) {
    assign_pattern_table_.Add(s, v, root);
}

STMT_IDX_SET PatternManager::GetAssignWithFullPattern(VAR_NAME v, EXPRESSION e) {
    return GetAssignWithPattern(v, e, assign_pattern_table_);
}
STMT_IDX_SET PatternManager::GetAssignWithPattern(VAR_NAME v, EXPRESSION e, AssignPatternTable table) {
    //debug
    std::cout << "v = " << v << std::endl;
    if (v.empty() && e.empty()) {
        return statement_table_.GetAll(STATEMENT_TYPE::assignStatement);
    }
    if (!v.empty() && e.empty()) {
        STMT_IDX_SET result = STMT_IDX_SET();
        auto data = assign_pattern_table_.GetData();
        for (auto tuple: data) {
            if (tuple.v == v) {
                result.insert(tuple.s);
            }
        }
        return result;
    }
    TNode* qroot = ParseExpression(e);
    if (v.empty() && !e.empty()) {
        STMT_IDX_SET result = STMT_IDX_SET();
        auto data = assign_pattern_table_.GetData();
        for (auto tuple: data) {
            if (HasMatchingPattern(tuple.eroot, *qroot)) {
                result.insert(tuple.s);
            }
        }
        return result;
    }
    STMT_IDX_SET result = STMT_IDX_SET();
    auto data = assign_pattern_table_.GetData();
    for (auto tuple: data) {
        //debug
        std::cout << "matching with tuple {" << tuple.s << ", " << tuple.v << "}" << std::endl;
        std::cout << "tuple.v == v: " << (tuple.v == v) << std::endl;

        if (tuple.v == v && HasMatchingPattern(tuple.eroot, *qroot)) {
            std::cout << "they match!" << std::endl;
            result.insert(tuple.s);
        }
    }
    //debug
    std::cout << "printing through result set: " << std::endl;
    for (auto s: result) {
        std::cout << " s = " << s << std::endl;
    }
    return result;
}

EXPRESSION_TABLE AssignPatternTable::GetData() {
    return data_;
}

void AssignPatternTable::Add(STMT_IDX s, VAR_NAME v, TNode root) {
    ASSIGN_PATTERN tuple = {s, v, root};
    data_.push_back(tuple);
    //debug
    std::cout << "pringint all tuples in AssignPatternTable: " << std::endl;
    for (auto tuple: data_) {
        std::cout << "tuple: s = " << tuple.s << ", v = " << tuple.v << std::endl;
    }
}

STMT_IDX_LIST PatternManager::GetStmtIdxSetIntersection(STMT_IDX_SET s1, STMT_IDX_SET s2) {
    auto output = STMT_IDX_LIST();
    auto l1 = STMT_IDX_LIST(s1.begin(), s1.end());
    auto l2 = STMT_IDX_LIST(s2.begin(), s2.end());
    std::sort(l1.begin(), l1.end());
    std::sort(l2.begin(), l2.end());
    std::set_intersection(l1.begin(), l1.end(), l2.begin(), l2.end(), back_inserter(output));
    return output;
}

STMT_IDX_LIST PatternManager::GetStmtIdxSetIntersection(STMT_IDX_LIST l1, STMT_IDX_SET s2) {
    auto output = STMT_IDX_LIST();
    auto l2 = STMT_IDX_LIST(s2.begin(), s2.end());
    std::sort(l1.begin(), l1.end());
    std::sort(l2.begin(), l2.end());
    std::set_intersection(l1.begin(), l1.end(), l2.begin(), l2.end(), back_inserter(output));
    return output;
}

bool PatternManager::HasMatchingPattern(TNode root, TNode qroot) {
    vector<TNode*> children = root.GetChildrenVector();
    if (!children.empty()) {
        for (TNode* child: children) {
            if (HasMatchingPattern(*child, qroot)) {
                return true;
            };
        }
    } else {
        //debug
        std::cout << "node matching: qroot has node type " <<qroot.GetNodeType() <<", root has type " << root.GetNodeType() << std::endl;
        if (qroot.GetNodeType() == root.GetNodeType()) {
            if (root.GetNodeType() == TNode::NODE_TYPE::varName) {
                if (*(qroot.GetName()) == *(root.GetName())) {
                    return true;
                }
            } else if (root.GetNodeType() == TNode::NODE_TYPE::constValue) {
                std::cout << "node matching: qroot has value " << qroot.GetConstValue() << ", root has value " << root.GetConstValue() << std::endl;
                if ((int) qroot.GetConstValue() == (int) root.GetConstValue()) {
                    std::cout << "node matching: they are equal!" << std::endl;
                    return true;
                }
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
        //debug
        std::cout << "expression node has value " << val << std::endl;
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
STMT_VAR_PAIR_LIST PatternManager::GetAssignStmtVarPairWithFullPattern(VAR_NAME v, EXPRESSION e) {
    auto data = assign_pattern_table_.GetData();
    if (v.empty() && e.empty()) {
        STMT_VAR_PAIR_LIST result = STMT_VAR_PAIR_LIST();
        for (auto tuple: data) {
            STMT_VAR_PAIR pair = {tuple.s, tuple.v};
            result.push_back(pair);
        }
        return result;
    }
    if (e.empty()) {
        STMT_VAR_PAIR_LIST result = STMT_VAR_PAIR_LIST();
        for (auto tuple: data) {
            if (tuple.v == v) {
                STMT_VAR_PAIR pair = {tuple.s, tuple.v};
                result.push_back(pair);
            }
        }
        return result;
    }

    TNode* qroot = ParseExpression(e);
    if (v.empty()){
        STMT_VAR_PAIR_LIST result = STMT_VAR_PAIR_LIST();
        for (auto tuple: data) {
            if (HasMatchingPattern(tuple.eroot, *qroot)) {
                STMT_VAR_PAIR pair = {tuple.s, tuple.v};
                result.push_back(pair);
            }
        }
        return result;
    }
    STMT_VAR_PAIR_LIST result = STMT_VAR_PAIR_LIST();
    for (auto tuple: data) {
        if (tuple.v == v && HasMatchingPattern(tuple.eroot, *qroot)) {
            STMT_VAR_PAIR pair = {tuple.s, tuple.v};
            result.push_back(pair);
        }
    }
    return result;
}

STMT_VAR_PAIR_LIST PatternManager::GetAssignStmtVarPairWithSubPattern(VAR_NAME v, EXPRESSION e) {
    return STMT_VAR_PAIR_LIST();
}
STMT_IDX_SET PatternManager::GetAssignWithSubpattern(VAR_NAME v, EXPRESSION e) {
    return STMT_IDX_SET();
}
