#include "catch.hpp"
using namespace std;

#include "pkb/PKB.h"
#include "pql/PQLEvaluator.h"

PKB pkb_st = PKB();
PQLEvaluator pql_st = PQLEvaluator();

void isSameResult(unordered_set<vector<string>*> pkb_two_result, unordered_set<vector<string>*> pql_two_result) {
	bool is_found = false;

	for (vector<string>* entry1 : pkb_two_result) {
		for (vector<string>* entry2 : pql_two_result) {
			if (*entry1 == *entry2) {
				is_found = true;
			}
		}
	}

	REQUIRE(is_found == true);
}

TEST_CASE("Follows") {
	pkb_st.GetRelationManager().AddFollows(1, 2);
	pkb_st.GetRelationManager().AddFollows(3, 4);

	pkb_st.GetRelationManager().AddFollows(7, 8);
	pkb_st.GetRelationManager().AddFollows(8, 9);
	pkb_st.GetRelationManager().AddFollows(9, 10);
	
	// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsFollows(1, 2) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "1", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsFollows(-1, 2) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "_", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsFollows(1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsFollows(1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "_", "_"));

	REQUIRE(pkb_st.GetRelationManager().IsFollows(-1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "", ""));
	REQUIRE(pkb_st.GetRelationManager().IsFollows(-1, 2) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsFollows(1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "1", ""));

	// ONE user-declared entities
	unordered_set<string> pkb_one_result = unordered_set<string>();
	unordered_set<string> pql_one_result = unordered_set<string>();

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetFollows(1));
	pql_one_result = pql_st.EvaluateOneDeclaredSet(TYPE_COND_FOLLOWS, "1");
	REQUIRE(pkb_one_result == pql_one_result);
	
	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetFollows(-1));
	pql_one_result = pql_st.EvaluateOneDeclaredSet(TYPE_COND_FOLLOWS, "_");
	REQUIRE(pkb_one_result == pql_one_result);
	
	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseFollows(1));
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_FOLLOWS, "1");
	REQUIRE(pkb_one_result == pql_one_result);
	
	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseFollows(-1));
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_FOLLOWS, "_");
	REQUIRE(pkb_one_result == pql_one_result);

	// TWO user-declared entities
	unordered_set<vector<string>*> pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllFollows());
	unordered_set<vector<string>*> pql_two_result = pql_st.EvaluateTwoDeclaredSet(TYPE_COND_FOLLOWS);
	isSameResult(pkb_two_result, pql_two_result);
}

TEST_CASE("FollowsT") {
	pkb_st.GetRelationManager().AddFollowsStar(1, 2);
	pkb_st.GetRelationManager().AddFollowsStar(3, 4);

	pkb_st.GetRelationManager().AddFollowsStar(7, 8);
	pkb_st.GetRelationManager().AddFollowsStar(8, 9);
	pkb_st.GetRelationManager().AddFollowsStar(9, 10);

	pkb_st.GetRelationManager().AddFollowsStar(7, 9);
	pkb_st.GetRelationManager().AddFollowsStar(7, 10);
	pkb_st.GetRelationManager().AddFollowsStar(8, 10);

	// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsFollowsStar(1, 2) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "1", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsFollowsStar(-1, 2) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "_", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsFollowsStar(1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsFollowsStar(1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "_", "_"));

	REQUIRE(pkb_st.GetRelationManager().IsFollowsStar(-1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "", ""));
	REQUIRE(pkb_st.GetRelationManager().IsFollowsStar(-1, 2) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsFollowsStar(1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "1", ""));

	// ONE user-declared entities
	unordered_set<string> pkb_one_result = unordered_set<string>();
	unordered_set<string> pql_one_result = unordered_set<string>();

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetFollowsStars(1));
	pql_one_result = pql_st.EvaluateOneDeclaredSet(TYPE_COND_FOLLOWS_T, "1");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetFollowsStars(-1));
	pql_one_result = pql_st.EvaluateOneDeclaredSet(TYPE_COND_FOLLOWS_T, "_");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseFollowsStars(1));
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_FOLLOWS_T, "1");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseFollowsStars(-1));
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_FOLLOWS_T, "_");
	REQUIRE(pkb_one_result == pql_one_result);

	// TWO user-declared entities
	unordered_set<vector<string>*> pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllFollowsStars());
	unordered_set<vector<string>*> pql_two_result = pql_st.EvaluateTwoDeclaredSet(TYPE_COND_FOLLOWS_T);
	isSameResult(pkb_two_result, pql_two_result);
}

