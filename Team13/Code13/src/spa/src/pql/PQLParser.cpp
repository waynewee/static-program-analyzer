#include <string>
using namespace std;
#include "PQLParser.h"
#include <vector>
#include <unordered_set>
#include <sstream>
#include <iostream>
#include "QuerySyntaxValidator.h"
#include <regex>
#include "QueryInfo.h"

QueryInfo PQLParser::parse(string s) {
    QueryInfo queryInfo;
    QuerySyntaxValidator query_syntax_validator;
    unordered_map<string, string> varMap;
    unordered_map<string, vector<vector<string>>> relRefMap;

    string pattern_var_name;

    vector<string> temp_storage;
    string str = s;

    unordered_map<string, int> differentiable_user_declared_var;

    unordered_set<string> all_user_declared_var;
    string delimiter = ";";
    size_t pos = 0;
    string token;
    try {
        if ((pos = str.find(delimiter)) == string::npos) {
            throw "Error : Invalid format, ';' not found";
        }

        while ((pos = str.find(delimiter)) != string::npos) {
            token = str.substr(0, pos);
            temp_storage.push_back(token);
            str.erase(0, pos + delimiter.length());
        }

        str = trimLeadingWhitespaces(str);
        // remaining str is the select portion
        // cout << "SELECT CLAUSE : " << str << endl;

        query_syntax_validator.validateSelectClauseStartsWithSelect(str);

        // temp storage has all the declarations

        for (auto decl : temp_storage) {
            // cout << "decl: " << decl << endl;
            decl = trimLeadingWhitespaces(decl);
            string entType = decl.substr(0, decl.find_first_of(" "));

            // cout << "decl_type : " << entType << endl;

            unordered_map<string, int> vars_declared;
            vars_declared = query_syntax_validator.validateDeclaration(decl);
            for (pair<string, int> var : vars_declared) {
                query_syntax_validator.validateVariableName(var.first);
                differentiable_user_declared_var.insert(var);
                all_user_declared_var.insert(var.first);
                varMap[var.first] = entType;
            }
        }


        string select_clause = str;
        // cout << "select_clause : " << select_clause << endl;

        std::istringstream iss(select_clause);

        // get next word, remove current
        select_clause = trimLeadingWhitespaces(select_clause);
        select_clause.erase(0, select_clause.find_first_of(" "));
        select_clause = trimLeadingWhitespaces(select_clause);

        string next_word = select_clause.substr(0, select_clause.find_first_of(" "));
        next_word = trimLeadingWhitespaces(next_word);
        next_word = trimTrailingWhitespaces(next_word);

        // cout << "variable user wants to select : " << next_word << endl;

        // now, next word MUST BE a variable declared by the user
        // if he didnt declare
        if (!query_syntax_validator.validateVariableExists(next_word, all_user_declared_var)) {
            throw "Error : variable after 'Select' is undeclared";
        }

        queryInfo.setOutputVar(next_word);

        select_clause = trimLeadingWhitespaces(select_clause);
        select_clause = trimTrailingWhitespaces(select_clause);


        // cout << "IS SELECT CLAUSE EMPTY : " << select_clause.empty() << endl;
        // cout << "SELECT CLAUSE IS NOW : " << select_clause << endl;

        // didnt declare clause
        /*
        if (next_word.compare(select_clause) == 0) {
            throw ("Error : 'Select' not found");
        }
        */

        select_clause.erase(0, select_clause.find_first_of(" "));
        select_clause = trimLeadingWhitespaces(select_clause);
        // cout << "IS SELECT CLAUSE EMPTY : " << select_clause.empty() << endl;
        // cout << "SELECT CLAUSE IS NOW : " << select_clause << endl;

        // now, next word MUST BE either such OR pattern
        next_word = select_clause.substr(0, select_clause.find_first_of(" "));
        next_word = trimLeadingWhitespaces(next_word);
        next_word = trimTrailingWhitespaces(next_word);

        if (select_clause.empty()) {
            return queryInfo;
        }

        // cout << "next_word : " << next_word << endl;

        // if NEITHER such NOR pattern is found
        if (next_word.compare("such") != 0 && next_word.compare("pattern") != 0) {
            throw ("Error : cannot find 'such that' or 'pattern' clause");
        }

        // NOW next word can be such, pattern or and
        while (select_clause.find_first_not_of(' ') != string::npos && !select_clause.empty())
        {
            // String still not empty, continue
            select_clause = trimLeadingWhitespaces(select_clause);

            // now, next word MUST BE either such OR pattern
            next_word = select_clause.substr(0, select_clause.find_first_of(" "));
            next_word = trimLeadingWhitespaces(next_word);
            next_word = trimTrailingWhitespaces(next_word);

            // cout << "next_word in while loop : " << next_word << endl;

            // if NEITHER such NOR pattern NOR and is found
            if (next_word.compare("such") != 0 && next_word.compare("pattern") != 0 && next_word.compare("and")) {
                throw ("Error : cannot find 'such that' or 'pattern' clause");
            }

            if (next_word.compare("such") == 0) {
                // next word is such
                // the next word now should be 'that', if not error
                // MUST only have one spacing between such that, e.g. such   that -> NOT ALLOWED
                select_clause.erase(0, select_clause.find_first_of(" ") + 1);
                // select_clause = trimLeadingWhitespaces(select_clause);


                next_word = select_clause.substr(0, select_clause.find_first_of(" "));
                next_word = trimLeadingWhitespaces(next_word);
                next_word = trimTrailingWhitespaces(next_word);

                unordered_map<string, vector<string>> suchThatClauseResult;

                // cout << "next_word : " << next_word << endl;
                if (next_word.compare("that") == 0) {
                    // we found such that clause
                    // MUST only have one spacing between such that, e.g. such   that -> NOT ALLOWED
                    // select_clause = trimLeadingWhitespaces(select_clause);
                    select_clause.erase(0, select_clause.find_first_of(" "));
                    select_clause = trimLeadingWhitespaces(select_clause);

                    string such_that_clause = select_clause.substr(0, select_clause.find_first_of(")") + 1);
                    select_clause.erase(0, select_clause.find_first_of(")") + 1);
                    // cout << "such_that_clause : " << such_that_clause << endl;

                    suchThatClauseResult = query_syntax_validator.validateSuchThatClause(such_that_clause, differentiable_user_declared_var);

                    // cout << "select clause now left with : " << select_clause << endl;
                }

                for (auto const& pair : suchThatClauseResult) {
                    // already inside
                    if (relRefMap.count(pair.first) == 1) {
                        vector<vector<string>> empty;
                        relRefMap.at(pair.first).push_back(pair.second);
                    }
                    else {
                        vector<vector<string>> empty;
                        relRefMap[pair.first] = empty;
                        // since second is a vector
                        relRefMap.at(pair.first).push_back(pair.second);
                    }
                }
            }

            if (next_word.compare("pattern") == 0) {
                unordered_map<string, vector<string>> patternClauseResult;
                // next word is var name
                // the next word now should be user declared var, if not error
                select_clause.erase(0, select_clause.find_first_of(" "));
                select_clause = trimLeadingWhitespaces(select_clause);

                next_word = select_clause.substr(0, select_clause.find_first_of(" "));
                next_word = trimLeadingWhitespaces(next_word);
                next_word = trimTrailingWhitespaces(next_word);
                // cout << "varname in pattern : " << next_word << endl;

                pattern_var_name = next_word;

                if (!query_syntax_validator.validateVariableExists(next_word, all_user_declared_var)) {
                    throw "Error : Variable after pattern is undeclared!";
                }

                next_word = select_clause.substr(0, select_clause.find_first_of(" "));
                next_word = trimLeadingWhitespaces(next_word);
                next_word = trimTrailingWhitespaces(next_word);

                select_clause = trimLeadingWhitespaces(select_clause);
                select_clause.erase(0, select_clause.find_first_of(" "));

                if (select_clause.find("\"") != string::npos && select_clause.find("\"") < select_clause.find_first_of(")")) {
                    int endOfPatternClause = query_syntax_validator.nthOccurrence(select_clause, "\"", 2);

                    string temp = select_clause.substr(endOfPatternClause, select_clause.length());

                    int distanceToClosedBracket = temp.find_first_of(")") + 1;

                    string pattern_clause = select_clause.substr(0, endOfPatternClause + distanceToClosedBracket);

                    // cout << "pattern clause : " << pattern_clause << endl;

                    select_clause.erase(0, endOfPatternClause + distanceToClosedBracket);

                    patternClauseResult = query_syntax_validator.validatePatternClause(pattern_clause, differentiable_user_declared_var);
                }
                else {
                    // cannot find ", so pattern variables are blank. we can find by next closing bracket
                    int endOfPatternClause = select_clause.find_first_of(")") + 1;

                    string pattern_clause = select_clause.substr(0, endOfPatternClause);

                    // cout << "pattern clause : " << pattern_clause << endl;

                    select_clause.erase(0, endOfPatternClause);

                    patternClauseResult = query_syntax_validator.validatePatternClause(pattern_clause, differentiable_user_declared_var);
                }


                // cout << "select clause now left with : " << select_clause << endl;

                for (auto const& pair : patternClauseResult) {
                    // already inside
                    vector<string> p = pair.second;
                    p.push_back(pattern_var_name);
                    for (auto i : p) {
                        //cout << i << " | ";
                        //cout << endl;
                    }
                    if (relRefMap.count(pair.first) == 1) {
                        vector<vector<string>> empty;
                        relRefMap.at(pair.first).push_back(p);
                    }
                    else {
                        vector<vector<string>> empty;
                        relRefMap[pair.first] = empty;
                        // since second is a vector
                        relRefMap.at(pair.first).push_back(p);
                    }
                }

            }
        }
        // PRINTING HERE  
        queryInfo.setRelRefMap(relRefMap);
        queryInfo.setVarMap(varMap);
    }
    catch (const char* msg) {
        cerr << msg << endl;
        queryInfo.setValidToFalse();
    }
    return queryInfo;
}

string PQLParser::trimLeadingWhitespaces(const string& s) {
    return regex_replace(s, regex("^\\s+"), string(""));
}

string PQLParser::trimTrailingWhitespaces(const string& s) {
    return std::regex_replace(s, regex("\\s+$"), string(""));
}
