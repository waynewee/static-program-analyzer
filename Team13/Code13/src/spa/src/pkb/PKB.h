#pragma once

#ifndef _PKB_H_
#define _PKB_H_

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <CustomTypes.h>
#include "DataManager.h"
#include "RelationManager.h"

using namespace std;

class TNode;

class PKB {
    static DataManager* data_manager_;
    static RelationManager* relation_manager_;
    static TNode* ast_;
public:
    PKB() {
    }
    DataManager* GetDataManager();
    RelationManager* GetRelationManager();

    void reset() {
        data_manager_ = new DataManager();
        relation_manager_ = new RelationManager();
    }
};

#endif