#pragma once

#include <PQLCustomTypes.h>

class PQLOptimization {
private:
    STRING_INTEGER_MAP occurrence_count_;
    STRING_INTEGER_MAP ranks_of_functions = {
        { TYPE_WITH_CLAUSE,     1 },
        { TYPE_COND_FOLLOWS,    2 },
        { TYPE_COND_MODIFIES_S, 3 },
        { TYPE_COND_MODIFIES_P, 4 },
        { TYPE_COND_PARENT,     5 },
        { TYPE_COND_USES_S,     6 },
        { TYPE_COND_USES_P,     7 },
        { TYPE_COND_CALLS,      8 },
        { TYPE_COND_FOLLOWS_T,  9 },
        { TYPE_COND_PARENT_T,   10 },
        { TYPE_COND_CALLS_T,    11 },
        { TYPE_COND_NEXT,       12 },
        { TYPE_COND_PATTERN_F,  13 },
        { TYPE_COND_PATTERN_P,  14 },
        { TYPE_COND_NEXT_T,     500 },
        { TYPE_COND_AFFECTS,    600 },
        { TYPE_COND_AFFECTS_T,  700 }
    };

public:
    PQLOptimization();
    PQLOptimization(STRING_INTEGER_MAP occurrence_count);

    int GetOccurrenceScore(string target);
    int GetTotalScore(int param1_score, int param2_score);
    string ParsingSynonym(string synonym);

    bool operator()(const STRING_LIST& lhs, const STRING_LIST& rhs);
};