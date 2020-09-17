#include <iostream>
#include <queue>
#include <list>

#include "TreeTraverse.h"
#include "../pkb/TNode.h"

void TreeTraverse::traverse(TNode* root) {

	if (root == NULL) {
		return;
	}

	std::queue<TNode*> queue;

	queue.push(root);

	std::cout << "Printing Level Order Traversal" << std::endl;

	while (!queue.empty()) {
		TNode* node = queue.front();
		queue.pop();

		std::cout << node->getData() << std::endl;

		for (TNode* cNode : node->GetChildrenVector()) {
			queue.push(cNode);
		}
	}

}