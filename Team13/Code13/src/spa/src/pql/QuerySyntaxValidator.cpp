#include "QuerySyntaxValidator.h"
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <regex>
#include <sstream>
#include <iostream>
using namespace std;

const unordered_set<string> validDesignEntities = { "stmt" , "read" , "print" , "call" , "while" , "if" , "assign" , "variable" , "constant" , "procedure" };
const unordered_set<string> validRelrefNames = { "Follows" , "FollowsT" , "Follows*" , "Parent" , "ParentT" , "Parent*" , "Uses" , "UsesS" , "UsesP" , "Modifies" ,  "ModifiesS" , "ModifiesP" };

/*
	Returns a vector of the variables user declared.
*/
vector<string> QuerySyntaxValidator::validateDeclaration(string decl) {
	// this one validates the declaration
	// ie. assign a, b -> true, assijn a, b -> false, stmt s -> true
	/*
		1. Split by ","
		2. Split by " "
		3. Check first word, if it exists in valid design entities
	*/

	string decl_clone = decl;
	vector<string> temp_results;
	unordered_set<string> variable_names_declared;

	vector<string> final_results;

	string comma = ",";
	size_t pos = 0;
	string token;

	decl_clone = trimLeadingWhitespaces(decl_clone);
	decl_clone = trimTrailingWhitespaces(decl_clone);

	// this while loop is splitting by comma.
	// assign a, b, c -> [assign a] [b] [c]
	while ((pos = decl_clone.find(comma)) != string::npos) {
		token = decl_clone.substr(0, pos);
		token = trimLeadingWhitespaces(token);
		token = trimTrailingWhitespaces(token);
		temp_results.push_back(token);

		decl_clone.erase(0, pos + comma.length());
		// cout << "clone b : " << temp_clone << endl;
		decl_clone = trimLeadingWhitespaces(decl_clone);
		decl_clone = trimTrailingWhitespaces(decl_clone);
		// cout << "clone a : " << temp_clone << endl;

	}

	temp_results.push_back(decl_clone);

	string variable_with_entity = temp_results.front(); // variable with entity is the first set delimited by comma, assign a, b, c -> [assign a]
	temp_results.erase(temp_results.begin());

	for (auto var_name : temp_results) {
		validateVariableName(var_name);
		if (variable_names_declared.count(var_name) == 1) {
			throw ("Error : Declared same variable name twice ");
		}
		// cout << "Does it continue? ";
		// cout << "Insert variable name to set : " << var_name << endl;
		variable_names_declared.insert(var_name); // inserts the variables into a set to later check if the user repeated the values
		final_results.push_back(var_name);
	}

	vector<std::string> first_stmt; // first_stmt is everything before the first comma.
	std::istringstream iss(variable_with_entity);
	string entity_type = variable_with_entity.substr(0, variable_with_entity.find_first_of(" "));

	cout << "Entity Type : " << entity_type << endl;
	
	iss >> variable_with_entity;
	for (string variable_with_entity; iss >> variable_with_entity;) {
		first_stmt.push_back(variable_with_entity);	
		if (variable_names_declared.count(first_stmt.back()) == 1) {
			throw ("Error : Declared same variable name twice ");
		}
		// cout << "Insert variable name to set : " << first_stmt.back() << endl;
		variable_names_declared.insert(first_stmt.back());
		final_results.push_back(first_stmt.back());
	}
	

	if (first_stmt.size() !=  1) {
		throw ("Error : Invalid Design Entity Declaration!");
	}

	// checks if first word is a valid design entity
	if (validDesignEntities.count(entity_type) != 1)  {
		throw ("Error : Invalid Design Entity Type declared!");
	}

	for (auto i : final_results) {
		cout << "User declared variable : " << i << endl;
	}
	return final_results;
}

