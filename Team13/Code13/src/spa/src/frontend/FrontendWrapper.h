#ifndef _FRONTENDWRAPPER_H_
#define _FRONTENDWRAPPER_H_

#include <string>

#include <FrontendTypes.h>
#include <TNode.h>

using namespace std;

class FrontendWrapper {
public:
	FrontendWrapper::FrontendWrapper(string file_name);

	TOKEN_LIST token_list_;

	TNode* GetAST();
	//CFGNode* GetCFG();

private:
};

#endif