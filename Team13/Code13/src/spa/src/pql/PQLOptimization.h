#pragma once

#include <PQLCustomTypes.h>

class PQLOptimization {
private:
    STRING_INTEGER_MAP occurrence_count_;
    STRING_INTEGER_MAP ranks_of_functions = {
        { TYPE_COND_FOLLOWS,    1 },
        { TYPE_COND_MODIFIES_S, 2 },
        { TYPE_COND_MODIFIES_P, 3 },
        { TYPE_COND_PARENT,     4 },
        { TYPE_COND_USES_S,     5 },
        { TYPE_COND_USES_P,     6 },
        { TYPE_COND_CALLS,      7 },
        { TYPE_COND_FOLLOWS_T,  8 },
        { TYPE_COND_PARENT_T,   9 },
        { TYPE_COND_CALLS_T,    10 },
        { TYPE_COND_NEXT,       11 },
        { TYPE_COND_PATTERN_F,  12 },
        { TYPE_COND_PATTERN_P,  13 },
        { TYPE_COND_NEXT_T,     14 },
        { TYPE_COND_AFFECTS,    15 },
        { TYPE_COND_AFFECTS_T,  16 }
    };

public:
    PQLOptimization();

    PQLOptimization(STRING_INTEGER_MAP occurrence_count);

    bool operator()(const STRING_LIST& lhs, const STRING_LIST& rhs);
};