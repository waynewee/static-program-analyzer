#include "QueryRules.h"
#include "WhitespaceHandler.h"
#include <sstream>
#include <regex>
#include <string>
#include <iostream>
using namespace std;

const STRING_SET valid_design_entities = { "stmt" , "read" , "print" , "call" , "while" , "if" , "assign" , "variable" , "constant" , "prog_line" , "procedure" };
const STRING_SET valid_rel_ref_names = { "Follows" , "FollowsT" , "Follows*" , "Parent" , "ParentT" ,
											"Parent*" , "Uses" , "UsesS" , "UsesP" , "Modifies" ,  "ModifiesS" , "ModifiesP" ,
												"Calls" , "CallsT" , "Calls*" , "Next" , "NextT" , "Next*" };
const STRING_SET valid_stmt_ref_synonyms = { "stmt" , "read" , "print" , "call" , "while" , "if" , "assign" };
const STRING_SET valid_ent_ref_synonyms = { "variable", "procedure" };
const STRING_SET valid_line_ref_synonyms = { "prog_line" };
const STRING_SET valid_attr_names = { "procName" , "varName" , "value" , "stmt#" };


BOOLEAN QueryRules::IsIdent(STRING token) {
	BOOLEAN result = false;
	regex expr("[A-Za-z][a-zA-Z_0-9]*");
	if (regex_match(token, expr)) {
		result = true;
	}
	return result;
}

BOOLEAN QueryRules::IsName(STRING token) {
	BOOLEAN result = false;
	regex expr("[A-Za-z][a-zA-Z_0-9]*");
	if (regex_match(token, expr)) {
		result = true;
	}
	return result;
}

BOOLEAN QueryRules::IsInteger(STRING token) {
	BOOLEAN result = false;
	regex expr("[0-9]*");
	if (regex_match(token, expr)) {
		result = true;
	}
	return result;
}

BOOLEAN QueryRules::IsSynonym(STRING token) {
	BOOLEAN result = false;
	if (IsIdent(token)) {
		result = true;
	}
	return result;
}

BOOLEAN QueryRules::IsStmtRef(STRING token, STRING synonym_type) {
	BOOLEAN result = false;
	if (valid_stmt_ref_synonyms.count(synonym_type) == 1) {
		if (IsSynonym(token)) {
			result = true;
		}
	}
	else if (token.compare("_") == 0) {
		result = true;
	}
	else if (IsInteger(token)) {
		result = true;
	}
	return result;
}

BOOLEAN QueryRules::IsEntRef(STRING token, STRING synonym_type) {
	BOOLEAN result = false;
	if (valid_ent_ref_synonyms.count(synonym_type) == 1) {
		if (IsSynonym(token)) {
			result = true;
		}
	}
	else if (token.compare("_") == 0) {
		result = true;
	}
	else if (token.front() == '\"' && token.back() == '\"') {
		// remember to ensure that what's inside the "" is already trimmed!
		STRING ident = token.substr(1, token.length() - 2);
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&ident);
		if (IsIdent(ident)) {
			result = true;
		}
	}
	return result;
}

BOOLEAN QueryRules::IsLineRef(STRING token, STRING synonym_type) {
	BOOLEAN result = false;
	if (valid_line_ref_synonyms.count(synonym_type) == 1) {
		if (IsSynonym(token)) {
			result = true;
		}
	}
	else if (token.compare("_") == 0) {
		result = true;
	}
	else if (IsInteger(token)) {
		result = true;
	}
	return result;
}

BOOLEAN QueryRules::IsElem(STRING token) {
	BOOLEAN result = false;
	if (IsSynonym(token)) {
		result = true;
	}

	if (IsAttrRef(token)) {
		result = true;
	}

	return result;
}

BOOLEAN QueryRules::IsAttrName(STRING token) {
	BOOLEAN result = false;
	if (valid_attr_names.count(token) == 1) {
		result = true;
	}
	return result;
}

