#include "PQLCustomTypes.h"

#pragma once
class WhitespaceHandler
{
public:
	static void TrimLeadingWhitespaces(STRING* s);
	static void TrimTrailingWhitespaces(STRING* s);
	static void TrimLeadingAndTrailingWhitespaces(STRING* s);
};

