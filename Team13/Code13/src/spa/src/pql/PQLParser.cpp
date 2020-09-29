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

QueryInfo PQLParser::Parse(string s) {
	QueryInfo query_info;
	QuerySyntaxValidator* query_syntax_validator = new QuerySyntaxValidator();
	PQLParserStorage* PQL_parser_storage = new PQLParserStorage();
    string query = s;

    try {
        // get all the user declarations
        STRING_LIST all_declarations = SplitBySemicolons(&query);
        PQL_parser_storage->SetAllDeclarations(all_declarations);
        
        TrimLeadingWhitespaces(&query);

        for (auto decl : all_declarations) {
            TrimLeadingWhitespaces(&decl);
            string entType = decl.substr(0, decl.find_first_of(" "));

            STRING_STRING_MAP vars_declared = query_syntax_validator->ValidateDeclaration(decl);

            for (pair<string, string> var : vars_declared) {
                query_syntax_validator->ValidateVariableName(var.first);
                //cout << "1st : " << var.first << "2nd : " << entType << endl;
                PQL_parser_storage->StoreVariable(var, entType);
            }
        }

        query_syntax_validator->ValidateSelectClauseStartsWithSelect(query);
        // LEFT WITH select clause
        //cout << "query is now :" << query << endl;

        DeleteOneWordAndRetrieveIt(&query);
        //cout << "query is now :" << query << endl;

        string user_select_var = DeleteOneWordAndRetrieveIt(&query);
        //cout << "user_select var is :" << user_select_var << endl;
        //cout << "query is now :" << query << endl;

        query_syntax_validator->ValidateVariableExists(user_select_var, PQL_parser_storage->GetAllUserDeclaredVar());

        query_info.SetOutputVar(user_select_var);

        TrimLeadingWhitespaces(&query);

        if (query.empty()) {
            //cout << "query is empty already" << endl;
            query_info.PrintOutputVar();
            return query_info;
        }
        while (query.find_first_not_of(' ') != string::npos && !query.empty()) {
            TrimLeadingWhitespaces(&query);
            string suchThatWithOneSpacing = query.substr(0, 9);
            // cout << suchThatWithOneSpacing << endl;
            // SUCH OR PATTERN
            string current_clause = DeleteOneWordAndRetrieveIt(&query);
            //cout << "curr clause : " << current_clause << endl;
            if (current_clause.compare("such") != 0 && current_clause.compare("pattern") != 0) {
                throw ("Error : cannot find 'such that' or 'pattern' clause");
            }

            if (current_clause.compare("such") == 0) {
                if (suchThatWithOneSpacing.compare("such that") != 0) {
                    throw ("Error : Invalid 'such that' clause");
                }
                string that_word = DeleteOneWordAndRetrieveIt(&query);
                STRING_STRINGLIST_MAP suchThatClauseResult;
                if (that_word.compare("that") == 0) {
                    //cout << "such that in query is now : " << query << endl;
                    string such_that_clause = query.substr(0, query.find_first_of(")") + 1);
                    query.erase(0, query.find_first_of(")") + 1);
                    suchThatClauseResult = query_syntax_validator->ValidateSuchThatClause(such_that_clause, PQL_parser_storage->GetAllUserDeclaredVar());

                    PQL_parser_storage->StoreSuchThatClauseResult(suchThatClauseResult);
                }
            }

            if (current_clause.compare("pattern") == 0) {
                STRING_STRINGLIST_MAP patternClauseResult;
                string pattern_var_name = DeleteByOpenBracketAndRetrieveIt(&query);
                query_syntax_validator->ValidateVariableIsAssign(pattern_var_name, PQL_parser_storage->GetVarMap());
                //cout << "pattern var name : " << pattern_var_name << endl;
                //cout << "QUERY PATTERN : " << query << endl;
                patternClauseResult = ParsePatternClause(&query, PQL_parser_storage->GetAllUserDeclaredVar(), query_syntax_validator);

                PQL_parser_storage->StorePatternClauseResult(patternClauseResult, pattern_var_name);
            }
        }
        // such OR pattern
        //string current_clause = deleteOneWordAndRetrieveIt(&query);
        query_info.SetRelRefMap(PQL_parser_storage->GetRelRefMap());
        query_info.SetVarMap(PQL_parser_storage->GetVarMap());
    } 
    catch (const char* msg) {
        //cerr << msg << endl;
        query_info.SetValidToFalse();
    }
    /*
    query_info.PrintOutputVar();
    query_info.PrintRelRefMap();
    query_info.PrintVarMap();
    */
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

string PQLParser::DeleteOneWordAndRetrieveIt(string* str) {
    TrimLeadingWhitespaces(str);
    string next_word = str->substr(0, str->find_first_of(" "));
    str->erase(0, str->find_first_of(" "));
    TrimLeadingWhitespaces(str);

    TrimLeadingWhitespaces(&next_word);
    TrimTrailingWhitespaces(&next_word);
    return next_word;
}

string PQLParser::DeleteByOpenBracketAndRetrieveIt(string* str) {
    TrimLeadingWhitespaces(str);
    string next_word = str->substr(0, str->find_first_of("("));
    str->erase(0, str->find_first_of("("));
    TrimLeadingWhitespaces(str);

    TrimLeadingWhitespaces(&next_word);
    TrimTrailingWhitespaces(&next_word);
    return next_word;
}

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