BOOLEAN QueryRules::IsDesignEntity(STRING token) {
	BOOLEAN result = false;
	if (valid_design_entities.count(token) == 1) {
		result = true;
	}
	return result;
}

BOOLEAN QueryRules::IsSelect(STRING token) {
	BOOLEAN result = false;
	if (token.compare(TYPE_SELECT_CLAUSE) == 0) {
		result = true;
	}
	return result;
}

/*
	Remember to pass in "stmt s" without the semicolon.
*/
BOOLEAN QueryRules::IsDeclaration(STRING token, STRING_STRING_MAP entity_map) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING sub_token;
	STRING comma = ",";
	size_t pos = 0;
	BOOLEAN first_pass = true;
	STRING_SET declared_synonyms;
	// If there is a comma found (indicating multiple synonyms)
	if (temp_token.find(comma) != STRING::npos) {
		while ((pos = temp_token.find(comma)) != STRING::npos) {
			WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
			sub_token = temp_token.substr(0, temp_token.find_first_of(","));
			WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&sub_token);
			//cout << "SUBTOKEN:" << sub_token << endl;
			if (first_pass) {
				// Here should have the design-entity and synonym
				STRING supposed_design_entity = sub_token.substr(0, sub_token.find_first_of(" "));
				//cout << "sde:" << supposed_design_entity << "|" << endl;
				if (!IsDesignEntity(supposed_design_entity)) {
					result = false;
					break;
				}
				// Erase the design-entity
				sub_token.erase(0, sub_token.find_first_of(" "));
				temp_token.erase(0, temp_token.find_first_of(" "));
				first_pass = false;
			}
			// sub_token is now a synonym. check if is synonym
			WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&sub_token);
			//cout << "Subtoken:" << sub_token << endl;
			if (!IsSynonym(sub_token)) {
				result = false;
				break;
			}
			if (declared_synonyms.count(sub_token) != 0) {
				result = false;
				break;
			}
			if (entity_map.count(sub_token) != 0) {
				result = false;
				break;
			}

			else {
				declared_synonyms.insert(sub_token);
			}
			// Erase the validated segment from the temp_token
			// cout << "TEMPTOKEN:" << temp_token << endl;
			WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
			temp_token.erase(0, temp_token.find_first_of(",") + 1);
		}
		// Passed the last comma, this is checking for last synonym.
		// cout << "Last value:" << temp_token << endl;
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		if (!IsSynonym(temp_token) || declared_synonyms.count(temp_token) != 0) {
			result = false;
		}
		if (entity_map.count(temp_token) != 0) {
			result = false;
		}
	}
	else {
		// No comma found. One synonym declared.
		STRING supposed_design_entity = temp_token.substr(0, temp_token.find_first_of(" "));
		if (!IsDesignEntity(supposed_design_entity)) {
			result = false;
		}
		temp_token.erase(0, temp_token.find_first_of(" "));
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		if (!IsSynonym(temp_token)) {
			result = false;
		}
		if (entity_map.count(temp_token) != 0) {
			result = false;
			return result;
		}
	}
	if (DEBUG) {
		if (!result) {
			cout << "Error at QueryRules::IsDeclaration()" << endl;
		}
	}
	return result;
}

BOOLEAN QueryRules::IsResultClause(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = false;
	if (IsTuple(token, declared_var_names)) {
		result = true;
	}
	if (token.compare("BOOLEAN") == 0) {
		result = true;
	}
	return result;
}

