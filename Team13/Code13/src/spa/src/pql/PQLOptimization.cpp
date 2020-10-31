#include <PQLOptimization.h>

PQLOptimization::PQLOptimization() {
    this->occurrence_count_ = *(new STRING_INTEGER_MAP());
}

PQLOptimization::PQLOptimization(STRING_INTEGER_MAP occurrence_count) {
    this->occurrence_count_ = occurrence_count;
}

STRING_INTEGER_MAP PQLOptimization::GetOccurrenceCount() {
    return this->occurrence_count_;
}

void PQLOptimization::SetOccurrenceCount(STRING_INTEGER_MAP occurrence_count) {
    this->occurrence_count_ = occurrence_count;
}

bool PQLOptimization::operator()(const STRING_LIST& lhs, const STRING_LIST& rhs) {
    int lhs_priority = ranks_of_functions.at(lhs[0]) /*+ occurrence_count_.at(lhs[1]) + occurrence_count_.at(lhs[2])*/;
    int rhs_priority = ranks_of_functions.at(rhs[0]) /* + occurrence_count_.at(rhs[1]) + occurrence_count_.at(rhs[2])*/;
    return  lhs_priority < rhs_priority;
}

bool PQLOptimization::operator()(const STRING_PAIR& lhs, const STRING_PAIR& rhs) {
    int lhs_priority = occurrence_count_.at(lhs.first) + occurrence_count_.at(lhs.second);
    int rhs_priority = occurrence_count_.at(rhs.first) + occurrence_count_.at(rhs.second);
    return  lhs_priority < rhs_priority;
}