void QuerySyntaxValidator::validateSelectClauseStartsWithSelect(string s) {
	// just check if first word is select in select clause.
	/*
		Select s such that Follows (s1, s2); -> true, first word is Select
		such that s such that Follows (s1, s4); -> nope, throw error
	*/
	vector<std::string> first_stmt;
	string select_clause = s;
	std::istringstream iss(select_clause);
	iss >> select_clause;
	first_stmt.push_back(select_clause);
	if (first_stmt.front().compare("Select") != 0) {
		throw ("Error : Select clause does not start with exactly 'Select'! ");
	}
}

bool QuerySyntaxValidator::validateVariableName(string s) {
	// validates if a variable name follows the rules of variables in SIMPLE
	/*
		Basically NAME: LETTER (LETTER | DIGIT)*
		a variable name must start with letter, then can be followed by either letter or digit.
		Can be just a Letter, i.e. J
		e.g. J2Lpm -> true
		24Sdf -> not valid variable name, throw error
	*/

	regex expr("[A-Za-z][a-zA-Z_0-9]*");

	if (!regex_match(s, expr)) {
		throw ("Error : Invalid variable name!");
	}
	return true;
}

bool QuerySyntaxValidator::validateVariableExists(string s, unordered_set<string> varNames) {
	/*
		Just need to check if string s is in the set of varNames. Very simple
	*/
	bool res = false;
	if (varNames.count(s) != 1) {
		// cant find
		res = false;
	}
	else {
		res = true;
	}
	return res;
}

