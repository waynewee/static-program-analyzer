#include "PQLParser.h"
#include "QueryInfo.h"
#include "QuerySyntaxValidator.h"
#include "WhitespaceHandler.h"
#include <vector>
#include <unordered_set>
#include <sstream>
#include <iostream>
#include <regex>
#include <string>
using namespace std;

/*
Main Query Parser of the PQL.
Calls QuerySyntaxValidator to validate each portion, before attempting to parse.
*/
QueryInfo PQLParser::Parse(STRING s) {
    QueryInfo query_info;

    QuerySyntaxValidator* query_syntax_validator = new QuerySyntaxValidator();
    STRING query = s;
    BOOLEAN is_query_valid = true;

    // Tables to build from parsing and pass to queryInfo
    STRING_STRING_MAP entity_map;
    STRING_STRINGLISTLIST_MAP st_map;
    STRING_STRINGLISTLIST_MAP pattern_map;
    STRINGPAIR_SET with_map;
    STRING_LIST output_list;

    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&query);

    try {
        STRING_LIST all_declarations = SplitBySemicolons(&query);
        for (auto decl : all_declarations) {
            WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&decl);
            STRING design_entity_type = decl.substr(0, decl.find_first_of(" "));
            // VALIDATION
            if (!query_syntax_validator->ValidateDeclaration(decl)) {
                is_query_valid = false;
            }
            // Get the result for each declaration
            STRING_STRING_MAP declaration_result;
            declaration_result = ParseDeclaration(decl);

            for (STRING_PAIR var : declaration_result) {
                entity_map[var.first] = var.second;
            }
        }

        STRING supposed_select_token = RetrieveToken(&query);

        if (!query_syntax_validator->ValidateSelect(supposed_select_token)) {
            // cout << "NOT SELECT!" << endl;
            is_query_valid = false;
            throw ("Error : At PQLParser, Validate Select clause");
        }

        STRING supposed_result_cl;

        if (query.find(">") != STRING::npos) {
            // found >, so we know that tuple has multiple elems : e.g. Select <s1, s2>
            // cout << "FOUND > " << endl;
            supposed_result_cl = RetrieveTokenByClosingAngleBracket(&query);
            if (!query_syntax_validator->ValidateResultClause(supposed_result_cl, entity_map)) {
                is_query_valid = false;
                throw ("Error : At PQLParser, Validating Result Clause");
            }
            // cout << "Tuple supposed result cl :" << supposed_result_cl << "|" << endl;
            ParseResultClauseTuple(supposed_result_cl, &output_list);
        }
        else {
            // didn't find >, so we know that it's just one elem : e.g. Select s1
            // cout << "didnt find" << endl;

            supposed_result_cl = RetrieveToken(&query);
            if (!query_syntax_validator->ValidateResultClause(supposed_result_cl, entity_map)) {
                is_query_valid = false;
                throw ("Error : At PQLParser, Validating Result Clause");
            }
            // Validated, Time to Parse
            // cout << "Tuple supposed result cl :" << supposed_result_cl << "|" << endl;
            ParseResultClauseElem(supposed_result_cl, &output_list);
        }

        // cout << "remaining:" << query << "|" << endl;

        /*
        What's remaining is either the suchthat-clause, with-clause or pattern-clause.
        This loop erases the clause after parsing them, and loops until the string is empty.
        If it's a suchthat or with clause,
        */
        regex r("such that|pattern|with|and");
        STRING current_clause_type;
        BOOLEAN current_is_and_clause = false;
        BOOLEAN first_passed = false;
        while (query.find_first_not_of(' ') != STRING::npos && !query.empty()) {
            STRING current_token;
            STRING clause_type;
            STRING supposed_such_that = query.substr(0, 10);
            smatch m;
            // cout << "supposed_such_that:" << supposed_such_that << "|" << endl;
            WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&supposed_such_that);
            // query.erase(0, 10);

            if (supposed_such_that.compare(TYPE_SUCH_THAT_CLAUSE) == 0) {
                clause_type = TYPE_SUCH_THAT_CLAUSE;
                current_clause_type = TYPE_SUCH_THAT_CLAUSE;
                RetrieveToken(&query); // delete 'such' from the query
                RetrieveToken(&query); // delete 'that' from the query
                current_is_and_clause = false;
            }
            else {
                clause_type = RetrieveToken(&query);
                if (clause_type.compare(TYPE_PATTERN_CLAUSE) == 0) {
                    current_clause_type = TYPE_PATTERN_CLAUSE;
                    current_is_and_clause = false;
                }
                else if (clause_type.compare(TYPE_WITH_CLAUSE) == 0) {
                    current_clause_type = TYPE_WITH_CLAUSE;
                    current_is_and_clause = false;
                }
                else if (clause_type.compare(TYPE_AND_CLAUSE) == 0 && first_passed) {
                    clause_type = current_clause_type;
                    current_is_and_clause = true;
                }
                else {
                    // all clause types are wrong
                    is_query_valid = false;
                    throw ("Error : Clause type is not suchthat/with/and/pattern");
                }
            }
            first_passed = true;  // first clause has parsed, now 'and' can be allowed in place of such that / pattern / with

            // cout << "remainingQuery:" << query << endl;
            STRING clause_arguments;
            if (regex_search(query, m, r)) {
                clause_arguments = m.prefix().str();
                // cout << "clause_arguments:" << clause_arguments << "|" << endl;
                query.erase(0, clause_arguments.length());
            }
            else {
                // cant find the regex, this is the last clause, or if this is the only clause
                clause_arguments = query;
                query.erase(0, clause_arguments.length());
            }
            STRING full_clause;
            if (clause_type.compare(TYPE_AND_CLAUSE) == 0) {
                full_clause = current_clause_type;
            }
            else {
                // it's not 'and'
                full_clause = clause_type;
            }
            full_clause.append(" ");
            full_clause.append(clause_arguments);
            WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&full_clause);
            // cout << "full_clause:" << full_clause << endl;
            // cout << "REMAINING QUERY:" << query << endl;

            // FOR EACH CLAUSE TYPE
            // cout << "PERFORM VALIDATION ON CLAUSE:" << full_clause << endl;

            // cout << "clause_type:" << clause_type << "|" << endl;
            if (clause_type.compare(TYPE_SUCH_THAT_CLAUSE) == 0) {
                if (query_syntax_validator->ValidateSuchthatClause(full_clause, entity_map)) {
                    full_clause.erase(0, 9); // erase such that away
                    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&full_clause);
                    STRING relref_type = query_syntax_validator->GetValidRelRefType(full_clause, entity_map);
                    full_clause.erase(0, full_clause.find_first_of("(")); // erase relref away
                    // cout << "full clause should have the opening bracket" << full_clause << endl;
                    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&full_clause);
                    // cout << "FULLCLAUSE ARGUMENTS:" << full_clause << "|" << endl;
                    STRING_LIST arguments = ExtractArguments(full_clause);
                    if (st_map.count(relref_type) != 0) {
                        // relref already there
                        st_map.at(relref_type).push_back(arguments);
                    }
                    else {
                        // relref not there
                        STRINGLIST_LIST list_of_list_of_arguments;
                        list_of_list_of_arguments.push_back(arguments);
                        st_map[relref_type] = list_of_list_of_arguments;
                    }
                }
                else {
                    is_query_valid = false;
                    throw ("Invalid Query : at suchthat clause");
                }
            }

            if (clause_type.compare(TYPE_PATTERN_CLAUSE) == 0) {
                if (query_syntax_validator->ValidatePatternClause(full_clause, entity_map)) {
                    full_clause.erase(0, full_clause.find_first_of(" "));
                    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&full_clause); // pattern is erased away
                    STRING pattern_select_var = RetrieveToken(&full_clause);
                    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&full_clause);
                    STRING_LIST pattern_result = ExtractArguments(full_clause);
                    pattern_result.push_back(pattern_select_var);
                    STRINGLIST_LIST list_of_pattern_result;
                    if (IsPatternPartial(full_clause)) {
                        STRING expr_spec = pattern_result.at(1);
                        expr_spec = expr_spec.substr(1, expr_spec.length() - 2);
                        pattern_result.at(1) = expr_spec;
                        list_of_pattern_result.push_back(pattern_result);
                        if (pattern_map.count(TYPE_COND_PATTERN_P) == 0) {
                            pattern_map[TYPE_COND_PATTERN_P] = list_of_pattern_result;
                        }
                        else {
                            pattern_map.at(TYPE_COND_PATTERN_P).push_back(pattern_result);
                        }
                    }
                    else {
                        list_of_pattern_result.push_back(pattern_result);
                        if (pattern_map.count(TYPE_COND_PATTERN_F) == 0) {
                            pattern_map[TYPE_COND_PATTERN_F] = list_of_pattern_result;
                        }
                        else {
                            pattern_map.at(TYPE_COND_PATTERN_F).push_back(pattern_result);
                        }
                    }
                }
                else {
                    is_query_valid = false;
                    throw ("Invalid Query : at pattern clause");
                }
            }

            if (clause_type.compare(TYPE_WITH_CLAUSE) == 0) {
                if (query_syntax_validator->ValidateWithClause(full_clause, entity_map)) {
                    full_clause.erase(0, full_clause.find_first_of(" "));
                    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&full_clause); // erase with clause

                }
            }
        }
    }
    catch (const char* msg) {
        //cerr << msg << endl;
        cout << "Error caught" << endl;
        is_query_valid = false;
        query_info.SetValidToFalse();
    }
    catch (...) {
        //cerr << msg << endl;
        cout << "Error caught" << endl;
        is_query_valid = false;
        query_info.SetValidToFalse();
    }

    if (!is_query_valid) {
        query_info.SetValidToFalse();
        cout << "QUERY IS INVALID:" << endl;
    }
    else {
        // QUERY IS VALID BLOCK!!!!!
        cout << "QUERY IS VALID:" << endl;
        query_info.SetEntityMap(entity_map);
        query_info.SetOutputList(output_list);
        query_info.SetStMap(st_map);
        query_info.SetPatternMap(pattern_map);
    }
    
    query_info.PrintEntityMap();
    query_info.PrintOutputList();
    query_info.PrintStMap();
    query_info.PrintPatternMap();
    
    return query_info;
}

