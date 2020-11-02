#include <PQLOptimization.h>

PQLOptimization::PQLOptimization() {
    this->occurrence_count_ = *(new STRING_INTEGER_MAP());
}

PQLOptimization::PQLOptimization(STRING_INTEGER_MAP occurrence_count) {
    this->occurrence_count_ = occurrence_count;
}

bool PQLOptimization::operator()(const STRING_LIST& lhs, const STRING_LIST& rhs) {
    int lhs_priority = ranks_of_functions.at(lhs[0]) + GetOccurrenceScore(lhs[1]) + GetOccurrenceScore(lhs[2]);
    int rhs_priority = ranks_of_functions.at(rhs[0]) + GetOccurrenceScore(rhs[1]) + GetOccurrenceScore(rhs[2]);
    return  lhs_priority < rhs_priority;
}

bool PQLOptimization::operator()(const STRING_PAIR& lhs, const STRING_PAIR& rhs) {
    int lhs_priority = GetOccurrenceScore(lhs.first) + GetOccurrenceScore(lhs.second);
    int rhs_priority = GetOccurrenceScore(rhs.first) + GetOccurrenceScore(rhs.second);;
   
    return  lhs_priority < rhs_priority;
}

int PQLOptimization::GetOccurrenceScore(string target) {
    int score = 0;

    if (Contains(target)) {
        score = occurrence_count_.at(target);
    }

    return score;
}

bool PQLOptimization::Contains(string value) {
    return occurrence_count_.find(value) != occurrence_count_.end();
}