unordered_map<string, vector<string>> QuerySyntaxValidator::validateSuchThatClause(string s, unordered_set<string> declaredVarNames) {
	// validates if what's after such that is legit
	/*
		So if : select s such that Follows (s1, s2);
		will pass in the two tokens after 'such that' is seen
		So in this case, [ Follows (s1, s2) ] is passed in as the argument
		and we check against the list of variable names in the program
		and we need to check against RelrefNames

		Select a such that Parent* (w, a) pattern a (“count”, _)
	*/
	string temp = s;
	vector<std::string> such_that_clause;

	string open_brackets_delimiter = "(";
	string comma_delimiter = ",";

	size_t pos_bracket = 0;
	size_t pos_comma = 0;

	string token;
	// open bracket is not found
	if ((pos_bracket = temp.find(open_brackets_delimiter)) == string::npos) {
		throw ("Error : Invalid Relation-Reference Format, open bracket not found! ");
	}
	string declared_relRef = temp.substr(0, pos_bracket);
	temp.erase(0, pos_bracket + open_brackets_delimiter.length()); // erase off front part -> remaining with s1, s2 );
	declared_relRef = trimTrailingWhitespaces(declared_relRef);
	
	if (validRelrefNames.count(declared_relRef) != 1) {
		throw ("Error : Invalid Relational-Reference name!");
	}

	// cout << "Declared Relref : " << declared_relRef << endl;

	// cout << temp << endl;

	temp = trimTrailingWhitespaces(temp); // if e.g. "Follows (s1, s2)" -> temp is now left with "s1, s2)"

	if (temp.back() != ')') {
		throw ("Error : Invalid Relation-Reference Format");
	}

	if ((pos_comma = temp.find(comma_delimiter)) == string::npos) {
		throw ("Error : Invalid Relation-Reference Format, comma not found! ");
	}

	string first_argument = temp.substr(0, pos_comma);
	
	first_argument = trimLeadingWhitespaces(first_argument);
	first_argument = trimTrailingWhitespaces(first_argument);

	cout << "such that clause : First argument : " << first_argument << endl;
	// validateVariableName(first_argument);

	temp.erase(0, pos_comma + comma_delimiter.length());
	temp = trimLeadingWhitespaces(temp);
	temp = trimTrailingWhitespaces(temp);
	
	string second_argument = temp.substr(0, temp.length() - 1);

	second_argument = trimLeadingWhitespaces(second_argument);
	second_argument = trimTrailingWhitespaces(second_argument);
	// validateVariableName(second_argument);

	cout << "such that clause : Second argument : " << second_argument << endl;

	unordered_map<string, vector<string>> relRef_map;
	string assigned_relRef;
	vector<string> arguments;

	if (declared_relRef.compare("Follows") == 0 || declared_relRef.compare("Follows*") == 0 || 
		declared_relRef.compare("Parent") == 0 || declared_relRef.compare("Parent*") == 0) {
		// If the relref is Follows or Parent, we just need to check if the argument is declared by the user
		// note that we don't need to check the validity here because it's already checked by the declaration validator
		if (declared_relRef.compare("Follows") == 0) {
			arguments.push_back(first_argument);
			arguments.push_back(second_argument);
			relRef_map["Follows"] = arguments;
		}
		if (declared_relRef.compare("Follows*") == 0) {
			arguments.push_back(first_argument);
			arguments.push_back(second_argument);
			assigned_relRef = "FollowsT";
			relRef_map["FollowsT"] = arguments;
		}
		if (declared_relRef.compare("Parent") == 0) {
			arguments.push_back(first_argument);
			arguments.push_back(second_argument);
			relRef_map["Parent"] = arguments;
		}
		if (declared_relRef.compare("Parent*") == 0) {
			arguments.push_back(first_argument);
			arguments.push_back(second_argument);
			assigned_relRef = "ParentT";
			relRef_map["ParentT"] = arguments;
		}
		if (declaredVarNames.count(first_argument) != 1 && !isInteger(first_argument)) { // first argument wasnt defined by the user
			throw ("Error : First argument is not declared or is invalid ");
		}
		if (declaredVarNames.count(second_argument) != 1 && !isInteger(second_argument)) { // second argument wasnt defined by the user
			throw ("Error : Second argument is not declared or is invalid ");
		}
	}

	/*  
		UsesS : ‘Uses’ ‘(’ stmtRef ‘,’ entRef ‘)’
		UsesP: ‘Uses’ ‘(’ entRef ‘, ’ entRef ‘)’
		ModifiesS : ‘Modifies’ ‘(’ stmtRef ‘, ’ entRef ‘)’
		ModifiesP : ‘Modifies’ ‘(’ entRef ‘, ’ entRef ‘)’
	*/

	// function name is Uses
	if (declared_relRef.compare("Uses") == 0) {
		// first argument is an entRef
		// cout << "what is first arg : " << first_argument;
		if (isInteger(first_argument) || declaredVarNames.count(first_argument) == 1) {
			assigned_relRef = "UsesS";
		}
		else {
			assigned_relRef = "UsesP";
		}
	}

	// function name is Modifies
	if (declared_relRef.compare("Modifies") == 0) {
		// first argument is an entRef
		if (isInteger(first_argument) || declaredVarNames.count(first_argument) == 1) {
			assigned_relRef = "ModifiesS";
		}
		else {
			assigned_relRef = "ModifiesP";
		}
	}

	cout << "declared_relRef : " << declared_relRef << endl;
	cout << "assigned_relRef : " << assigned_relRef << endl;


	// regardless, first argument cannot be '_'
	if (assigned_relRef.compare("UsesS") == 0 || assigned_relRef.compare("ModifiesS") == 0) {
		// first argument is a statement ref
		if (declaredVarNames.count(first_argument) != 1 && !isInteger(first_argument)) { 
			// first argument wasnt defined by the user or is just invalid
			throw ("Error : First argument is not declared or is invalid ");
		}
		// second argument is an entity ref
		// because entity ref always comes in the form ""
		string parsed_second_argument = second_argument.substr(1, second_argument.length() - 2);
		//cout << "2nd arg : " << second_argument << endl;

		// cout << "123123 second arg : " << second_argument << endl;
		if (declaredVarNames.count(second_argument) != 1 || !isEntRef(second_argument)) {
			// second argument wasnt defined user or is just invalid
			throw ("Error : Second argument is not declared or is invalid");
		}

		if (assigned_relRef.compare("UsesS") == 0) {
			arguments.push_back(first_argument);
			arguments.push_back(parsed_second_argument);
			relRef_map["UsesS"] = arguments;
		}

		if (assigned_relRef.compare("ModifiesS") == 0) {
			arguments.push_back(first_argument);
			arguments.push_back(parsed_second_argument);
			relRef_map["ModifiesS"] = arguments;
		}
	}


	if (assigned_relRef.compare("UsesP") == 0 || assigned_relRef.compare("ModifiesP") == 0) {
		// first argument is a entity ref
		// string parsed_first_argument = first_argument.substr(1, first_argument.length() - 2);
		//cout << "such that clause :  1st arg : " << parsed_first_argument << endl;

		if (declaredVarNames.count(first_argument) != 1 && !isEntRef(first_argument)) {
			// second argument wasnt defined by the SIMPLE program at frontend or is just invalid
			throw ("Error : First argument is not declared or is invalid ");
		}
		// second argument is an entity ref
		// string parsed_second_argument = second_argument.substr(1, second_argument.length() - 2);
		// cout << "such that clause :  2nd arg : " << parsed_second_argument << endl;

		if (declaredVarNames.count(second_argument) != 1 && !isEntRef(second_argument)) {
			// second argument wasnt defined by the SIMPLE program at frontend or is just invalid
			throw ("Error : Second argument is not declared or is invalid ");
		}

		if (assigned_relRef.compare("UsesP") == 0) {
			arguments.push_back(first_argument);
			arguments.push_back(second_argument);
			relRef_map["UsesP"] = arguments;
		}

		if (assigned_relRef.compare("ModifiesP") == 0) {
			arguments.push_back(first_argument);
			arguments.push_back(second_argument);
			relRef_map["ModifiesP"] = arguments;
		}
	}

	return relRef_map;
}