STRING_LIST PQLParser::SplitBySemicolons(string* query) {
    string delimiter = ";";
    size_t pos = 0;
    string token;
    STRING_LIST all_declarations;
    if ((pos = query->find(delimiter)) == string::npos) {
        throw "Error : Invalid format, ';' not found";
    }

    while ((pos = query->find(delimiter)) != string::npos) {
        token = query->substr(0, pos);
        all_declarations.push_back(token);
        query->erase(0, pos + delimiter.length());
    }
    return all_declarations;
}

STRING_STRING_MAP PQLParser::ParseDeclaration(STRING decl) {
    STRING_STRING_MAP variable_names_declared;
    STRING_LIST temp_results;
    STRING decl_clone = decl;
    size_t pos = 0;
    STRING token;
    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&decl_clone);

    while ((pos = decl_clone.find(",")) != STRING::npos) {
        token = decl_clone.substr(0, pos);
        WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&token);
        temp_results.push_back(token);
        decl_clone.erase(0, pos + 1);
        WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&decl_clone);
    }
    temp_results.push_back(decl_clone); // last declaration after the ending comma
    // cout << "trying to call front" << endl;
    STRING entity_synonym = temp_results.front(); // this is "assign a" in "assign a, b, c"
    // cout << "front called" << endl;
    temp_results.erase(temp_results.begin());

    STRING entity_type = entity_synonym.substr(0, entity_synonym.find_first_of(" "));
    STRING first_synonym = entity_synonym.substr(entity_synonym.find_first_of(" "), entity_synonym.length());
    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_synonym);

    variable_names_declared[first_synonym] = entity_type;

    for (auto synonym : temp_results) {
        variable_names_declared[synonym] = entity_type;
    }
    /*
    cout << "-------varmap:--------" << endl;
    for (auto elem : variable_names_declared) {
        cout << elem.first << " -> " << elem.second << endl;
    }
    */
    return variable_names_declared;
}


