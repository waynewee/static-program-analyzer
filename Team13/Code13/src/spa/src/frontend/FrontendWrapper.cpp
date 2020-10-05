#include <ASTBuilder.h>
#include <CFGBuilder.h>
#include <FileReader.h>
#include <FrontendWrapper.h>
#include <Tokenizer.h>
#include <TNode.h>

FrontendWrapper::FrontendWrapper(string file_name) {
	FileReader fileReader(file_name);
	string input = fileReader.ReadFile();
	Tokenizer tokenizer(input);
	token_list_ = tokenizer.GetTokenList();
}

TNode* FrontendWrapper::GetAST() {

	ASTBuilder ast_builder;
	TNode* ast_root_node = ast_builder.BuildMainPrgNode(token_list_);

	return ast_root_node;
}

void FrontendWrapper::GetCFG(TNode* ast_root_node) {

	for (TNode* proc_node: ast_root_node->GetChildrenVector()) {
		CFGBuilder cfg_builder(proc_node);
		cfg_builder.BuildCFG();
	}
}