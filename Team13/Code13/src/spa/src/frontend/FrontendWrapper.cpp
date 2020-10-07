#include <ASTBuilder.h>
#include <FileReader.h>
#include <FrontendWrapper.h>
#include <Tokenizer.h>
#include <TNode.h>
#include <SimpleValidator.h>

FrontendWrapper::FrontendWrapper(string file_name) {
	FileReader fileReader(file_name);

	string input = fileReader.ReadFile();

	Tokenizer tokenizer(input);

	token_list_ = tokenizer.GetTokenList();
	try {
		SimpleValidator simple_validator;
		simple_validator.IsValid(tokenizer.GetTokenList());
	}
	catch (string exception) {
		std::cout << exception << std::endl;
		exit(1);
	}
	
}

TNode* FrontendWrapper::GetAST() {

	ASTBuilder ast_builder;
	TNode* ast_root_node = ast_builder.BuildMainPrgNode(token_list_);

	return ast_root_node;
}