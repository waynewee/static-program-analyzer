#include "PQLCustomTypes.h"
#include "PQLParser.h"
#include "PQLParserStorage.h"
#include "QueryInfo.h"
#include "QuerySyntaxValidator.h"
#include <vector>
#include <unordered_set>
#include <sstream>
#include <iostream>
#include <regex>
#include <string>
using namespace std;

QueryInfo* PQLParser::Parse(string s) {
	QueryInfo* query_info = new QueryInfo();
	QuerySyntaxValidator* query_syntax_validator = new QuerySyntaxValidator();
	PQLParserStorage* PQL_parser_storage = new PQLParserStorage();
    string query = s;

    try {
        // get all the user declarations
        STANDARD_STRINGLIST all_declarations = SplitBySemicolons(&query);
        PQL_parser_storage->SetAllDeclarations(all_declarations);
        
        TrimLeadingWhitespaces(&query);

        for (auto decl : all_declarations) {
            TrimLeadingWhitespaces(&decl);
            string ent_type = decl.substr(0, decl.find_first_of(" "));

            STANDARD_STRING_STRING_MAP vars_declared = query_syntax_validator->ValidateDeclaration(decl);

            for (pair<string, string> var : vars_declared) {
                query_syntax_validator->ValidateVariableName(var.first);
                //cout << "1st : " << var.first << "2nd : " << entType << endl;
                PQL_parser_storage->StoreVariable(var, ent_type);
            }
        }

        query_syntax_validator->ValidateSelectClauseStartsWithSelect(query);
        // LEFT WITH select clause
        //cout << "query is now :" << query << endl;

        DeleteOneWordAndRetrieveIt(&query);
        //cout << "query is now :" << query << endl;

        string user_select_var = DeleteOneWordAndRetrieveIt(&query);
        // cout << "user_select var is :" << user_select_var << endl;
        // cout << "query is now :" << query << endl;

        query_syntax_validator->ValidateVariableExists(user_select_var, PQL_parser_storage->GetAllUserDeclaredVar());

        STRING_PTR user_output_var = new string(user_select_var);
        query_info->SetOutputVar(user_output_var);

        TrimLeadingWhitespaces(&query);

        if (query.empty()) {
            //cout << "query is empty already" << endl;
            query_info->PrintOutputVar();
            return query_info;
        }
        while (query.find_first_not_of(' ') != string::npos && !query.empty()) {
            TrimLeadingWhitespaces(&query);
            string such_that_with_one_spacing = query.substr(0, 9);
            // cout << suchThatWithOneSpacing << endl;
            // SUCH OR PATTERN
            string current_clause = DeleteOneWordAndRetrieveIt(&query);
            cout << "curr clause : " << current_clause << endl;
            if (current_clause.compare("such") != 0 && current_clause.compare("pattern") != 0) {
                throw ("Error : cannot find 'such that' or 'pattern' clause");
            }

            if (current_clause.compare("such") == 0) {
                if (such_that_with_one_spacing.compare("such that") != 0) {
                    throw ("Error : Invalid 'such that' clause");
                }
                string that_word = DeleteOneWordAndRetrieveIt(&query);
                STANDARD_STRING_STRINGLIST_MAP such_that_clause_result;
                if (that_word.compare("that") == 0) {
                    cout << "such that in query is now : " << query << endl;
                    string such_that_clause = query.substr(0, query.find_first_of(")") + 1);
                    query.erase(0, query.find_first_of(")") + 1);
                    such_that_clause_result = query_syntax_validator->ValidateSuchThatClause(such_that_clause, PQL_parser_storage->GetAllUserDeclaredVar());

                    PQL_parser_storage->StoreSuchThatClauseResult(such_that_clause_result);
                }
            }

            if (current_clause.compare("pattern") == 0) {
                STANDARD_STRING_STRINGLIST_MAP pattern_clause_result;
                string pattern_var_name = DeleteOneWordAndRetrieveIt(&query);
                cout << "pattern var name : " << pattern_var_name << endl;
                cout << "QUERY PATTERN : " << query << endl;
                pattern_clause_result = ParsePatternClause(&query, PQL_parser_storage->GetAllUserDeclaredVar(), query_syntax_validator);

                PQL_parser_storage->StorePatternClauseResult(pattern_clause_result, pattern_var_name);
            }
        }
        // such OR pattern
        //string current_clause = deleteOneWordAndRetrieveIt(&query);

        // unordered_map<string*, string*>* resultVarMap = new unordered_map<string*, string*>();
        STRING_STRING_MAP_PTR result_var_map = new STRING_STRING_MAP();
        result_var_map = ToPointerVarMap(PQL_parser_storage->GetVarMap());
        // unordered_map<string*, vector<vector<string*>*>*>* resultRelRefMap = new unordered_map<string*, vector<vector<string*>*>*>();
        STRING_STRINGLISTLIST_MAP_PTR result_rel_ref_map = new STRING_STRINGLISTLIST_MAP();
        result_rel_ref_map = ToPointerRelRefMap(PQL_parser_storage->GetRelRefMap());

        query_info->SetVarMap(result_var_map);
        query_info->SetRelRefMap(result_rel_ref_map);
    } 
    catch (const char* msg) {
        cerr << msg << endl;
        query_info->SetValidToFalse();
    }
	return query_info;
}

