#include <ASTBuilder.h>
#include <CFGBuilder.h>
#include <FileReader.h>
#include <FrontendWrapper.h>
#include <Tokenizer.h>
#include <TNode.h>
#include <SimpleValidator.h>

FrontendWrapper::FrontendWrapper(string file_contents) {
	Tokenizer tokenizer(file_contents);
	token_list_ = tokenizer.GetTokenList();

	try {
		SimpleValidator simple_validator;
		simple_validator.IsValid(tokenizer.GetTokenList());
	}
	catch (char* exception) {
		std::cout << exception << std::endl;
		exit(1);
	}
}

TNode* FrontendWrapper::GetAST() {

	ASTBuilder ast_builder;
	TNode* ast_root_node = ast_builder.BuildMainPrgNode(token_list_);

	return ast_root_node;
}

CFG* FrontendWrapper::GetCFG(TNode* ast_root_node) {

	CFGBuilder cfg_builder = CFGBuilder(ast_root_node);
	CFG* cfg = cfg_builder.GetCFG();

	return cfg;
}