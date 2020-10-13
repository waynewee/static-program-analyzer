#pragma once

#include "PQLCustomTypes.h"
#include "WhitespaceHandler.h"
class PQLTokenizer
{
public:
	static STRING_LIST TokenizeBySemicolons(STRING* query);

	static STRING RetrieveToken(STRING* str);

	static STRING RetrieveTokenByOpenBracket(STRING* str);

	static STRING RetrieveTokenByClosingAngleBracket(STRING* str);
};