TEST_CASE("Parent") {
	pkb_st.GetRelationManager().AddParent(1, 2);
	pkb_st.GetRelationManager().AddParent(3, 4);

	pkb_st.GetRelationManager().AddParent(7, 8);
	pkb_st.GetRelationManager().AddParent(8, 9);
	pkb_st.GetRelationManager().AddParent(9, 10);

	pkb_st.GetRelationManager().AddParent(7, 9);
	pkb_st.GetRelationManager().AddParent(7, 10);
	pkb_st.GetRelationManager().AddParent(8, 10);

	// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsParent(1, 2) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_PARENT, "1", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsParent(-1, 2) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_PARENT, "_", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsParent(1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_PARENT, "1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsParent(1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_PARENT, "_", "_"));

	REQUIRE(pkb_st.GetRelationManager().IsParent(-1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_PARENT, "", ""));
	REQUIRE(pkb_st.GetRelationManager().IsParent(-1, 2) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_PARENT, "", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsParent(1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_PARENT, "1", ""));

	// ONE user-declared entities
	unordered_set<string> pkb_one_result = unordered_set<string>();
	unordered_set<string> pql_one_result = unordered_set<string>();

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetParents(1));
	pql_one_result = pql_st.EvaluateOneDeclaredSet(TYPE_COND_PARENT, "1");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetParents(-1));
	pql_one_result = pql_st.EvaluateOneDeclaredSet(TYPE_COND_PARENT, "_");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseParents(1));
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_PARENT, "1");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseParents(-1));
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_PARENT, "_");
	REQUIRE(pkb_one_result == pql_one_result);

	// TWO user-declared entities
	unordered_set<vector<string>*> pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllParents());
	unordered_set<vector<string>*> pql_two_result = pql_st.EvaluateTwoDeclaredSet(TYPE_COND_PARENT);
	isSameResult(pkb_two_result, pql_two_result);

}

TEST_CASE("ParentT") {
	pkb_st.GetRelationManager().AddParentStar(1, 2);
	pkb_st.GetRelationManager().AddParentStar(3, 4);

	pkb_st.GetRelationManager().AddParentStar(7, 8);
	pkb_st.GetRelationManager().AddParentStar(8, 9);
	pkb_st.GetRelationManager().AddParentStar(9, 10);

	pkb_st.GetRelationManager().AddParentStar(7, 9);
	pkb_st.GetRelationManager().AddParentStar(7, 10);
	pkb_st.GetRelationManager().AddParentStar(8, 10);

	// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsParentStar(1, 2) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_PARENT_T, "1", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsParentStar(-1, 2) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_PARENT_T, "_", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsParentStar(1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_PARENT_T, "1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsParentStar(1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_PARENT_T, "_", "_"));

	REQUIRE(pkb_st.GetRelationManager().IsParentStar(-1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_PARENT_T, "", ""));
	REQUIRE(pkb_st.GetRelationManager().IsParentStar(-1, 2) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_PARENT_T, "", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsParentStar(1, -1) == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_PARENT_T, "1", ""));

	// ONE user-declared entities
	unordered_set<string> pkb_one_result = unordered_set<string>();
	unordered_set<string> pql_one_result = unordered_set<string>();

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetParentStars(1));
	pql_one_result = pql_st.EvaluateOneDeclaredSet(TYPE_COND_PARENT_T, "1");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetParentStars(-1));
	pql_one_result = pql_st.EvaluateOneDeclaredSet(TYPE_COND_PARENT_T, "_");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseParentStars(1));
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_PARENT_T, "1");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseParentStars(-1));
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_PARENT_T, "_");
	REQUIRE(pkb_one_result == pql_one_result);

	// TWO user-declared entities
	unordered_set<vector<string>*> pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllParentStars());
	unordered_set<vector<string>*> pql_two_result = pql_st.EvaluateTwoDeclaredSet(TYPE_COND_PARENT_T);
	isSameResult(pkb_two_result, pql_two_result);

}

TEST_CASE("UsesS") {
	pkb_st.GetRelationManager().AddStmtUses(1, "v");
	pkb_st.GetRelationManager().AddStmtUses(3, "v");
	pkb_st.GetRelationManager().AddStmtUses(7, "v");

	//// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsStmtUses(1, "v") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_USES_S, "1", "v"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtUses(1, "") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_USES_S, "1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtUses(1, "v") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_USES_S, "1", ""));
	REQUIRE(pkb_st.GetRelationManager().IsStmtUses(-1, "") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_USES_S, "", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtUses(-1, "v") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_USES_S, "", "v"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtUses(-1, "") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_USES_S, "", ""));

	unordered_set<string> pkb_one_result = unordered_set<string>();
	unordered_set<string> pql_one_result = unordered_set<string>();

	//// ONE user-declared entities
	pkb_one_result = pkb_st.GetRelationManager().GetStmtUses(1);
	pql_one_result = pql_st.EvaluateOneDeclaredSet(TYPE_COND_USES_S, "1");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseStmtUses("v"));
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_USES_S, "v");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseStmtUses(""));
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_USES_S, "_");
	REQUIRE(pkb_one_result == pql_one_result);

	//// TWO user-declared entities
	unordered_set<vector<string>*> pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllStmtUses());
	unordered_set<vector<string>*> pql_two_result = pql_st.EvaluateTwoDeclaredSet(TYPE_COND_USES_S);
	isSameResult(pkb_two_result, pql_two_result);
}