STRING PQLParser::RetrieveToken(STRING* str) {
    WhitespaceHandler::TrimLeadingWhitespaces(str);
    STRING next_word = str->substr(0, str->find_first_of(" "));
    str->erase(0, str->find_first_of(" "));
    WhitespaceHandler::TrimLeadingWhitespaces(str);
    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&next_word);
    return next_word;
}

STRING PQLParser::RetrieveTokenByOpenBracket(STRING* str) {
    WhitespaceHandler::TrimLeadingWhitespaces(str);
    STRING next_word = str->substr(0, str->find_first_of("("));
    str->erase(0, str->find_first_of("("));
    WhitespaceHandler::TrimLeadingWhitespaces(str);
    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&next_word);
    return next_word;
}

STRING PQLParser::RetrieveTokenByClosingAngleBracket(STRING* str) {
    WhitespaceHandler::TrimLeadingWhitespaces(str);
    STRING next_word = str->substr(0, str->find_first_of(">") + 1);
    str->erase(0, str->find_first_of(">") + 1);
    WhitespaceHandler::TrimLeadingWhitespaces(str);
    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&next_word);
    return next_word;
}

VOID PQLParser::ParseResultClauseElem(STRING token, STRING_LIST* output_list) {
    output_list->push_back(token);
}

VOID PQLParser::ParseResultClauseTuple(STRING token, STRING_LIST* output_list) {
    STRING temp_token = token;
    STRING sub_token;
    STRING comma = ",";
    size_t pos = 0;
    // erase front and back <>
    temp_token = temp_token.substr(1, temp_token.length() - 2);
    while ((pos = temp_token.find(comma)) != STRING::npos) {
        sub_token = temp_token.substr(0, pos);
        // cout << "subtoken:" << sub_token << "|" << endl;
        WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&sub_token);
        output_list->push_back(sub_token);
        temp_token.erase(0, temp_token.find_first_of(",") + 1);
    }
    // last token : 
    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
    output_list->push_back(temp_token);
}

