#pragma once

#include "PQLCustomTypes.h"

class QueryResult
{
private:
	STRINGLIST_SET result;

public:
	QueryResult();

	STRINGLIST_SET GetResult();
	VOID SetResult(STRINGLIST_SET result);
};