void QuerySyntaxValidator::validatePatternClause(string s, unordered_set<string> declaredVarNames) {
	// validates what after pattern clause is legit
	/*
		Select a pattern a ( _ , _“v + x * y + z * t”_)
		Pass in this portion "( _ , _“v + x * y + z * t”_)"
		I think we dont need to care about the arguments passed
	*/

	string str = s;
	string comma = ",";
	int pos_comma;

	str = trimLeadingWhitespaces(str);
	str = trimTrailingWhitespaces(str);

	if (str.front() != '(') {
		throw ("Error : Invalid pattern expression");
	}

	if (str.back() != ')') {
		throw ("Error : Invalid Relation-Reference Format");
	}

	// erase the first bracket away
	str.erase(0, 1);

	if ((pos_comma = str.find(comma)) == string::npos) {
		throw ("Error : Invalid Pattern Format, comma not found! ");
	}

	str = trimLeadingWhitespaces(str);
	pos_comma = str.find(comma);
	string first_argument = str.substr(0, pos_comma);
	first_argument = trimLeadingWhitespaces(first_argument);
	first_argument = trimTrailingWhitespaces(first_argument);

	cout << "pattern clause first arg : " << first_argument << endl;
	str.erase(0, pos_comma + comma.length());

	str = trimLeadingWhitespaces(str);

	string second_argument = str.substr(0, str.length() - 1);
	second_argument = trimLeadingWhitespaces(second_argument);
	second_argument = trimTrailingWhitespaces(second_argument);


	cout << "pattern second arg : " << second_argument << endl;

	// first arg in pattern can be in ""
	/*
	if (!validateVariableExists(first_argument, declaredVarNames)) {
		if (!isUnderscore(first_argument)) {
			if (!isEntRef(first_argument)) {
				throw ("Error : Invalid pattern expression");
			}
		}
	}
	*/

	if (!isEntRef(first_argument)) {
		cout << "first argument " << first_argument << "is not an entity ref" << endl;
		if (!isUnderscore(first_argument)) {
			if (!validateVariableName(first_argument)) {
				throw ("Error : Invalid pattern expression");
			}
		}
	}

	if(!isUnderscore(second_argument)) {
		if (second_argument.front() == '_') {
			if (second_argument.back() == '_') {
				// value looks like _"x + y"_
				second_argument.erase(0, 1);
				second_argument.erase(second_argument.length() - 1, second_argument.length());
				// cout << "2nd arg test: " << second_argument;
			}
			else {
				throw ("Error : Invalid pattern expression, for 2nd argument");
			}
		}

		// cout << "2nd arg test: " << second_argument;
		// 2nd argument will be in the form "x + y" no matter what
		// erase the "" again
		// now is just in the form x + y
		second_argument.erase(0, 1);
		second_argument.erase(second_argument.length() - 1, second_argument.length());
		// cout << "2nd arg test: " << second_argument;
		validateExpression(second_argument);
	}

}

