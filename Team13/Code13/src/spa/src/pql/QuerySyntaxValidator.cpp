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
	Returns an unordered map of the variables user declared.
	Key -> value pair is varname -> 0 if var is a stmt, varname -> 1 if var is a proc
*/
unordered_map<string, string> QuerySyntaxValidator::ValidateDeclaration(string decl) {
	// this one validates the declaration
	// ie. assign a, b -> true, assijn a, b -> false, stmt s -> true
	/*
		1. Split by ","
		2. Split by " "
		3. Check first word, if it exists in valid design entities
	*/

	string decl_clone = decl;
	vector<string> temp_results;

	unordered_map<string, string> variable_names_declared;
	// if variable name is a PROCEEDURE, then <VAR_NAME, 1>
	// if variable name is a STMT, then <VAR_NAME, 0>
	// for each key-value pair.

	vector<string> final_results;

	string comma = ",";
	size_t pos = 0;
	string token;

	decl_clone = TrimLeadingWhitespaces(decl_clone);
	decl_clone = TrimTrailingWhitespaces(decl_clone);

	// this while loop is splitting by comma.
	// assign a, b, c -> [assign a] [b] [c]
	while ((pos = decl_clone.find(comma)) != string::npos) {
		token = decl_clone.substr(0, pos);
		token = TrimLeadingWhitespaces(token);
		token = TrimTrailingWhitespaces(token);
		temp_results.push_back(token);

		decl_clone.erase(0, pos + comma.length());
		// cout << "clone b : " << temp_clone << endl;
		decl_clone = TrimLeadingWhitespaces(decl_clone);
		decl_clone = TrimTrailingWhitespaces(decl_clone);
		// cout << "clone a : " << temp_clone << endl;

	}

	temp_results.push_back(decl_clone);

	string variable_with_entity = temp_results.front(); // variable with entity is the first set delimited by comma, assign a, b, c -> [assign a]
	temp_results.erase(temp_results.begin());

	vector<std::string> first_stmt; // first_stmt is everything before the first comma.
	std::istringstream iss(variable_with_entity);
	string entity_type = variable_with_entity.substr(0, variable_with_entity.find_first_of(" "));

	// cout << "Entity Type : " << entity_type << endl;

	for (auto var_name : temp_results) {
		ValidateVariableName(var_name);
		if (variable_names_declared.count(var_name) == 1) {
			throw ("Error : Declared same variable name twice ");
		}
		// cout << "Does it continue? ";
		// cout << "Insert variable name to set : " << var_name << endl;
		// cout << "entity type is :" << entity_type << "|" << endl;
		// inserts the variables into a set to later check if the user repeated the values
		if (entity_type.compare("variable") == 0) {
			variable_names_declared[var_name] = "variable";
		}
		else if (entity_type.compare("procedure") == 0) {
			variable_names_declared[var_name] = "procedure";
		}
		else {
			variable_names_declared[var_name] = "stmtType";
		}
		// cout << "number put in :" << variable_names_declared.at(var_name) << endl;
		final_results.push_back(var_name);
	}

	iss >> variable_with_entity;
	for (string variable_with_entity; iss >> variable_with_entity;) {
		first_stmt.push_back(variable_with_entity);
		if (variable_names_declared.count(first_stmt.back()) == 1) {
			throw ("Error : Declared same variable name twice ");
		}
		// cout << "Insert variable name to set : " << first_stmt.back() << endl;
		if (entity_type.compare("variable") == 0) {
			variable_names_declared[first_stmt.back()] = "variable";
		}
		else if (entity_type.compare("procedure") == 0) {
			variable_names_declared[first_stmt.back()] = "procedure";
		}
		else {
			variable_names_declared[first_stmt.back()] = "stmtType";
		}
		final_results.push_back(first_stmt.back());
	}

	for (auto i : first_stmt) {
		// cout << "First stmt : " << i << endl;
	}

	if (first_stmt.size() != 1) {
		throw ("Error : Invalid Design Entity Declarationsdfsdf!");
	}

	// checks if first word is a valid design entity
	if (validDesignEntities.count(entity_type) != 1) {
		throw ("Error : Invalid Design Entity Type declared!");
	}

	return variable_names_declared;
}

