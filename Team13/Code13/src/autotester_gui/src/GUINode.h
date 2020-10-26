#ifndef _GUINODE_H_
#define _GUINODE_H_

#include <TNode.h>

using namespace std;

class GUINode {
public:
	TNode* parent_;
	TNode* node_;
	int x_; 
	int y_;
	GUINode(TNode* node, TNode* parent, int x, int y);

};

#endif