#pragma once

#include "PQLCustomTypes.h"

class QueryResult
{
public:
	STRING_SET result;

	STRING_SET GetResult();
	VOID SetResult(STRING_SET result);
	BOOLEAN IsEmpty();
};