STRING_LIST PQLParser::ExtractArguments(STRING token) {
    // takes a bracket with two arguments , e.g. ( s1, s2) with any amount of whitespace. If argument has "", trim the whitespace inside.
    STRING temp_token = token;
    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
    // cout << "temp token:" << temp_token << endl;
    temp_token = temp_token.substr(1, temp_token.length() - 2); // erasing opening and closing brackets
    // cout << "Trimmed temp_token:" << temp_token << endl;
    INTEGER comma_count = 0;
    for (int i = 0; i < temp_token.size(); i++) {
        if (temp_token[i] == ',') comma_count++;
    }
    STRING_LIST argument_list;
    if (comma_count == 1) {
        // ONLY TWO ARGS
        STRING first_arg = temp_token.substr(0, temp_token.find_first_of(","));
        STRING second_arg = temp_token.substr(temp_token.find_first_of(",") + 1, temp_token.length());
        // cout << "EXTRACTING FIRST ARG: " << first_arg << endl;
        // cout << "1st arg:" << first_arg << endl;
        // cout << "2nd arg:" << second_arg << endl;
        WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
        WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
        STRING first_arg_first_two_chars;
        STRING first_arg_last_two_chars;
        STRING second_arg_first_two_chars;
        STRING second_arg_last_two_chars;
        if (first_arg.length() > 2) {
            first_arg_first_two_chars = first_arg.substr(0, 2);
            first_arg_last_two_chars = first_arg.substr(first_arg.length() - 2, first_arg.length());
        }
        if (second_arg.length() > 2) {
            second_arg_first_two_chars = second_arg.substr(0, 2);
            second_arg_last_two_chars = second_arg.substr(second_arg.length() - 2, second_arg.length());
        }
        //cout << "first_arg_first_two_chars:" << first_arg_first_two_chars << "|" << endl;
        //cout << "first_arg_last_two_chars:" << first_arg_last_two_chars << "|" << endl;
        //cout << "second_arg_first_two_chars:" << second_arg_first_two_chars << "|" << endl;
        // cout << "second_arg_last_two_chars:" << second_arg_last_two_chars << "|" << endl;
        if (first_arg.front() == '\"' && first_arg.back() == '\"') {
            first_arg = first_arg.substr(1, first_arg.length() - 2);
            WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
            // cout << "1st arg: " << first_arg << "|" << endl;
            // append back " "
            // cout << "PUTTING BACK ARGS:" << endl;
            // cout << "first Arg : " << first_arg << endl;
            STRING first_arg_result = "\"";
            first_arg_result.append(first_arg);
            first_arg_result.append("\"");
            argument_list.push_back(first_arg_result);
        }
        else if (first_arg_first_two_chars.compare("_\"") == 0 && first_arg_last_two_chars.compare("\"_") == 0) {
            first_arg = first_arg.substr(2, first_arg.length() - 4);
            WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
            // cout << "1st arg: " << first_arg << "|" << endl;
            // append back " "
            STRING first_arg_result = "_\"";
            first_arg_result.append(first_arg);
            first_arg_result.append("\"_");
            argument_list.push_back(first_arg_result);
        }
        else {
            argument_list.push_back(first_arg);
        }
        if (second_arg.front() == '\"' && second_arg.back() == '\"') {
            second_arg = second_arg.substr(1, second_arg.length() - 2);
            WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
            // cout << "2nd arg: "<< second_arg << "|" << endl;
            // append back " "
            STRING second_arg_result = "\"";
            second_arg_result.append(second_arg);
            second_arg_result.append("\"");
            argument_list.push_back(second_arg_result);
        }
        else if (second_arg_first_two_chars.compare("_\"") == 0 && second_arg_last_two_chars.compare("\"_") == 0) {
            // cout << "YES EQUAL";
            second_arg = second_arg.substr(2, second_arg.length() - 4);
            WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
            //  cout << "2nd arg: "<< second_arg << "|" << endl;
             // append back " "
            STRING second_arg_result = "_\"";
            second_arg_result.append(second_arg);
            second_arg_result.append("\"_");
            argument_list.push_back(second_arg_result);
        }
        else {
            argument_list.push_back(second_arg);
        }
    }
    if (comma_count == 2) {
        // three args
        STRING first_arg = temp_token.substr(0, temp_token.find_first_of(","));
        temp_token.erase(0, temp_token.find_first_of(",") + 1);
        STRING second_arg = temp_token.substr(0, temp_token.find_first_of(","));
        STRING third_arg = temp_token.substr(temp_token.find_first_of(",") + 1, temp_token.length());
        WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
        WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
        WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&third_arg);
        // only first arg need to be entref.
        if (first_arg.front() == '\"' && first_arg.back() == '\"') {
            first_arg = first_arg.substr(1, first_arg.length() - 2);
            WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
            // cout << "1st arg: " << first_arg << "|" << endl;
            // append back " "
            STRING first_arg_result = "\"";
            first_arg_result.append(first_arg);
            first_arg_result.append("\"");
            argument_list.push_back(first_arg_result);
        }
        else {
            argument_list.push_back(first_arg);
        }
        argument_list.push_back(second_arg);
        argument_list.push_back(third_arg);
    }
    /*
    for (auto s : argument_list) {
        cout << "extracted args:" << s << "|" << endl;
    }
    */
    return argument_list;
}


