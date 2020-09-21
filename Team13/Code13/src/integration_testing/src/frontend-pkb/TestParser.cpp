#include <frontend/SimpleParser.h>
#include <frontend/Tokenizer.h>
#include <testUtils/Statement.h>
#include <catch.hpp>
#include <TNode.h>
#include <queue>

#include "ParserTestCases.h"

SimpleParser parser = SimpleParser();
vector<TNode*> InOrderTraversal(TNode* root) {

	std::vector<TNode*> inOrder;
	std::queue<TNode*> temp;

	inOrder.push_back(root);
	temp.push(root);

	if (root == NULL) {
		return inOrder;
	}

	while (!temp.empty()) {
		TNode* curr = temp.front();
		temp.pop();

		for (TNode* child : curr->GetChildrenVector()) {
			inOrder.push_back(child);
			temp.push(child);
		}
	}
	return inOrder;
}

bool SameTNode(TNode* n1, TNode* n2) {
	std::cout << "n1: " << n1->getData()
		<< " | n2: " << n2->getData() << std::endl;
	return n1->getData() == n2->getData();
}

bool SameAST(vector<TNode*> v1, vector<TNode*> v2) {
	if (v1.size() != v2.size()) {
		std::cout << " why : (" << std::endl;
		return false;
	}
	for (int i = 0; i < v1.size(); i++) {
		if (!SameTNode(v1.at(i), v2.at(i))) {
			return false;
		}
	}
	return true;
}

TEST_CASE("Parsing assignment statement for variable to simple expression") {
	Tokenizer tokenizer(*(AssgnTest1.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = parser.Parse(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest1.root)));
}

TEST_CASE("Parsing assignment statement for variable to constant") {
	Tokenizer tokenizer(*(AssgnTest2.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = parser.Parse(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest2.root)));
}

TEST_CASE("Parsing assignment statement for variable to variable") {
	Tokenizer tokenizer(*(AssgnTest3.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = parser.Parse(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest3.root)));
}

TEST_CASE("Parsing assignment statement for ") {
	Tokenizer tokenizer(*(AssgnTest4.SourceProgram));
	tokenizer.Tokenize();
	TNode* actualRoot = parser.Parse(tokenizer.GetTokenList());
	std::cout << "Test Case 4's AST: " << std::endl;
	AssgnTest4.root->Print(AssgnTest4.root);

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest4.root)));
}
/*
TEST_CASE("Parsing regular assignment statement") {
	Tokenizer tokenizer(AssgnTest1.SourceProgram);
	tokenizer.Tokenize();
	TNode* actualRoot = parser.Parse(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest1.root)));
}

TEST_CASE("Parsing regular assignment statement") {
	Tokenizer tokenizer(AssgnTest1.SourceProgram);
	tokenizer.Tokenize();
	TNode* actualRoot = parser.Parse(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest1.root)));
}

TEST_CASE("Parsing regular assignment statement") {
	Tokenizer tokenizer(AssgnTest1.SourceProgram);
	tokenizer.Tokenize();
	TNode* actualRoot = parser.Parse(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest1.root)));
}

TEST_CASE("Parsing regular assignment statement") {
	Tokenizer tokenizer(AssgnTest1.SourceProgram);
	tokenizer.Tokenize();
	TNode* actualRoot = parser.Parse(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest1.root)));
}

TEST_CASE("Parsing regular assignment statement") {
	Tokenizer tokenizer(AssgnTest1.SourceProgram);
	tokenizer.Tokenize();
	TNode* actualRoot = parser.Parse(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest1.root)));
}

TEST_CASE("Parsing regular assignment statement") {
	Tokenizer tokenizer(AssgnTest1.SourceProgram);
	tokenizer.Tokenize();
	TNode* actualRoot = parser.Parse(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest1.root)));
}

TEST_CASE("Parsing regular assignment statement") {
	Tokenizer tokenizer(AssgnTest1.SourceProgram);
	tokenizer.Tokenize();
	TNode* actualRoot = parser.Parse(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest1.root)));
}

TEST_CASE("Parsing regular assignment statement") {
	Tokenizer tokenizer(AssgnTest1.SourceProgram);
	tokenizer.Tokenize();
	TNode* actualRoot = parser.Parse(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest1.root)));
}

TEST_CASE("Parsing regular assignment statement") {
	Tokenizer tokenizer(AssgnTest1.SourceProgram);
	tokenizer.Tokenize();
	TNode* actualRoot = parser.Parse(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest1.root)));
}

TEST_CASE("Parsing regular assignment statement") {
	Tokenizer tokenizer(AssgnTest1.SourceProgram);
	tokenizer.Tokenize();
	TNode* actualRoot = parser.Parse(tokenizer.GetTokenList());

	REQUIRE(SameAST(InOrderTraversal(actualRoot), InOrderTraversal(AssgnTest1.root)));
}*/
