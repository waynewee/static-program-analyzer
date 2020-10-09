#pragma once

#ifndef _PKB_H_
#define _PKB_H_

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include <CFG.h>
#include <CFGManager.h>
#include <CustomTypes.h>
#include <DataManager.h>
#include <DesignExtractor.h>
#include <PatternManager.h>
#include <RelationManager.h>

using namespace std;

class TNode;

class PKB {
    static DataManager data_manager_;
    static RelationManager relation_manager_;
    static PatternManager pattern_manager_;
    static CFGManager cfg_manager_;
    static TNode ast_;
    static CFG cfg_;
public:
    PKB() {
    }
    TNode GetASTRoot();
    DataManager GetDataManager();
    RelationManager GetRelationManager();
    PatternManager GetPatternManager();
    CFGManager GetCFGManager();
    CFG& GetCFG();

    void SetASTRoot(TNode* root);
    void SetCFG(CFG& cfg);
};

class ASTRootTypeUnmatchException: public exception {
    virtual const char* what() const throw()
    {
        return "AST root node type can only be program!";
    }
};


#endif