void QuerySyntaxValidator::ValidateSelectClauseStartsWithSelect(string s) {
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

bool QuerySyntaxValidator::ValidateVariableName(string s) {
	// validates if a variable name follows the rules of variables in SIMPLE
	/*
		Basically NAME: LETTER (LETTER | DIGIT)*
		a variable name must start with letter, then can be followed by either letter or digit.
		Can be just a Letter, i.e. J
		e.g. J2Lpm -> true
		24Sdf -> not valid variable name, throw error
	*/

	regex expr("[A-Za-z][a-zA-Z_0-9]*");

	string str = TrimLeadingWhitespaces(s);
	str = TrimTrailingWhitespaces(str);

	if (!regex_match(str, expr)) {
		throw ("Error : Invalid variable name!");
	}
	return true;
}

bool QuerySyntaxValidator::ValidateVariableExists(string s, unordered_map<string, string> varNames) {
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
	if (!res) {
		throw ("Error : variable user wants to Select has not been declared! ");
	}
	return res;
}

unordered_map<string, vector<string>> QuerySyntaxValidator::ValidateSuchThatClause(string s, unordered_map<string, string> declaredVarNames) {
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
	declared_relRef = TrimTrailingWhitespaces(declared_relRef);

	if (validRelrefNames.count(declared_relRef) != 1) {
		throw ("Error : Invalid Relational-Reference name!");
	}

	// cout << "Declared Relref : " << declared_relRef << endl;

	// cout << temp << endl;

	temp = TrimTrailingWhitespaces(temp); // if e.g. "Follows (s1, s2)" -> temp is now left with "s1, s2)"

	if (temp.back() != ')') {
		throw ("Error : Invalid Relation-Reference Format");
	}

	if ((pos_comma = temp.find(comma_delimiter)) == string::npos) {
		throw ("Error : Invalid Relation-Reference Format, comma not found! ");
	}

	string first_argument = temp.substr(0, pos_comma);

	first_argument = TrimLeadingWhitespaces(first_argument);
	first_argument = TrimTrailingWhitespaces(first_argument);

	// cout << "such that clause : First argument : " << first_argument << endl;
	// validateVariableName(first_argument);

	temp.erase(0, pos_comma + comma_delimiter.length());
	temp = TrimLeadingWhitespaces(temp);
	temp = TrimTrailingWhitespaces(temp);

	string second_argument = temp.substr(0, temp.length() - 1);

	second_argument = TrimLeadingWhitespaces(second_argument);
	second_argument = TrimTrailingWhitespaces(second_argument);
	// validateVariableName(second_argument);

	// cout << "such that clause : Second argument : " << second_argument << endl;

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
		if (declaredVarNames.count(first_argument) != 1 && !IsInteger(first_argument) && !IsUnderscore(first_argument)) { // first argument wasnt defined by the user
			throw ("Error : First argument is not declared or is invalid ");
		}
		if (declaredVarNames.count(second_argument) != 1 && !IsInteger(second_argument) && !IsUnderscore(second_argument)) { // second argument wasnt defined by the user
			throw ("Error : Second argument is not declared or is invalid ");
		}

		/*
		if (isInteger(second_argument)) {
			throw ("Error : Second argument cannot be an Integer ");
		}
		*/

		if (declaredVarNames.count(first_argument) == 1) {
			if (declaredVarNames.at(first_argument).compare("stmtType") != 0) {
				throw ("Error : First argument must be type Stmt in Follows/Parent clause ");
			}
		}

		if (declaredVarNames.count(second_argument) == 1) {
			if (declaredVarNames.at(second_argument).compare("stmtType") != 0) {
				throw ("Error : First argument must be type Stmt in Follows/Parent clause ");
			}
		}

	}

	/*
		UsesS : ‘Uses’ ‘(’ stmtRef ‘,’ entRef ‘)’
		UsesP: ‘Uses’ ‘(’ entRef ‘, ’ entRef ‘)’
		ModifiesS : ‘Modifies’ ‘(’ stmtRef ‘, ’ entRef ‘)’
		ModifiesP : ‘Modifies’ ‘(’ entRef ‘, ’ entRef ‘)’
	*/

	/*
	if (isInteger(second_argument)) {
		throw ("Error : Second argument cannot be an Integer ");
	}
	*/

	// function name is Uses
	if (declared_relRef.compare("Uses") == 0) {
		// regardless, 2nd argument must be entRef
		if (declaredVarNames.count(second_argument) == 1) {
			if (declaredVarNames.at(second_argument).compare("stmtType") == 0) {
				throw ("Error : 2nd argument must be of type entref");
			}
		}
		// FIRST ARGUMENT CANNOT BE UNDERSCORE
		if (IsUnderscore(first_argument)) {
			throw ("Error : First argument cannot be underscore ");
		}
		// first argument is an entRef
		// cout << "what is first arg : " << first_argument;
		if (IsInteger(first_argument)) {
			assigned_relRef = "UsesS";
		}
		else if ((declaredVarNames.count(first_argument) == 1) && declaredVarNames.at(first_argument).compare("stmtType") == 0) {
			assigned_relRef = "UsesS";
		}
		else {
			assigned_relRef = "UsesP";
		}
	}

	// function name is Modifies
	if (declared_relRef.compare("Modifies") == 0) {
		// regardless, 2nd argument must be entRef
		if (declaredVarNames.count(second_argument) == 1) {
			if (declaredVarNames.at(second_argument).compare("stmtType") == 0) {
				throw ("Error : 2nd argument must be of type entref");
			}
		}
		if (IsUnderscore(first_argument)) {
			throw ("Error : First argument cannot be underscore ");
		}
		// first argument is an entRef
		if (IsInteger(first_argument)) {
			assigned_relRef = "ModifiesS";
		}
		else if ((declaredVarNames.count(first_argument) == 1) && declaredVarNames.at(first_argument).compare("stmtType") == 0) {
			assigned_relRef = "ModifiesS";
		}
		else {
			assigned_relRef = "ModifiesP";
		}
	}

	// cout << "declared_relRef : " << declared_relRef << endl;
	// cout << "assigned_relRef : " << assigned_relRef << endl;


	// regardless, first argument cannot be '_'
	if (assigned_relRef.compare("UsesS") == 0 || assigned_relRef.compare("ModifiesS") == 0) {
		// first argument is a statement ref
		if ((declaredVarNames.count(first_argument) != 1 && !IsInteger(first_argument)) && !IsUnderscore(first_argument)) {
			// first argument wasnt defined by the user or is just invalid
			throw ("Error : First argument is not declared or is invalid ");
		}
		if (IsInteger(second_argument)) {
			throw ("Error : Second argument cannot be an Integer ");
		}
		// second argument is an entity ref
		string parsed_second_argument = second_argument;

		if (declaredVarNames.count(second_argument) == 1) {
			// found second arg in declared var names
			// if this second argument is NOT A VARIABLE declared by USER
			// cout << "2nd arg here : " << second_argument << "with number : " << declaredVarNames.at(second_argument) << endl;
			if (declaredVarNames.at(second_argument).compare("stmtType") == 0) {
				throw ("Error : second argument is not of type variable/procedure declared by the user");
			}
		}

		if (parsed_second_argument.at(0) == ('\"') && parsed_second_argument.at(parsed_second_argument.length() - 1) == ('\"')) {
			parsed_second_argument = second_argument.substr(1, second_argument.length() - 2);
		}
		//cout << "2nd arg : " << second_argument << endl;

		// cout << "123123 second arg : " << second_argument << endl;
		// is inside or is an EntRef
		if (!IsUnderscore(second_argument)) {
			if (declaredVarNames.count(second_argument) != 1 && !IsEntRef(second_argument, declaredVarNames)) {
				// second argument wasnt defined user or is just invalid
				throw ("Error : Second argument is not declared or is invalid");
			}
		}

		// cout << "parsed 2nd arg : " << parsed_second_argument << endl;
		if (assigned_relRef.compare("UsesS") == 0) {
			arguments.push_back(first_argument);
			arguments.push_back(second_argument);
			relRef_map["UsesS"] = arguments;
		}

		if (assigned_relRef.compare("ModifiesS") == 0) {
			arguments.push_back(first_argument);
			arguments.push_back(second_argument);
			relRef_map["ModifiesS"] = arguments;
		}
	}


	if (assigned_relRef.compare("UsesP") == 0 || assigned_relRef.compare("ModifiesP") == 0) {
		// first argument is a entity ref
		// string parsed_first_argument = first_argument.substr(1, first_argument.length() - 2);
		//cout << "such that clause :  1st arg : " << parsed_first_argument << endl;

		//cout << "FIRST ARG : " << first_argument;
		if (!IsEntRef(first_argument, declaredVarNames)) {
			throw ("Error : First argument is not declared or is invalid ");

		}

		if (IsInteger(second_argument)) {
			throw ("Error : Second argument cannot be an Integer ");
		}

		if (declaredVarNames.count(second_argument) == 1) {
			// found second arg in declared var names
			// if this second argument is NOT A VARIABLE declared by USER
			if (declaredVarNames.at(second_argument).compare("stmtType") == 0) {
				throw ("Error : second argument is not of type variable/procedure declared by the user");
			}
		}

		//cout << "modifies 2nd arg :" << second_argument;
		if (!IsUnderscore(second_argument)) {
			if (!IsEntRef(second_argument, declaredVarNames)) {

				throw ("Error : Second argument is not declared or is invalid ");

			}
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

unordered_map<string, vector<string>> QuerySyntaxValidator::ValidatePatternClause(string s, unordered_map<string, string> declaredVarNames) {
	// validates what after pattern clause is legit
	/*
		Select a pattern a ( _ , _“v + x * y + z * t”_)
		Pass in this portion "( _ , _“v + x * y + z * t”_)"
		I think we dont need to care about the arguments passed
	*/

	unordered_map<string, vector<string>> result;
	vector<string> arguments;
	string str = s;
	string comma = ",";
	int pos_comma;

	str = TrimLeadingWhitespaces(str);
	str = TrimTrailingWhitespaces(str);

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

	str = TrimLeadingWhitespaces(str);
	pos_comma = str.find(comma);
	string first_argument = str.substr(0, pos_comma);
	first_argument = TrimLeadingWhitespaces(first_argument);
	first_argument = TrimTrailingWhitespaces(first_argument);

	// cout << "pattern clause first arg : " << first_argument << endl;
	arguments.push_back(first_argument);

	str.erase(0, pos_comma + comma.length());

	str = TrimLeadingWhitespaces(str);

	string second_argument = str.substr(0, str.length() - 1);
	second_argument = TrimLeadingWhitespaces(second_argument);
	second_argument = TrimTrailingWhitespaces(second_argument);


	// cout << "pattern second arg : " << second_argument << endl;
	arguments.push_back(second_argument);

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

	if (!IsEntRef(first_argument, declaredVarNames)) {
		// cout << "first argument " << first_argument << "is not an entity ref" << endl;
		if (!IsUnderscore(first_argument)) {
			if (!ValidateVariableName(first_argument)) {
				throw ("Error : Invalid pattern expression");
			}
		}
	}

	if (!IsUnderscore(second_argument)) {
		if (second_argument.front() == '_') {
			second_argument.erase(0, 1);
			if (second_argument.back() == '_') {
				// value looks like "x + y"_
				second_argument.erase(second_argument.length() - 1, second_argument.length());
				// cout << "2nd arg test: " << second_argument;
			}
			else {
				throw ("Error : Invalid argument for Pattern clause!");
			}
		}
		if (second_argument.back() == '_') {
			throw ("Error : Invalid argument for Pattern clause!");
		}

		// cout << "2nd arg test: " << second_argument;
		// 2nd argument will be in the form "x + y" no matter what
		// erase the "" again
		// now is just in the form x + y
		second_argument.erase(0, 1);
		second_argument.erase(second_argument.length() - 1, second_argument.length());
		// cout << "2nd arg test: " << second_argument;
		ValidateExpression(second_argument);
	}


	result["pattern"] = arguments;

	return result;
}

void QuerySyntaxValidator::ValidateExpression(const string& s) {
	string str = s;
	if (s.length() <= 0) {
		throw ("Error : Invalid pattern expression");
	}

	str = TrimLeadingWhitespaces(str);
	str = TrimTrailingWhitespaces(str);

	int open_bracket_count = 0;
	bool prev_was_operator = false;
	bool prev_was_open_bracket = false;

	if (IsOperator(str.front()) || IsOperator(str.back())) {
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
		else if (IsOperator(c)) {
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


string QuerySyntaxValidator::TrimLeadingWhitespaces(const string& s) {
	return regex_replace(s, regex("^\\s+"), string(""));
}

string QuerySyntaxValidator::TrimTrailingWhitespaces(const string& s) {
	return std::regex_replace(s, regex("\\s+$"), string(""));
}

string QuerySyntaxValidator::TrimAllWhitespaces(const string& s) {
	return std::regex_replace(s, regex("^\\s+$"), string(""));
}

bool QuerySyntaxValidator::IsInteger(string i) {
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

bool QuerySyntaxValidator::IsEntRef(string s, unordered_map<string, string> declaredVars) {
	bool res = false;
	if (s.length() <= 0) {
		throw ("Error : Invalid entRef Name");
	}

	// cout << "entref we checking : " << s << endl;;
	if (s.at(0) == ('\"') && s.at(s.length() - 1) == ('\"')) {
		// "this is also allowed" 
		string substr = s.substr(1, (s.length() - 2));
		// cout << "Checking inside the inverted commas : " << substr << endl;
		if (ValidateVariableName(substr)) {

			// cout << substr << " is " << " valid " << endl;
			res = true;
		}
		//	cout << "Yes s, is ent " << s << endl;
	}
	else if (!IsInteger(s)) {
		// var name is valid
		if (s.compare("_") != 0) {
			if (ValidateVariableName(s) && declaredVars.count(s) == 1) {
				res = true;
			}
		}
	}
	else if (s.compare("_") == 0) {
		res = true;
	}

	return res;
}

bool QuerySyntaxValidator::IsOperator(char c) {
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

bool QuerySyntaxValidator::IsUnderscore(string s) {
	return s == "_";
}

int QuerySyntaxValidator::nthOccurrence(string* str, const string& target, int nth)
{
	size_t  pos = 0;
	int     cnt = 0;

	while (cnt != nth)
	{
		pos += 1;
		pos = str->find(target, pos);
		if (pos == string::npos)
			return -1;
		cnt++;
	}
	return pos;
}