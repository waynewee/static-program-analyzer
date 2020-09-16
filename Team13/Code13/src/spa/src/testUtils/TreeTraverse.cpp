#include <iostream>
#include <queue>
#include <list>

#include "TreeTraverse.h"
#include "../pkb/TNode.h"

void TreeTraverse::traverse(TNode* root) {

	std::cout << "Traversing Tree" << std::endl;

	std::queue<TNode*> q;

	TNode* tNode = root;

	while (tNode != NULL) {

		list<TNode*> children = tNode->getChildren();

		for (TNode* child : children) {
			q.push(child);
			std::cout << child->getData() << std::endl;
		}
		std::cout << q.size() << std::endl;
		TNode* nextNode = q.back();
		q.pop();

		tNode = nextNode;

	}

}