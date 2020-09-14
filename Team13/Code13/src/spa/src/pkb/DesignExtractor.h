#ifndef DESIGN_EXTRACTOR_H
#define DESIGN_EXTRACTOR_H

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"
#include "CustomTypes.h"

using namespace std;

class DesignExtractor {
public:
	bool extractFollows(RelationManager manager, TNode* root);
};

#endif