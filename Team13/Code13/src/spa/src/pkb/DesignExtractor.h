#ifndef DESIGN_EXTRACTOR_H
#define DESIGN_EXTRACTOR_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"
#include "TNode.h"
#include "CustomTypes.h"

using namespace std;

// class DesignExtractor {
// public:
bool ExtractFollows(RelationManager manager, TNode root);
bool ExtractParent(RelationManager manager, TNode root);
bool ExtractModifies(RelationManager manager, TNode root);
bool ExtractUses(RelationManager manager, TNode root);
bool ExtractData(DataManager manager, TNode root);
bool ExtractPattern(PatternManager manager, TNode root);

// };

#endif