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

QueryInfo PQLParser::parse(string s) {
	QueryInfo query_info;
	QuerySyntaxValidator* query_syntax_validator = new QuerySyntaxValidator();
	PQLParserStorage* PQL_parser_storage = new PQLParserStorage();
    string query = s;

    try {
        // get all the user declarations
        vector<string> all_declarations = splitBySemicolons(&query);
        PQL_parser_storage->setAllDeclarations(all_declarations);
        
        trimLeadingWhitespaces(&query);

        for (auto decl : all_declarations) {
            trimLeadingWhitespaces(&decl);
            string entType = decl.substr(0, decl.find_first_of(" "));

            unordered_map<string, string> vars_declared = query_syntax_validator->validateDeclaration(decl);

            for (pair<string, string> var : vars_declared) {
                query_syntax_validator->validateVariableName(var.first);
                //cout << "1st : " << var.first << "2nd : " << entType << endl;
                PQL_parser_storage->storeVariable(var, entType);
            }
        }

        query_syntax_validator->validateSelectClauseStartsWithSelect(query);
        // LEFT WITH select clause
        //cout << "query is now :" << query << endl;

        deleteOneWordAndRetrieveIt(&query);
        //cout << "query is now :" << query << endl;

        string user_select_var = deleteOneWordAndRetrieveIt(&query);
        cout << "user_select var is :" << user_select_var << endl;
        cout << "query is now :" << query << endl;

        query_syntax_validator->validateVariableExists(user_select_var, PQL_parser_storage->getAllUserDeclaredVar());

        query_info.setOutputVar(user_select_var);

        trimLeadingWhitespaces(&query);

        if (query.empty()) {
            //cout << "query is empty already" << endl;
            query_info.printOutputVar();
            return query_info;
        }
        while (query.find_first_not_of(' ') != string::npos && !query.empty()) {
            trimLeadingWhitespaces(&query);
            string suchThatWithOneSpacing = query.substr(0, 9);
            // cout << suchThatWithOneSpacing << endl;
            // SUCH OR PATTERN
            string current_clause = deleteOneWordAndRetrieveIt(&query);
            cout << "curr clause : " << current_clause << endl;
            if (current_clause.compare("such") != 0 && current_clause.compare("pattern") != 0) {
                throw ("Error : cannot find 'such that' or 'pattern' clause");
            }

            if (current_clause.compare("such") == 0) {
                if (suchThatWithOneSpacing.compare("such that") != 0) {
                    throw ("Error : Invalid 'such that' clause");
                }
                string that_word = deleteOneWordAndRetrieveIt(&query);
                unordered_map<string, vector<string>> suchThatClauseResult;
                if (that_word.compare("that") == 0) {
                    cout << "such that in query is now : " << query << endl;
                    string such_that_clause = query.substr(0, query.find_first_of(")") + 1);
                    query.erase(0, query.find_first_of(")") + 1);
                    suchThatClauseResult = query_syntax_validator->validateSuchThatClause(such_that_clause, PQL_parser_storage->getAllUserDeclaredVar());

                    PQL_parser_storage->storeSuchThatClauseResult(suchThatClauseResult);
                }
            }

            if (current_clause.compare("pattern") == 0) {
                unordered_map<string, vector<string>> patternClauseResult;
                string pattern_var_name = deleteOneWordAndRetrieveIt(&query);
                cout << "pattern var name : " << pattern_var_name << endl;
                cout << "QUERY PATTERN : " << query << endl;
                patternClauseResult = parsePatternClause(&query, PQL_parser_storage->getAllUserDeclaredVar(), query_syntax_validator);

                PQL_parser_storage->storePatternClauseResult(patternClauseResult, pattern_var_name);
            }
        }
        // such OR pattern
        //string current_clause = deleteOneWordAndRetrieveIt(&query);
        query_info.setRelRefMap(PQL_parser_storage->getRelRefMap());
        query_info.setVarMap(PQL_parser_storage->getVarMap());
    } 
    catch (const char* msg) {
        cerr << msg << endl;
        query_info.setValidToFalse();
    }
    query_info.printOutputVar();
    query_info.printRelRefMap();
    query_info.printVarMap();
	return query_info;
}

void PQLParser::trimLeadingWhitespaces(string* s) {
    int last_leading_space = s->find_first_not_of(" ");
    s->erase(0, last_leading_space);
}

void PQLParser::trimTrailingWhitespaces(string* s) {
    int first_trailing_space = s->find_last_not_of(" ") + 1;
    s->erase(first_trailing_space, s->length());
}

vector<string> PQLParser::splitBySemicolons(string* query) {
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

string PQLParser::deleteOneWordAndRetrieveIt(string* str) {
    trimLeadingWhitespaces(str);
    string next_word = str->substr(0, str->find_first_of(" "));
    str->erase(0, str->find_first_of(" "));
    trimLeadingWhitespaces(str);

    trimLeadingWhitespaces(&next_word);
    trimTrailingWhitespaces(&next_word);
    return next_word;
}

unordered_map<string, vector<string>> PQLParser::parsePatternClause(string* clause, unordered_map<string, string> all_user_declared_var,
    QuerySyntaxValidator* query_syntax_validator) {
    unordered_map<string, vector<string>> patternClauseResult;
    if (clause->find("\"") != string::npos && clause->find("\"") < clause->find_first_of(")")) {
        int endOfPatternClause = query_syntax_validator->nthOccurrence(clause, "\"", 2);

        string temp = clause->substr(endOfPatternClause, clause->length());

        int distanceToClosedBracket = temp.find_first_of(")") + 1;

        string pattern_clause = clause->substr(0, endOfPatternClause + distanceToClosedBracket);

        // cout << "pattern clause : " << pattern_clause << endl;

        clause->erase(0, endOfPatternClause + distanceToClosedBracket);

        patternClauseResult = query_syntax_validator->validatePatternClause(pattern_clause, all_user_declared_var);
    }
    else {
        // cannot find ", so pattern variables are blank. we can find by next closing bracket
        int endOfPatternClause = clause->find_first_of(")") + 1;

        string pattern_clause = clause->substr(0, endOfPatternClause);

        // cout << "pattern clause : " << pattern_clause << endl;

        clause->erase(0, endOfPatternClause);

        patternClauseResult = query_syntax_validator->validatePatternClause(pattern_clause, all_user_declared_var);
    }
    
    return patternClauseResult;
}