BOOLEAN QueryRules::IsTuple(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = false;
	STRING temp_token = token;
	// cout << "temp_token:" << temp_token << endl;
	if (IsElem(temp_token)) {
		// cout << "yes is elem" << endl;
		// element must be declared by user
		if (IsAttrRef(temp_token)) {
			// cout << "yes is attrRef" << endl;
			// there is a dot
			STRING synonym_subtoken = temp_token.substr(0, temp_token.find_first_of("."));
			// cout << "synonym_st" << synonym_subtoken << endl;
			if (declared_var_names.count(synonym_subtoken) == 1) {
				result = true;
				return result;
			}
		}
		if (declared_var_names.count(token) == 1) {
			result = true;
			return result;
		}
	}
	STRING sub_token;
	STRING comma = ",";
	size_t pos = 0;
	if (temp_token.front() == '<' && temp_token.back() == '>') {
		temp_token = temp_token.substr(1, temp_token.length() - 2);
		while ((pos = temp_token.find(comma)) != STRING::npos) {
			sub_token = temp_token.substr(0, pos);
			// cout << "subtoken:" << sub_token << "|" << endl;
			WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&sub_token);

			if (IsAttrRef(sub_token)) {
				STRING synonym_subtoken = sub_token.substr(0, sub_token.find_first_of("."));
				// cout << "synonym_st" << synonym_subtoken << endl;
				if (declared_var_names.count(synonym_subtoken) != 1) {
					result = false;
					return result; // synonym not declared.
				}
				else {
					result = true;
				}
			}
			else if (IsSynonym(sub_token)) {
				if (declared_var_names.count(sub_token) != 1) {
					result = false;
					return result;
				}
				else {
					result = true;
				}
			}
			else {
				// if not attrRef or synonym, return false.
				result = false;
				return result;
			}
			temp_token.erase(0, temp_token.find_first_of(",") + 1);
		}
		// last token : 
		cout << "we reach last token. " << endl;
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		cout << "last token:" << temp_token << endl;
		if (IsAttrRef(temp_token)) {
			STRING synonym_subtoken = temp_token.substr(0, temp_token.find_first_of("."));
			// cout << "synonym_st" << synonym_subtoken << endl;
			if (declared_var_names.count(synonym_subtoken) != 1) {
				result = false;
				return result; // synonym not declared.
			}
			else {
				result = true;
			}
		}
		else if (IsSynonym(temp_token)) {
			if (declared_var_names.count(temp_token) != 1) {
				result = false;
				return result;
			}
			else {
				result = true;
			}
		}
		else {
			result = false;
			return result;
		}
	}
	if (DEBUG) {
		if (!result) {
			cout << "Error at QueryRules::IsTuple()" << endl;
		}
	}
	return result;
}

BOOLEAN QueryRules::IsWithClause(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING with_token = temp_token.substr(0, temp_token.find_first_of(" "));
	if (with_token.compare(TYPE_WITH_CLAUSE) != 0) {
		result = false;
	}
	temp_token.erase(0, temp_token.find_first_of(" "));
	WhitespaceHandler::TrimLeadingWhitespaces(&temp_token);
	STRING attr_cond_token = temp_token;

	if (!IsAttrCond(attr_cond_token, declared_var_names)) {
		result = false;
	}

	return result;
}

BOOLEAN QueryRules::IsSuchThatClause(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	size_t second_whitespace_pos = 0;
	int count = 0;
	while (count != 2)
	{
		second_whitespace_pos += 1;
		second_whitespace_pos = temp_token.find(" ", second_whitespace_pos);
		count++;
	}
	if (second_whitespace_pos > 9) {
		second_whitespace_pos = temp_token.length() - 1;
	}
	STRING suchthat_token = temp_token.substr(0, second_whitespace_pos);
	if (suchthat_token.compare(TYPE_SUCH_THAT_CLAUSE) != 0) {
		result = false;
	}
	temp_token.erase(0, temp_token.find_first_of(" ") + 1); // erase such
	temp_token.erase(0, temp_token.find_first_of(" ")); // erase that
	WhitespaceHandler::TrimLeadingWhitespaces(&temp_token);
	STRING rel_cond_token = temp_token;

	if (!IsRelCond(rel_cond_token, declared_var_names)) {
		result = false;
	}

	//cout << "relcond:" << rel_cond_token << endl;
	return result;
}