void QuerySyntaxValidator::validateExpression(const string& s) {
	string str = s;
	if (s.length() <= 0) {
		throw ("Error : Invalid pattern expression ");
	}

	str = trimLeadingWhitespaces(str);
	str = trimTrailingWhitespaces(str);

	int open_bracket_count = 0;
	bool prev_was_operator = false;
	bool prev_was_open_bracket = false;

	if (isOperator(str.front()) || isOperator(str.back())) {
		throw ("Error : Invalid pattern expression");
	}


	for (char const& c : s) {
		if (c == ' ') {
			continue;
		}
		if (c == '(') {
			open_bracket_count++;
			prev_was_open_bracket = true;
			continue;
		}
		else if (c == ')') {
			if (open_bracket_count <= 0 || prev_was_operator) {
				throw ("Error : Invalid pattern expression");
			}
			open_bracket_count--;
		} 
		else if (isOperator(c)) {
			if (prev_was_operator || prev_was_open_bracket) {
				throw ("Error : Invalid pattern expression");
			}
			prev_was_operator = true;
			continue;
		}
		else if (!isalpha(c) && !isdigit(c)) {
			throw ("Error : Invalid pattern expression");
		}
		prev_was_operator = false;
		prev_was_open_bracket = false;
	} 

	if (prev_was_operator || prev_was_open_bracket) {
		throw ("Error : Invalid pattern expression");
	}
	if (open_bracket_count != 0) {
		throw ("Error : Invalid pattern expression");
	}

}


string QuerySyntaxValidator::trimLeadingWhitespaces(const string& s) {
	return regex_replace(s, regex("^\\s+"), string(""));
}

string QuerySyntaxValidator::trimTrailingWhitespaces(const string& s) {
	return std::regex_replace(s, regex("\\s+$"), string(""));
}

string QuerySyntaxValidator::trimAllWhitespaces(const string& s) {
	return std::regex_replace(s, regex("^\\s+$"), string(""));
}

bool QuerySyntaxValidator::isInteger(string i) {
	regex expr("[0-9]*");

	bool res;

	if (regex_match(i, expr)) {
		res = true;
	}
	else {
		res = false;
	}
	return res;
}

bool QuerySyntaxValidator::isEntRef(string s) {
	bool res = false;
	if (s.length() <= 0) {
		throw ("Error : Invalid entRef Name");
	}

	// cout << "entref we checking : " << s << endl;;
	if (s.at(0) == ('\"') && s.at(s.length()- 1) == ('\"')) {
		// "this is also allowed" 
		string substr = s.substr(1, (s.length() - 2));
		// cout << "Checking inside the inverted commas : " << substr << endl;
		if (validateVariableName(substr)) {

			// cout << substr << " is " << " valid " << endl;
			res = true;
		}
		//	cout << "Yes s, is ent " << s << endl;
	} else if (!isInteger(s)) {
		// var name is valid
		if (validateVariableName(s)) {
			res = true;
		}
	} else if (s.compare("_") == 0) {
		res = true;
	}

	return res;
}

bool QuerySyntaxValidator::isOperator(char c) {
	bool res = false;
	
	switch (c) {
	case '+':
		res = true;
	case '/':
		res = true;
	case '-':
		res = true;
	case '%':
		res = true;
	case '*':
		res = true;
	}
	
	return res;
}

bool QuerySyntaxValidator::isUnderscore(string s) {
	return s == "_";
}