void PQLParser::TrimLeadingWhitespaces(string* s) {
    int last_leading_space = s->find_first_not_of(" ");
    s->erase(0, last_leading_space);
}

void PQLParser::TrimTrailingWhitespaces(string* s) {
    int first_trailing_space = s->find_last_not_of(" ") + 1;
    s->erase(first_trailing_space, s->length());
}

vector<string> PQLParser::SplitBySemicolons(string* query) {
    string delimiter = ";";
    size_t pos = 0;
    string token;
    vector<string> all_declarations;
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

string PQLParser::DeleteOneWordAndRetrieveIt(string* str) {
    TrimLeadingWhitespaces(str);
    string next_word = str->substr(0, str->find_first_of(" "));
    str->erase(0, str->find_first_of(" "));
    TrimLeadingWhitespaces(str);

    TrimLeadingWhitespaces(&next_word);
    TrimTrailingWhitespaces(&next_word);
    return next_word;
}

STANDARD_STRING_STRINGLIST_MAP PQLParser::ParsePatternClause(string* clause, STANDARD_STRING_STRING_MAP all_user_declared_var,
    QuerySyntaxValidator* query_syntax_validator) {
    STANDARD_STRING_STRINGLIST_MAP pattern_clause_result;
    if (clause->find("\"") != string::npos && clause->find("\"") < clause->find_first_of(")")) {
        int end_of_pattern_clause = query_syntax_validator->nthOccurrence(clause, "\"", 2);

        string temp = clause->substr(end_of_pattern_clause, clause->length());

        int distance_to_closed_bracket = temp.find_first_of(")") + 1;

        string pattern_clause = clause->substr(0, end_of_pattern_clause + distance_to_closed_bracket);

        // cout << "pattern clause : " << pattern_clause << endl;

        clause->erase(0, end_of_pattern_clause + distance_to_closed_bracket);

        pattern_clause_result = query_syntax_validator->ValidatePatternClause(pattern_clause, all_user_declared_var);
    }
    else {
        // cannot find ", so pattern variables are blank. we can find by next closing bracket
        int end_of_pattern_clause = clause->find_first_of(")") + 1;

        string pattern_clause = clause->substr(0, end_of_pattern_clause);

        // cout << "pattern clause : " << pattern_clause << endl;

        clause->erase(0, end_of_pattern_clause);

        pattern_clause_result = query_syntax_validator->ValidatePatternClause(pattern_clause, all_user_declared_var);
    }
    
    return pattern_clause_result;
}

STRING_STRING_MAP_PTR PQLParser::ToPointerVarMap(STANDARD_STRING_STRING_MAP str_map) {
    STRING_STRING_MAP_PTR result = new STRING_STRING_MAP();
    for (pair<string, string> elems : str_map) {
        string* first = new string(elems.first);
        string* second = new string(elems.second);
        result->insert({ first, second });
    }
    return result;
}

STRING_STRINGLISTLIST_MAP_PTR PQLParser::ToPointerRelRefMap(STANDARD_STRING_STRINGLISTLIST_MAP relRef_map) {
    STRING_STRINGLISTLIST_MAP_PTR result = new STRING_STRINGLISTLIST_MAP();
    for (pair<string, vector<vector<string>>> elems : relRef_map) {
        STRING_PTR first = new string(elems.first);
        STRINGLIST_LIST_PTR second = new STRINGLIST_LIST();
        for (vector<string> all_args : elems.second) {

            vector<string*>* all_args_ptr = new vector<string*>();
            for (string s : all_args) {
                STRING_PTR s_ptr = new string(s);
                all_args_ptr->push_back(s_ptr);
            }
            second->push_back(all_args_ptr);
        }
        result->insert({ first, second });
    }
    return result;
}