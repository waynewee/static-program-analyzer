#pragma once

#include "PQLCustomTypes.h"

class QueryResult
{
public:
	STRING_SET_PTR result;

	STRING_SET_PTR getResult();
	void setResult(STRING_SET_PTR result);
	BOOLEAN isEmpty();
};
