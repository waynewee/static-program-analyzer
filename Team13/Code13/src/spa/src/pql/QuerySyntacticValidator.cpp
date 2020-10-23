#include "QuerySyntacticValidator.h"
#include <regex>
#include <iostream>
#include "WhitespaceHandler.h"

const STRING_SET valid_design_entities = { TYPE_DESIGN_ENTITY_STMT , TYPE_DESIGN_ENTITY_READ , TYPE_DESIGN_ENTITY_PRINT ,
											TYPE_DESIGN_ENTITY_CALL , TYPE_DESIGN_ENTITY_WHILE , TYPE_DESIGN_ENTITY_IF ,
											TYPE_DESIGN_ENTITY_ASSIGN , TYPE_DESIGN_ENTITY_VARIABLE , TYPE_DESIGN_ENTITY_CONSTANT ,
											TYPE_DESIGN_ENTITY_PROG_LINE , TYPE_DESIGN_ENTITY_PROCEDURE };
const STRING_SET valid_rel_ref_names = { TYPE_COND_FOLLOWS , TYPE_COND_FOLLOWS_T , TYPE_COND_FOLLOWS_STAR , TYPE_COND_PARENT ,
										TYPE_COND_PARENT_T , TYPE_COND_PARENT_STAR , TYPE_COND_USES , TYPE_COND_USES_S ,
										TYPE_COND_USES_P , TYPE_COND_MODIFIES ,  TYPE_COND_MODIFIES_S , TYPE_COND_MODIFIES_P ,
										TYPE_COND_CALLS , TYPE_COND_CALLS_T , TYPE_COND_CALLS_STAR , TYPE_COND_NEXT ,
										TYPE_COND_NEXT_T , TYPE_COND_NEXT_STAR , TYPE_COND_AFFECTS , TYPE_COND_AFFECTS_T ,
										TYPE_COND_AFFECTS_STAR };
const STRING_SET valid_stmt_ref_synonyms = { TYPE_DESIGN_ENTITY_STMT , TYPE_DESIGN_ENTITY_READ , TYPE_DESIGN_ENTITY_PRINT ,
											TYPE_DESIGN_ENTITY_CALL , TYPE_DESIGN_ENTITY_WHILE , TYPE_DESIGN_ENTITY_IF ,
											TYPE_DESIGN_ENTITY_ASSIGN };
const STRING_SET valid_ent_ref_synonyms = { TYPE_DESIGN_ENTITY_VARIABLE, TYPE_DESIGN_ENTITY_PROCEDURE };
const STRING_SET valid_line_ref_synonyms = { TYPE_DESIGN_ENTITY_PROG_LINE };
const STRING_SET valid_attr_names = { TYPE_ATTRNAME_PROCNAME , TYPE_ATTRNAME_VARNAME , TYPE_ATTRNAME_VALUE , TYPE_ATTRNAME_STMT };
const STRING_SET valid_stmt_attr_types = { TYPE_DESIGN_ENTITY_CALL , TYPE_DESIGN_ENTITY_READ , TYPE_DESIGN_ENTITY_PRINT,
										TYPE_DESIGN_ENTITY_STMT, TYPE_DESIGN_ENTITY_WHILE, TYPE_DESIGN_ENTITY_IF,
										TYPE_DESIGN_ENTITY_ASSIGN };
const STRING_SET valid_procname_attr_types = { TYPE_DESIGN_ENTITY_PROCEDURE , TYPE_DESIGN_ENTITY_CALL };
const STRING_SET valid_varname_attr_types = { TYPE_DESIGN_ENTITY_VARIABLE , TYPE_DESIGN_ENTITY_READ , TYPE_DESIGN_ENTITY_PRINT };
const STRING_SET valid_value_attr_types = { TYPE_DESIGN_ENTITY_CONSTANT };

QuerySyntacticValidator::QuerySyntacticValidator() {

}