BOOLEAN PQLParser::IsPatternPartial(STRING token) {
    BOOLEAN is_partial = false;
    STRING temp_token = token;
    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
    temp_token = temp_token.substr(1, temp_token.length() - 2); // erasing opening and closing brackets
    // cout << "Trimmed temp_token:" << temp_token << endl;
    STRING first_arg = temp_token.substr(0, temp_token.find_first_of(","));
    STRING second_arg = temp_token.substr(temp_token.find_first_of(",") + 1, temp_token.length());
    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);

    STRING second_arg_first_two_chars;
    STRING second_arg_last_two_chars;
    if (second_arg.length() > 2) {
        second_arg_first_two_chars = second_arg.substr(0, 2);
        second_arg_last_two_chars = second_arg.substr(second_arg.length() - 2, second_arg.length());
    }

    if (second_arg_first_two_chars.compare("_\"") == 0 && second_arg_last_two_chars.compare("\"_") == 0) {
        is_partial = true;
    }

    return is_partial;
}


/*
STRING_STRINGLIST_MAP PQLParser::ParsePatternClause(string* clause, STRING_STRING_MAP all_user_declared_var,
    QuerySyntaxValidator* query_syntax_validator) {
    STRING_STRINGLIST_MAP patternClauseResult;
    if (clause->find("\"") != string::npos && clause->find("\"") < clause->find_first_of(")")) {
        int endOfPatternClause = query_syntax_validator->NthOccurrence(clause, "\"", 2);

        string temp = clause->substr(endOfPatternClause, clause->length());

        int distanceToClosedBracket = temp.find_first_of(")") + 1;

        string pattern_clause = clause->substr(0, endOfPatternClause + distanceToClosedBracket);

        // cout << "pattern clause : " << pattern_clause << endl;

        clause->erase(0, endOfPatternClause + distanceToClosedBracket);

        patternClauseResult = query_syntax_validator->ValidatePatternClause(pattern_clause, all_user_declared_var);
    }
    else {
        // cannot find ", so pattern variables are blank. we can find by next closing bracket
        int endOfPatternClause = clause->find_first_of(")") + 1;

        string pattern_clause = clause->substr(0, endOfPatternClause);

        // cout << "pattern clause : " << pattern_clause << endl;

        clause->erase(0, endOfPatternClause);

        patternClauseResult = query_syntax_validator->ValidatePatternClause(pattern_clause, all_user_declared_var);
    }

    return patternClauseResult;
}
*/