BOOLEAN QueryRules::IsPatternClause(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING pattern_token = temp_token.substr(0, temp_token.find_first_of(" "));
	if (pattern_token.compare(TYPE_COND_PATTERN) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of(" "));
	WhitespaceHandler::TrimLeadingWhitespaces(&temp_token);
	STRING pattern_cond_token = temp_token;

	if (!IsPatternCond(pattern_cond_token, declared_var_names)) {
		result = false;
		return result;
	}

	// cout << "patterncond:" << pattern_cond_token << endl;
	return result;
}

BOOLEAN QueryRules::IsAttrCond(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;

	STRING temp_token = token;
	STRING and_token = "and";
	size_t pos = 0;
	while ((pos = temp_token.find(and_token)) != STRING::npos) {
		// while we can find 'and'
		STRING supposed_attr_cond = temp_token.substr(0, temp_token.find_first_of("and"));
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&supposed_attr_cond);
		if (!IsAttrCompare(supposed_attr_cond, declared_var_names)) {
			result = false;
			return result;
		}
		temp_token.erase(0, temp_token.find_first_of("and") + 3);
	}
	return result;
}

BOOLEAN QueryRules::IsAttrCompare(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING first_ref = token.substr(0, token.find_first_of("="));
	STRING second_ref = token.substr(token.find_first_of("=") + 1, token.length());
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_ref);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_ref);
	// cout << "first_ref:" << first_ref << endl;
	// cout << "second_ref:" << second_ref << endl;
	STRING first_ref_synonym_type = "none";
	STRING second_ref_synonym_type = "none";
	if (declared_var_names.count(first_ref_synonym_type) != 0) {
		first_ref_synonym_type = declared_var_names.at(first_ref_synonym_type);
	}
	if (declared_var_names.count(second_ref_synonym_type) != 0) {
		second_ref_synonym_type = declared_var_names.at(second_ref_synonym_type);
	}
	if (!IsRef(first_ref, first_ref_synonym_type)) {
		result = false;
		return result;
	}
	if (!IsRef(second_ref, second_ref_synonym_type)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsRef(STRING token, STRING synonym_type) {
	BOOLEAN result = false;
	if (token.front() == '\"' && token.back() == '\"') {
		// remember to ensure that what's inside the "" is already trimmed!
		STRING ident = token.substr(1, token.length() - 2);
		if (IsIdent(ident)) {
			result = true;
		}
	}
	if (IsInteger(token)) {
		result = true;
	}
	if (IsAttrRef(token)) {
		result = true;
	}

	if (IsSynonym(token) && synonym_type.compare("prog_line") == 0) {
		result = true;
	}
	return result;
}

BOOLEAN QueryRules::IsAttrRef(STRING token) {
	BOOLEAN result = true;
	size_t pos_of_fullstop = token.find(".");
	STRING synonym_token = token.substr(0, pos_of_fullstop);
	STRING attr_name_token = token.substr(pos_of_fullstop + 1, token.length() - 1);
	if (!IsSynonym(synonym_token)) {
		result = false;
	}
	if (!IsAttrName(attr_name_token)) {
		result = false;
	}
	return result;
}

BOOLEAN QueryRules::IsRelCond(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);

	STRING and_token = "and";
	size_t pos = 0;
	while ((pos = temp_token.find(and_token)) != STRING::npos) {
		// while we can find 'and'
		cout << "found AND: " << endl;
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		STRING supposed_rel_ref = temp_token.substr(0, temp_token.find_first_of("and"));
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&supposed_rel_ref);
		cout << "supposed_rel_ref:" << supposed_rel_ref << "|" << endl;
		temp_token.erase(0, temp_token.find_first_of("and") + 3);
		if (!IsRelRef(supposed_rel_ref, declared_var_names)) {
			result = false;
			return result;
		}
	}
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token); // no and only one clause, OR and is erased until last clause

	// cout << "relCond temptoken:" << temp_token << endl;
	if (!IsRelRef(temp_token, declared_var_names)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsRelRef(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = false;
	if (IsModifiesP(token, declared_var_names)) {
		result = true;
		// cout << "It is ModifiesP" << endl;
		return result;
	}
	if (IsModifiesS(token, declared_var_names)) {
		result = true;
		// cout << "It is ModifiesS" << endl;
		return result;
	}
	if (IsUsesP(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsUsesS(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsCalls(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsCallsT(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsParent(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsParentT(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsFollows(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsFollowsT(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsNext(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsNextT(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsAffects(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsAffectsT(token, declared_var_names)) {
		result = true;
		return result;
	}
	return result;
}

STRING QueryRules::GetRelRefType(STRING token, STRING_STRING_MAP declared_var_names) {
	STRING type = "none";
	if (IsModifiesP(token, declared_var_names)) {
		type = TYPE_COND_MODIFIES_P;
		// cout << "It is ModifiesP" << endl;
		return type;
	}
	if (IsModifiesS(token, declared_var_names)) {
		type = TYPE_COND_MODIFIES_S;
		// cout << "It is ModifiesS" << endl;
		return type;
	}
	if (IsUsesP(token, declared_var_names)) {
		type = TYPE_COND_USES_P;
		return type;
	}
	if (IsUsesS(token, declared_var_names)) {
		type = TYPE_COND_USES_S;
		return type;
	}
	if (IsCalls(token, declared_var_names)) {
		type = TYPE_COND_CALLS;
		return type;
	}
	if (IsCallsT(token, declared_var_names)) {
		type = TYPE_COND_CALLS_T;
		return type;
	}
	if (IsParent(token, declared_var_names)) {
		type = TYPE_COND_PARENT;
		return type;
	}
	if (IsParentT(token, declared_var_names)) {
		type = TYPE_COND_PARENT_T;
		return type;
	}
	if (IsFollows(token, declared_var_names)) {
		type = TYPE_COND_FOLLOWS;
		return type;
	}
	if (IsFollowsT(token, declared_var_names)) {
		type = TYPE_COND_FOLLOWS_T;
		return type;
	}
	if (IsNext(token, declared_var_names)) {
		type = TYPE_COND_NEXT;
		return type;
	}
	if (IsNextT(token, declared_var_names)) {
		type = TYPE_COND_NEXT_T;
		return type;
	}
	if (IsAffects(token, declared_var_names)) {
		type = TYPE_COND_AFFECTS;
		return type;
	}
	if (IsAffectsT(token, declared_var_names)) {
		type = TYPE_COND_AFFECTS_T;
		return type;
	}
	return type;
}


BOOLEAN QueryRules::IsModifiesP(STRING token, STRING_STRING_MAP declared_var_names) {
	/*
		Token here should be something like Modifies (p, "x")
		Trailing and leading whitespace should be trimmed before passing in
	*/
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING modifies_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&modifies_token);
	STRING first_arg;
	STRING second_arg;
	if (modifies_token.compare(TYPE_COND_MODIFIES) != 0) {
		result = false;
		return result;
	}
	// Erase Modifies away
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef , cannot be underscore
	if (first_arg.compare("_") == 0) {
		result = false;
		return result;
	}
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsEntRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	STRING second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsEntRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsModifiesS(STRING token, STRING_STRING_MAP declared_var_names) {
	/*
		Token here should be something like Modifies (s, "x")
		Trailing and leading whitespace should be trimmed before passing in
	*/
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING modifies_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&modifies_token);
	STRING first_arg;
	STRING second_arg;
	if (modifies_token.compare(TYPE_COND_MODIFIES) != 0) {
		result = false;
		return result;
	}
	// Erase Modifies away
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef , cannot be underscore
	if (first_arg.compare("_") == 0) {
		result = false;
		return result;
	}
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	STRING second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsEntRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsUsesP(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING uses_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&uses_token);
	STRING first_arg;
	STRING second_arg;
	if (uses_token.compare(TYPE_COND_USES) != 0) {
		result = false;
		return result;
	}
	// Erase Uses away
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef , cannot be underscore
	if (first_arg.compare("_") == 0) {
		result = false;
		return result;
	}
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsEntRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	STRING second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsEntRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsUsesS(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING uses_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&uses_token);
	STRING first_arg;
	STRING second_arg;
	if (uses_token.compare(TYPE_COND_USES) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef , cannot be underscore
	if (first_arg.compare("_") == 0) {
		result = false;
		return result;
	}
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	STRING second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsEntRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsCalls(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING calls_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&calls_token);
	STRING first_arg;
	STRING second_arg;
	if (calls_token.compare(TYPE_COND_CALLS) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsEntRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	STRING second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsEntRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsCallsT(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING calls_star_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&calls_star_token);
	STRING first_arg;
	STRING second_arg;
	if (calls_star_token.compare(TYPE_COND_CALLS_STAR) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsEntRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	STRING second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsEntRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsParent(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING parent_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&parent_token);
	STRING first_arg;
	STRING second_arg;
	if (parent_token.compare(TYPE_COND_PARENT) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	STRING second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsStmtRef(second_arg, second_arg_type) && !IsLineRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsParentT(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING parent_star_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&parent_star_token);
	STRING first_arg;
	STRING second_arg;
	if (parent_star_token.compare(TYPE_COND_PARENT_STAR) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	STRING second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsStmtRef(second_arg, second_arg_type) && !IsLineRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsFollows(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING follows_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&follows_token);
	STRING first_arg;
	STRING second_arg;
	if (follows_token.compare(TYPE_COND_FOLLOWS) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;

	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}

	STRING second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsStmtRef(second_arg, second_arg_type) && !IsLineRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsFollowsT(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING follows_star_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&follows_star_token);
	STRING first_arg;
	STRING second_arg;
	if (follows_star_token.compare(TYPE_COND_FOLLOWS_STAR) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	STRING second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsStmtRef(second_arg, second_arg_type) && !IsLineRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsNext(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING next_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&next_token);
	STRING first_arg;
	STRING second_arg;
	if (next_token.compare(TYPE_COND_NEXT) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsLineRef(first_arg, first_arg_type) && !IsStmtRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	STRING second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsLineRef(second_arg, second_arg_type) && !IsStmtRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsNextT(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING next_star_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&next_star_token);
	STRING first_arg;
	STRING second_arg;
	if (next_star_token.compare(TYPE_COND_NEXT_STAR) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsLineRef(first_arg, first_arg_type) && !IsStmtRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	STRING second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsLineRef(second_arg, second_arg_type) && !IsStmtRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsAffects(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING parent_star_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&parent_star_token);
	STRING first_arg;
	STRING second_arg;
	if (parent_star_token.compare(TYPE_COND_AFFECTS) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	STRING second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsStmtRef(second_arg, second_arg_type) && !IsLineRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsAffectsT(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING parent_star_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&parent_star_token);
	STRING first_arg;
	STRING second_arg;
	if (parent_star_token.compare(TYPE_COND_AFFECTS_STAR) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// First argument must be an entRef
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsStmtRef(first_arg, first_arg_type) && !IsLineRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	// Second argument must be an entRef
	STRING second_arg_type = "none";
	if (declared_var_names.count(second_arg) == 1) {
		second_arg_type = declared_var_names.at(second_arg);
	}
	if (!IsStmtRef(second_arg, second_arg_type) && !IsLineRef(second_arg, second_arg_type)) {
		result = false;
		return result;
	}
	return result;
}

/*
Pattern clause rules here
Select a pattern a ( _ , “v + x * y + z * t”)
*/

BOOLEAN QueryRules::IsPatternCond(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	BOOLEAN first_passed = false;
	while (temp_token.find_first_not_of(' ') != STRING::npos && !temp_token.empty()) {
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		if (first_passed) {
			STRING and_token = temp_token.substr(0, temp_token.find_first_of(" "));
			// cout << "AND TOKEN:" << and_token << endl;
			if (and_token.compare("and") != 0) {
				result = false;
				return result;
			}
			temp_token.erase(0, temp_token.find_first_of(" "));
		}
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		STRING pattern_token = temp_token.substr(0, temp_token.find_first_of(")") + 1);
		// cout << "patterntoken:" << pattern_token << endl;
		if (!IsPattern(pattern_token, declared_var_names)) {
			result = false;
			return result;
		}
		temp_token.erase(0, temp_token.find_first_of(")") + 1);
		first_passed = true;
	}
	return result;
}

BOOLEAN QueryRules::IsPattern(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = false;
	if (IsAssign(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsSynWhile(token, declared_var_names)) {
		result = true;
		return result;
	}
	if (IsSynIf(token, declared_var_names)) {
		result = true;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsAssign(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING syn_assign_token;
	STRING syn_assign_token_type = "none";
	syn_assign_token = temp_token.substr(0, temp_token.find_first_of("("));
	// cout << "synassigntoken:" << syn_assign_token << endl;
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&syn_assign_token);
	// syn-assign must be of type assign
	if (declared_var_names.count(syn_assign_token) != 0) {
		// syn-assign token is declared
		syn_assign_token_type = declared_var_names.at(syn_assign_token);
	}
	// cout << "synassign tt:" << syn_assign_token_type << endl;
	if (syn_assign_token_type != TYPE_DESIGN_ENTITY_ASSIGN) {
		result = false;
		return result;
	}
	STRING first_arg;
	STRING second_arg;
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsEntRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	if (!IsExpressionSpec(second_arg)) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsExpressionSpec(STRING token) {
	BOOLEAN result = false;
	STRING temp_token = token;
	if (temp_token.compare("_") == 0) {
		result = true;
		return result;
	}
	if (temp_token.front() == '\"' && temp_token.back() == '\"') {
		temp_token = temp_token.substr(1, temp_token.length() - 2);
		if (IsExpr(temp_token)) {
			result = true;
			return result;
		}
	}
	if (temp_token.front() == '_' && temp_token.back() == '_') {
		temp_token = temp_token.substr(1, temp_token.length() - 2);
		if (temp_token.front() == '\"' && temp_token.back() == '\"') {
			temp_token = temp_token.substr(1, temp_token.length() - 2);
			if (IsExpr(temp_token)) {
				result = true;
				return result;
			}
		}
	}
	return result;
}

BOOLEAN QueryRules::IsExpr(STRING token) {
	BOOLEAN result = true;
	STRING expression_token = token;

	int open_bracket_count = 0;
	bool prev_was_operator = false;
	bool prev_was_open_bracket = false;

	if (IsOperator(expression_token.front()) || IsOperator(expression_token.back())) {
		result = false;
		return result;
	}

	for (char const& c : expression_token) {
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
				result = false;
				return result;
			}
			open_bracket_count--;
		}
		else if (IsOperator(c)) {
			if (prev_was_operator || prev_was_open_bracket) {
				result = false;
				return result;
			}
			prev_was_operator = true;
			continue;
		}
		else if (!isalpha(c) && !isdigit(c)) {
			result = false;
			return result;
		}
		prev_was_operator = false;
		prev_was_open_bracket = false;
	}

	if (prev_was_operator || prev_was_open_bracket) {
		result = false;
		return result;
	}
	if (open_bracket_count != 0) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsOperator(CHAR c) {
	BOOLEAN result = false;

	switch (c) {
	case '+':
		result = true;
	case '/':
		result = true;
	case '-':
		result = true;
	case '%':
		result = true;
	case '*':
		result = true;
	}

	return result;
}

BOOLEAN QueryRules::IsTerm(STRING token) {
	BOOLEAN result = false;
	STRING temp_token = token;
	if (IsFactor(token)) {
		result = true;
		return result;
	}
	regex operator_regex("[^*/%]+");
	smatch sm;
	regex_search(temp_token, sm, operator_regex);
	INTEGER pos = sm.position();
	cout << "POs:" << pos << endl;
	STRING term_token = temp_token.substr(0, pos);
	STRING factor_token = temp_token.substr(pos + 1, temp_token.length() - 1);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&term_token);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&factor_token);
	cout << "TermToken:" << term_token << endl;
	cout << "factorToken:" << factor_token << endl;

	return result;
}

BOOLEAN QueryRules::IsFactor(STRING token) {
	BOOLEAN result = false;
	if (IsVarName(token)) {
		result = true;
		return result;
	}
	if (IsConstValue(token)) {
		result = true;
		return result;
	}
	// if expr
	return result;
}

BOOLEAN QueryRules::IsVarName(STRING token) {
	return IsName(token);
}

BOOLEAN QueryRules::IsConstValue(STRING token) {
	return IsInteger(token);
}

BOOLEAN QueryRules::IsSynIf(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING syn_if_token;
	STRING syn_if_token_type = "none";
	syn_if_token = temp_token.substr(0, temp_token.find_first_of("("));
	// cout << "synassigntoken:" << syn_assign_token << endl;
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&syn_if_token);
	// syn-assign must be of type assign
	if (declared_var_names.count(syn_if_token) != 0) {
		// syn-assign token is declared
		syn_if_token_type = declared_var_names.at(syn_if_token);
	}
	if (syn_if_token_type != TYPE_DESIGN_ENTITY_IF) {
		result = false;
		return result;
	}
	STRING first_arg;
	STRING second_arg;
	STRING third_arg;
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	third_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&third_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// cout << "3rd arg:" << third_arg << "|" << endl;
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsEntRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	if (second_arg.compare("_") != 0) {
		result = false;
		return result;
	}
	if (third_arg.compare("_") != 0) {
		result = false;
		return result;
	}
	return result;
}

BOOLEAN QueryRules::IsSynWhile(STRING token, STRING_STRING_MAP declared_var_names) {
	BOOLEAN result = true;
	STRING temp_token = token;
	STRING syn_while_token;
	STRING syn_while_token_type = "none";
	syn_while_token = temp_token.substr(0, temp_token.find_first_of("("));
	// cout << "synassigntoken:" << syn_assign_token << endl;
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&syn_while_token);
	// syn-assign must be of type assign
	if (declared_var_names.count(syn_while_token) != 0) {
		// syn-assign token is declared
		syn_while_token_type = declared_var_names.at(syn_while_token);
	}
	if (syn_while_token_type != TYPE_DESIGN_ENTITY_WHILE) {
		result = false;
		return result;
	}
	STRING first_arg;
	STRING second_arg;
	temp_token.erase(0, temp_token.find_first_of("("));
	if (temp_token.front() != '(' || temp_token.back() != ')') {
		result = false;
		return result;
	}
	temp_token.erase(0, 1); // Erase opening bracket away
	first_arg = temp_token.substr(0, temp_token.find_first_of(","));
	temp_token.erase(0, temp_token.find_first_of(",") + 1);
	// cout << "TT:" << temp_token << endl;
	second_arg = temp_token.substr(0, temp_token.length() - 1);

	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_arg);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_arg);
	// cout << "First arg:" << first_arg << "|" << endl;
	// cout << "2nd arg:" << second_arg << "|" << endl;
	// cout << "3rd arg:" << third_arg << "|" << endl;
	STRING first_arg_type = "none";
	if (declared_var_names.count(first_arg) == 1) {
		first_arg_type = declared_var_names.at(first_arg);
	}
	if (!IsEntRef(first_arg, first_arg_type)) {
		result = false;
		return result;
	}
	if (second_arg.compare("_") != 0) {
		result = false;
		return result;
	}
	return result;
}