bool QuerySyntacticValidator::ValidateDeclaration(string token) {
	bool result = true;
	string temp_token = token;
	string sub_token;
	string comma = ",";
	size_t pos = 0;
	bool first_pass = true;
	if (temp_token.find(comma) != string::npos) {
		while ((pos = temp_token.find(comma)) != string::npos) {
			WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
			sub_token = temp_token.substr(0, temp_token.find_first_of(","));
			WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&sub_token);
			if (first_pass) {
				// Here should have the design-entity and synonym
				string supposed_design_entity = sub_token.substr(0, sub_token.find_first_of(" "));
				//cout << "sde:" << supposed_design_entity << "|" << endl;
				if (valid_design_entities.count(supposed_design_entity) == 0) {
					result = false;
					return result;
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
				return result;
			}
			// Erase the validated segment from the temp_token
			// cout << "TEMPTOKEN:" << temp_token << endl;
			WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
			temp_token.erase(0, temp_token.find_first_of(",") + 1);
		}
		// Passed the last comma, this is checking for last synonym.
		// cout << "Last value:" << temp_token << endl;
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		if (!IsSynonym(temp_token)) {
			result = false;
			return result;
		}
	}
	else {
		string supposed_design_entity = temp_token.substr(0, temp_token.find_first_of(" "));
		if (valid_design_entities.count(supposed_design_entity) == 0) {
			result = false;
			return result;
		}
		temp_token.erase(0, temp_token.find_first_of(" "));
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&temp_token);
		if (!IsSynonym(temp_token)) {
			result = false;
			return result;
		}
	}
	return result;
}

bool QuerySyntacticValidator::ValidateSuchThatClause(string token) {
	bool result = true;
	string temp_token = token;
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
	string suchthat_token = temp_token.substr(0, second_whitespace_pos);
	if (suchthat_token.compare(TYPE_SUCH_THAT_CLAUSE) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of(" ") + 1); // erase such
	temp_token.erase(0, temp_token.find_first_of(" ")); // erase that
	WhitespaceHandler::TrimLeadingWhitespaces(&temp_token);
	string rel_ref_token = temp_token;

	if (!IsRelRef(rel_ref_token)) {
		result = false;
		return result;
	}

	//cout << "relcond:" << rel_cond_token << endl;
	return result;
}

bool QuerySyntacticValidator::IsRelRef(string token) {
	bool result = true;
	string temp_token = token;
	string relref_token_type = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&relref_token_type);
	string first_arg;
	string second_arg;
	if (valid_rel_ref_names.count(relref_token_type) == 0) {
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

	if (!IsValidRelRefArgument(first_arg)) {
		result = false;
		return result;
	}
	if (!IsValidRelRefArgument(second_arg)) {
		result = false;
		return result;
	}
	return result;
}


bool QuerySyntacticValidator::IsValidRelRefArgument(string token) {
	// either synonym, underscore, integer, or "IDENT"
	bool result = false;

	// if its either, true, and continue
	if (IsSynonym(token)) {
		result = true;
		return result;
	}
	if (token.compare("_") == 0) {
		result = true;
		return result;
	}
	if (IsInteger(token)) {
		result = true;
		return result;
	}
	if (token.front() == '\"' && token.back() == '\"') {
		string ident = token.substr(1, token.length() - 2);
		// cout << "internal ident:" << ident << endl;
		if (IsIdent(ident)) {
			result = true;
			return result;
		}
	}
	return result;
}

