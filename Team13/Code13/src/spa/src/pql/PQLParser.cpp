#include <string>
using namespace std;
#include "PQLParser.h"
#include <vector>
#include <unordered_set>
#include <sstream>
#include <iostream>
#include "QuerySyntaxValidator.h"
#include <regex>

void PQLParser::parse(string s) {
    QuerySyntaxValidator query_syntax_validator;

	vector<string> temp_storage;
    string str = s;

    unordered_set<string> all_user_declared_var;

	string delimiter = ";";
    size_t pos = 0;
    string token;

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
    cout << "SELECT CLAUSE : " << str << endl;
    
    query_syntax_validator.validateSelectClauseStartsWithSelect(str);

    // temp storage has all the declarations

    for (auto decl : temp_storage) {
        cout << "decl: " << decl << endl;
        vector<string> vars_declared;
        vars_declared = query_syntax_validator.validateDeclaration(decl);
        for (auto var : vars_declared) {
            all_user_declared_var.insert(var);
        }
    }

    string select_clause = str;
    cout << "select_clause : " << select_clause << endl;

    std::istringstream iss(select_clause);

    // get next word, remove current
    select_clause = trimLeadingWhitespaces(select_clause);
    select_clause.erase(0, select_clause.find_first_of(" "));
    select_clause = trimLeadingWhitespaces(select_clause);

    string next_word = select_clause.substr(0, select_clause.find_first_of(" "));
    next_word = trimLeadingWhitespaces(next_word);
    next_word = trimTrailingWhitespaces(next_word);

    cout << "variable user wants to select : " << next_word << endl;

    // now, next word MUST BE a variable declared by the user
    // if he didnt declare
    if (!query_syntax_validator.validateVariableExists(next_word, all_user_declared_var)) {
        throw "Error : variable after 'Select' is undeclared";
    }

    select_clause = trimLeadingWhitespaces(select_clause);
    select_clause = trimTrailingWhitespaces(select_clause);

    // didnt declare clause
    if (next_word.compare(select_clause) == 0) {
        return;
    }

    select_clause.erase(0, select_clause.find_first_of(" "));
    select_clause = trimLeadingWhitespaces(select_clause);




    // now, next word MUST BE either such OR pattern
    next_word = select_clause.substr(0, select_clause.find_first_of(" "));
    next_word = trimLeadingWhitespaces(next_word);
    next_word = trimTrailingWhitespaces(next_word);

    // cout << "next_word : " << next_word << endl;

    // if NEITHER such NOR pattern is found
    if (next_word.compare("such") != 0 && next_word.compare("pattern") != 0) {
        throw ("Error : cannot find 'such that' or 'pattern' clause");
    }

    if (next_word.compare("such") == 0) {
        // next word is such
        // the next word now should be 'that', if not error
        select_clause.erase(0, select_clause.find_first_of(" "));
        select_clause = trimLeadingWhitespaces(select_clause);

        next_word = select_clause.substr(0, select_clause.find_first_of(" "));
        next_word = trimLeadingWhitespaces(next_word);
        next_word = trimTrailingWhitespaces(next_word);

        // cout << "next_word : " << next_word << endl;
        if (next_word.compare("that") == 0) {
            // we found such that clause
            select_clause.erase(0, select_clause.find_first_of(" "));
            select_clause = trimLeadingWhitespaces(select_clause);

            string such_that_clause = select_clause.substr(0, select_clause.find_first_of(")") + 1);
            select_clause.erase(0, select_clause.find_first_of(")") + 1);
            cout << "such_that_clause : " << such_that_clause << endl;

            query_syntax_validator.validateSuchThatClause(such_that_clause, all_user_declared_var);

            // cout << "select clause now left with : " << select_clause << endl;
        }
    }

    if (next_word.compare("pattern") == 0) {
        // next word is var name
        // the next word now should be user declared var, if not error
        select_clause.erase(0, select_clause.find_first_of(" "));
        select_clause = trimLeadingWhitespaces(select_clause);

        next_word = select_clause.substr(0, select_clause.find_first_of(" "));
        next_word = trimLeadingWhitespaces(next_word);
        next_word = trimTrailingWhitespaces(next_word);
        cout << "varname in pattern : " << next_word << endl;

        if (!query_syntax_validator.validateVariableExists(next_word, all_user_declared_var)) {
            throw "Error : Variable after pattern is undeclared!" ;
        }

        next_word = select_clause.substr(0, select_clause.find_first_of(" "));
        next_word = trimLeadingWhitespaces(next_word);
        next_word = trimTrailingWhitespaces(next_word);

        select_clause = trimLeadingWhitespaces(select_clause);
        select_clause.erase(0, select_clause.find_first_of(" "));
        string pattern_clause = select_clause.substr(0, select_clause.find_first_of(")") + 1);
        pattern_clause = trimLeadingWhitespaces(pattern_clause);

        cout << "pattern clause : " << pattern_clause << endl;

        select_clause.erase(0, select_clause.find_first_of(")") + 1);

        query_syntax_validator.validatePatternClause(pattern_clause, all_user_declared_var);

        // cout << "select clause now left with : " << select_clause << endl;
    
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

        cout << "next_word in while loop : " << next_word << endl;

        // if NEITHER such NOR pattern NOR and is found
        if (next_word.compare("such") != 0 && next_word.compare("pattern") != 0 && next_word.compare("and")) {
            throw ("Error : cannot find 'such that' or 'pattern' clause");
        }

        if (next_word.compare("and") == 0) {
            // skip one word
            select_clause = trimLeadingWhitespaces(select_clause);
            select_clause.erase(0, select_clause.find_first_of(" "));
            select_clause = trimLeadingWhitespaces(select_clause);

            // now, next word MUST BE either such OR pattern
            next_word = select_clause.substr(0, select_clause.find_first_of(" "));
            next_word = trimLeadingWhitespaces(next_word);
            next_word = trimTrailingWhitespaces(next_word);
        }

        if (next_word.compare("such") == 0) {
            // next word is such
            // the next word now should be 'that', if not error
            select_clause.erase(0, select_clause.find_first_of(" "));
            select_clause = trimLeadingWhitespaces(select_clause);

            next_word = select_clause.substr(0, select_clause.find_first_of(" "));
            next_word = trimLeadingWhitespaces(next_word);
            next_word = trimTrailingWhitespaces(next_word);

            // cout << "next_word : " << next_word << endl;
            if (next_word.compare("that") == 0) {
                // we found such that clause
                select_clause.erase(0, select_clause.find_first_of(" "));
                select_clause = trimLeadingWhitespaces(select_clause);

                string such_that_clause = select_clause.substr(0, select_clause.find_first_of(")") + 1);
                select_clause.erase(0, select_clause.find_first_of(")") + 1);
                cout << "such_that_clause : " << such_that_clause << endl;

                query_syntax_validator.validateSuchThatClause(such_that_clause, all_user_declared_var);

                // cout << "select clause now left with : " << select_clause << endl;
            }
        }

        if (next_word.compare("pattern") == 0) {
            // next word is var name
            // the next word now should be user declared var, if not error
            select_clause.erase(0, select_clause.find_first_of(" "));
            select_clause = trimLeadingWhitespaces(select_clause);

            next_word = select_clause.substr(0, select_clause.find_first_of(" "));
            next_word = trimLeadingWhitespaces(next_word);
            next_word = trimTrailingWhitespaces(next_word);
            cout << "varname in pattern : " << next_word << endl;

            if (!query_syntax_validator.validateVariableExists(next_word, all_user_declared_var)) {
                throw "Error : Variable after pattern is undeclared!";
            }

            next_word = select_clause.substr(0, select_clause.find_first_of(" "));
            next_word = trimLeadingWhitespaces(next_word);
            next_word = trimTrailingWhitespaces(next_word);

            select_clause = trimLeadingWhitespaces(select_clause);
            select_clause.erase(0, select_clause.find_first_of(" "));
            string pattern_clause = select_clause.substr(0, select_clause.find_first_of(")") + 1);
            pattern_clause = trimLeadingWhitespaces(pattern_clause);

            cout << "pattern clause : " << pattern_clause << endl;

            select_clause.erase(0, select_clause.find_first_of(")") + 1);

            query_syntax_validator.validatePatternClause(pattern_clause, all_user_declared_var);

            // cout << "select clause now left with : " << select_clause << endl;

        }
    }
}


string PQLParser::trimLeadingWhitespaces(const string& s) {
    return regex_replace(s, regex("^\\s+"), string(""));
}

string PQLParser::trimTrailingWhitespaces(const string& s) {
    return std::regex_replace(s, regex("\\s+$"), string(""));
}
