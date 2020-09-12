#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include <pkb/PKB.h>
#include <pkb/TNode.h>
DataManager *PKB::GetDataManager() {
    return data_manager_;
}
RelationManager* PKB::GetRelationManager() {
    return relation_manager_;
}
