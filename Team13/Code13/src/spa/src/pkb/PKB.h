#pragma once

#ifndef _PKB_H_
#define _PKB_H_

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <CustomTypes.h>
#include <DataManager.h>
#include <RelationManager.h>
#include <DesignExtractor.h>
#include <PatternManager.h>

using namespace std;

class TNode;

class PKB {
    static DataManager* data_manager_;
    static RelationManager* relation_manager_;
    static PatternManager* pattern_manager_;
    static TNode* ast_;
public:
    PKB() {
        this->pattern_manager_ = new PatternManager(data_manager_->GetStmtTable(), relation_manager_);
    }
    DataManager* GetDataManager();
    RelationManager* GetRelationManager();

    void SetASTRoot(TNode* root);
    TNode* GetASTRoot();
};

class ASTRootTypeUnmatchException: public exception {
    virtual const char* what() const throw()
    {
        return "AST root node type can only be program!";
    }
};


#endif
