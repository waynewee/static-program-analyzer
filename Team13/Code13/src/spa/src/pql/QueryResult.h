#pragma once

#include "PQLCustomTypes.h"

class QueryResult
{
public:
	STRING_SET_PTR result;

	STRING_SET_PTR GetResult();
	void SetResult(STRING_SET_PTR result);
	BOOLEAN isEmpty();
};
