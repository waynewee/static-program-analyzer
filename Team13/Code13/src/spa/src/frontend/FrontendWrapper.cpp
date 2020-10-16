#include <ASTBuilder.h>
#include <CFGBuilder.h>
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

vector<CFG*> FrontendWrapper::GetCFG(TNode* ast_root_node) {

	vector<CFG*> cfgs;

	for (TNode* proc_node: ast_root_node->GetChildrenVector()) {
		CFGBuilder cfg_builder(proc_node);
		CFG* cfg = cfg_builder.BuildCFG();
		cfgs.push_back(cfg);
	}

	return cfgs;
}