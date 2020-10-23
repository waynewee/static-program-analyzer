#include <PQLOptimization.h>

PQLOptimization::PQLOptimization() {}

PQLOptimization::PQLOptimization(STRING_INTEGER_MAP occurrence_count) {
    occurrence_count_ = occurrence_count;
}

bool PQLOptimization::operator()(const STRING_LIST& lhs, const STRING_LIST& rhs) {
    int lhs_priority = ranks_of_functions.at(lhs[0]) + occurrence_count_.at(lhs[1]) + occurrence_count_.at(lhs[2]);
    int rhs_priority = ranks_of_functions.at(rhs[0]) + occurrence_count_.at(rhs[1]) + occurrence_count_.at(rhs[2]);
    return  lhs_priority < rhs_priority;
}