bool QuerySyntacticValidator::ValidatePatternClause(string token, STRING_STRING_MAP declared_var_names) {
	bool result = true;
	string temp_token = token;
	string pattern_token = temp_token.substr(0, temp_token.find_first_of(" "));
	if (pattern_token.compare(TYPE_COND_PATTERN) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of(" "));
	WhitespaceHandler::TrimLeadingWhitespaces(&temp_token);
	string pattern_type_token = temp_token.substr(0, temp_token.find_first_of("("));
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&pattern_type_token);
	if (!IsSynonym(pattern_type_token)) {
		result = false;
		return result;
	}

	string first_arg;
	string second_arg;
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

	if (!IsValidRelRefArgument(first_arg)) {
		result = false;
		return result;
	}

	string pattern_type_token_type = "none";
	if (declared_var_names.count(pattern_type_token) != 0) {
		pattern_type_token_type = declared_var_names.at(pattern_type_token);
	}

	if (pattern_type_token_type.compare(TYPE_DESIGN_ENTITY_IF) == 0 || pattern_type_token_type.compare(TYPE_DESIGN_ENTITY_WHILE) == 0) {
		if (second_arg.compare("_") != 0) {
			result = false;
			return result;
		}
	}
	if (pattern_type_token_type.compare(TYPE_DESIGN_ENTITY_IF) == 0) {
		temp_token.erase(0, temp_token.find_first_of(",") + 1);
		string third_arg = temp_token.substr(0, temp_token.length() - 1);
		WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&third_arg);
		if (third_arg.compare("_") != 0) {
			result = false;
			return result;
		}
	}
	return result;
}

bool QuerySyntacticValidator::ValidateWithClause(string token) {
	bool result = true;
	string temp_token = token;
	string with_token = temp_token.substr(0, temp_token.find_first_of(" "));
	if (with_token.compare(TYPE_WITH_CLAUSE) != 0) {
		result = false;
		return result;
	}
	temp_token.erase(0, temp_token.find_first_of(" "));
	WhitespaceHandler::TrimLeadingWhitespaces(&temp_token);
	string attr_cond_token = temp_token;

	string first_ref = attr_cond_token.substr(0, attr_cond_token.find_first_of("="));
	string second_ref = attr_cond_token.substr(attr_cond_token.find_first_of("=") + 1, attr_cond_token.length());
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&first_ref);
	WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&second_ref);

	//if (!IsAttrCond(attr_cond_token, declared_var_names)) {
	//	result = false;
	// }
	if (!IsRef(first_ref)) {
		result = false;
		return result;
	}
	if (!IsRef(second_ref)) {
		result = false;
		return result;
	}
	return result;
}

bool QuerySyntacticValidator::IsIdent(string token) {
	bool result = false;
	regex expr("[A-Za-z][a-zA-Z_0-9]*");
	if (regex_match(token, expr)) {
		result = true;
	}
	return result;
}

bool QuerySyntacticValidator::IsName(string token) {
	bool result = false;
	regex expr("[A-Za-z][a-zA-Z_0-9]*");
	if (regex_match(token, expr)) {
		result = true;
	}
	return result;
}

bool QuerySyntacticValidator::IsInteger(string token) {
	bool result = false;
	regex expr("[0-9]*");
	if (regex_match(token, expr)) {
		result = true;
	}
	return result;
}

bool QuerySyntacticValidator::IsSynonym(string token) {
	bool result = false;
	if (IsIdent(token)) {
		result = true;
	}
	return result;
}

bool QuerySyntacticValidator::IsAttrRef(string token) {
	bool result = true;
	size_t pos_of_fullstop = token.find(".");
	string synonym_token = token.substr(0, pos_of_fullstop);
	string attr_name_token = token.substr(pos_of_fullstop + 1, token.length() - 1);
	if (!IsSynonym(synonym_token)) {
		result = false;
		return result;
	}
	if (valid_attr_names.count(attr_name_token) == 0) {
		result = false;
		return result;
	}
	return result;
}

bool QuerySyntacticValidator::IsRef(string token) {
	bool result = false;
	if (token.front() == '\"' && token.back() == '\"') {
		// remember to ensure that what's inside the "" is already trimmed!
		string ident = token.substr(1, token.length() - 2);
		if (IsIdent(ident)) {
			result = true;
			return result;
		}
	}
	if (IsInteger(token)) {
		result = true;
		return result;
	}
	if (IsAttrRef(token)) {
		result = true;
		return result;
	}
	if (IsSynonym(token)) {
		result = true;
		return result;
	}
	return result;
}