TEST_CASE("UsesP") {
	pkb_st.GetRelationManager().AddProcUses("v1", "v2");
	pkb_st.GetRelationManager().AddProcUses("v1", "v2");
	pkb_st.GetRelationManager().AddProcUses("v1", "v2");

	//// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsProcUses("v1", "v2") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_USES_P, "v1", "v2"));
	REQUIRE(pkb_st.GetRelationManager().IsProcUses("v1", "") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_USES_P, "v1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsProcUses("v1", "") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_USES_P, "v1", ""));
	REQUIRE(pkb_st.GetRelationManager().IsProcUses("", "") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_USES_P, "", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsProcUses("", "v2") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_USES_P, "", "v2"));
	REQUIRE(pkb_st.GetRelationManager().IsProcUses("", "") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_USES_P, "", ""));

	unordered_set<string> pkb_one_result = unordered_set<string>();
	unordered_set<string> pql_one_result = unordered_set<string>();

	//// ONE user-declared entities
	pkb_one_result = pkb_st.GetRelationManager().GetProcUses("v1");
	pql_one_result = pql_st.EvaluateOneDeclaredSet(TYPE_COND_USES_P, "v1");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pkb_st.GetRelationManager().GetInverseProcUses("v2");
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_USES_P, "v2");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pkb_st.GetRelationManager().GetInverseProcUses("");
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_USES_P, "_");
	REQUIRE(pkb_one_result == pql_one_result);

	//// TWO user-declared entities
	unordered_set<vector<string>*> pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllProcUses());
	unordered_set<vector<string>*> pql_two_result = pql_st.EvaluateTwoDeclaredSet(TYPE_COND_USES_P);
	isSameResult(pkb_two_result, pql_two_result);
}

TEST_CASE("ModifiesS") {
	pkb_st.GetRelationManager().AddStmtModifies(1, "v");
	pkb_st.GetRelationManager().AddStmtModifies(3, "v");
	pkb_st.GetRelationManager().AddStmtModifies(7, "v");

	//// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsStmtModifies(1, "v") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_MODIFIES_S, "1", "v"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtModifies(1, "") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_MODIFIES_S, "1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtModifies(1, "v") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_MODIFIES_S, "1", ""));
	REQUIRE(pkb_st.GetRelationManager().IsStmtModifies(-1, "") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_MODIFIES_S, "", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtModifies(-1, "v") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_MODIFIES_S, "", "v"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtModifies(-1, "") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_MODIFIES_S, "", ""));

	unordered_set<string> pkb_one_result = unordered_set<string>();
	unordered_set<string> pql_one_result = unordered_set<string>();

	//// ONE user-declared entities
	pkb_one_result = pkb_st.GetRelationManager().GetStmtModifies(1);
	pql_one_result = pql_st.EvaluateOneDeclaredSet(TYPE_COND_MODIFIES_S, "1");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseStmtModifies("v"));
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_MODIFIES_S, "v");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseStmtModifies(""));
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_MODIFIES_S, "_");
	REQUIRE(pkb_one_result == pql_one_result);

	//// TWO user-declared entities
	unordered_set<vector<string>*> pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllStmtModifies());
	unordered_set<vector<string>*> pql_two_result = pql_st.EvaluateTwoDeclaredSet(TYPE_COND_MODIFIES_S);
	isSameResult(pkb_two_result, pql_two_result);
}

TEST_CASE("ModifiesP") {
	pkb_st.GetRelationManager().AddProcModifies("v1", "v2");
	pkb_st.GetRelationManager().AddProcModifies("v1", "v2");
	pkb_st.GetRelationManager().AddProcModifies("v1", "v2");

	//// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsProcModifies("v1", "v2") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_MODIFIES_P, "v1", "v2"));
	REQUIRE(pkb_st.GetRelationManager().IsProcModifies("v1", "") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_MODIFIES_P, "v1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsProcModifies("v1", "") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_MODIFIES_P, "v1", ""));
	REQUIRE(pkb_st.GetRelationManager().IsProcModifies("", "") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_MODIFIES_P, "", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsProcModifies("", "v2") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_MODIFIES_P, "", "v2"));
	REQUIRE(pkb_st.GetRelationManager().IsProcModifies("", "") == pql_st.EvaluateNoUserDeclaredSet(TYPE_COND_MODIFIES_P, "", ""));

	unordered_set<string> pkb_one_result = unordered_set<string>();
	unordered_set<string> pql_one_result = unordered_set<string>();

	//// ONE user-declared entities
	pkb_one_result = pkb_st.GetRelationManager().GetProcModifies("v1");
	pql_one_result = pql_st.EvaluateOneDeclaredSet(TYPE_COND_MODIFIES_P, "v1");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pkb_st.GetRelationManager().GetInverseProcModifies("v2");
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_MODIFIES_P, "v2");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pkb_st.GetRelationManager().GetInverseProcModifies("");
	pql_one_result = pql_st.EvaluateInverseOneDeclaredSet(TYPE_COND_MODIFIES_P, "_");
	REQUIRE(pkb_one_result == pql_one_result);

	//// TWO user-declared entities
	unordered_set<vector<string>*> pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllProcModifies());
	unordered_set<vector<string>*> pql_two_result = pql_st.EvaluateTwoDeclaredSet(TYPE_COND_MODIFIES_P);
	isSameResult(pkb_two_result, pql_two_result);
}