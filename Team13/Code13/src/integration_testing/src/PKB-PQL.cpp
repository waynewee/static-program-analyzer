//#include "catch.hpp"
//using namespace std;
//
//#include "pkb/PKB.h"
//#include "pql/PQLEvaluator.h"
//
//PKB pkb = PKB();
//PQLEvaluator pql = PQLEvaluator();
//
//TEST_CASE("Follows") {
//	pkb.GetRelationManager()->AddFollows(1, 2);
//	pkb.GetRelationManager()->AddFollows(3, 4);
//
//	pkb.GetRelationManager()->AddFollows(7, 8);
//	pkb.GetRelationManager()->AddFollows(8, 9);
//	pkb.GetRelationManager()->AddFollows(9, 10);
//	
//	// NO user-declared entities
//	REQUIRE(pkb.GetRelationManager()->IsFollows(1, 2) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "1", "2"));
//	REQUIRE(pkb.GetRelationManager()->IsFollows(-1, 2) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "_", "2"));
//	REQUIRE(pkb.GetRelationManager()->IsFollows(1, -1) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "1", "_"));
//	REQUIRE(pkb.GetRelationManager()->IsFollows(1, -1) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "_", "_"));
//
//	// ONE user-declared entities
//	unordered_set<string> pkbOneResult = unordered_set<string>();
//	unordered_set<string> pqlOneResult = unordered_set<string>();
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetFollows(1)));
//	pqlOneResult = pql.evaluateOneDeclaredSet(TYPE_COND_FOLLOWS, "1");
//	REQUIRE(pkbOneResult == pqlOneResult);
//	
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetFollows(-1)));
//	pqlOneResult = pql.evaluateOneDeclaredSet(TYPE_COND_FOLLOWS, "_");
//	REQUIRE(pkbOneResult == pqlOneResult);
//	
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetInverseFollows(1)));
//	pqlOneResult = pql.evaluateInverseOneDeclaredSet(TYPE_COND_FOLLOWS, "1");
//	REQUIRE(pkbOneResult == pqlOneResult);
//	
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetInverseFollows(-1)));
//	pqlOneResult = pql.evaluateInverseOneDeclaredSet(TYPE_COND_FOLLOWS, "_");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	// TWO user-declared entities
//	unordered_set<vector<string>*> pkbTwoResult = pql.convertSet(*(pkb.GetRelationManager()->GetAllFollows()));
//	unordered_set<vector<string>*> pqlTwoResult = pql.evaluateTwoDeclaredSet(TYPE_COND_FOLLOWS);
//	REQUIRE(pkbOneResult == pqlOneResult);
//}
//
//TEST_CASE("FollowsT") {
//	pkb.GetRelationManager()->AddFollowsStar(1, 2);
//	pkb.GetRelationManager()->AddFollowsStar(3, 4);
//
//	pkb.GetRelationManager()->AddFollowsStar(7, 8);
//	pkb.GetRelationManager()->AddFollowsStar(8, 9);
//	pkb.GetRelationManager()->AddFollowsStar(9, 10);
//
//	pkb.GetRelationManager()->AddFollowsStar(7, 9);
//	pkb.GetRelationManager()->AddFollowsStar(7, 10);
//	pkb.GetRelationManager()->AddFollowsStar(8, 10);
//
//	// NO user-declared entities
//	REQUIRE(pkb.GetRelationManager()->IsFollowsStar(1, 2) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "1", "2"));
//	REQUIRE(pkb.GetRelationManager()->IsFollowsStar(-1, 2) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "_", "2"));
//	REQUIRE(pkb.GetRelationManager()->IsFollowsStar(1, -1) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "1", "_"));
//	REQUIRE(pkb.GetRelationManager()->IsFollowsStar(1, -1) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "_", "_"));
//
//	// ONE user-declared entities
//	unordered_set<string> pkbOneResult = unordered_set<string>();
//	unordered_set<string> pqlOneResult = unordered_set<string>();
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetFollowsStars(1)));
//	pqlOneResult = pql.evaluateOneDeclaredSet(TYPE_COND_FOLLOWS_T, "1");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetFollowsStars(-1)));
//	pqlOneResult = pql.evaluateOneDeclaredSet(TYPE_COND_FOLLOWS_T, "_");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetInverseFollowsStars(1)));
//	pqlOneResult = pql.evaluateInverseOneDeclaredSet(TYPE_COND_FOLLOWS_T, "1");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetInverseFollowsStars(-1)));
//	pqlOneResult = pql.evaluateInverseOneDeclaredSet(TYPE_COND_FOLLOWS_T, "_");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	// TWO user-declared entities
//	unordered_set<vector<string>*> pkbTwoResult = pql.convertSet(*(pkb.GetRelationManager()->GetAllFollowsStars()));
//	unordered_set<vector<string>*> pqlTwoResult = pql.evaluateTwoDeclaredSet(TYPE_COND_FOLLOWS_T);
//	REQUIRE(pkbOneResult == pqlOneResult);
//}
//
//TEST_CASE("Parent") {
//	pkb.GetRelationManager()->AddParent(1, 2);
//	pkb.GetRelationManager()->AddParent(3, 4);
//
//	pkb.GetRelationManager()->AddParent(7, 8);
//	pkb.GetRelationManager()->AddParent(8, 9);
//	pkb.GetRelationManager()->AddParent(9, 10);
//
//	pkb.GetRelationManager()->AddParent(7, 9);
//	pkb.GetRelationManager()->AddParent(7, 10);
//	pkb.GetRelationManager()->AddParent(8, 10);
//
//	// NO user-declared entities
//	REQUIRE(pkb.GetRelationManager()->IsParent(1, 2) == pql.evaluateNoUserDeclaredSet(TYPE_COND_PARENT, "1", "2"));
//	REQUIRE(pkb.GetRelationManager()->IsParent(-1, 2) == pql.evaluateNoUserDeclaredSet(TYPE_COND_PARENT, "_", "2"));
//	REQUIRE(pkb.GetRelationManager()->IsParent(1, -1) == pql.evaluateNoUserDeclaredSet(TYPE_COND_PARENT, "1", "_"));
//	REQUIRE(pkb.GetRelationManager()->IsParent(1, -1) == pql.evaluateNoUserDeclaredSet(TYPE_COND_PARENT, "_", "_"));
//
//	// ONE user-declared entities
//	unordered_set<string> pkbOneResult = unordered_set<string>();
//	unordered_set<string> pqlOneResult = unordered_set<string>();
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetParents(1)));
//	pqlOneResult = pql.evaluateOneDeclaredSet(TYPE_COND_PARENT, "1");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetParents(-1)));
//	pqlOneResult = pql.evaluateOneDeclaredSet(TYPE_COND_PARENT, "_");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetInverseParents(1)));
//	pqlOneResult = pql.evaluateInverseOneDeclaredSet(TYPE_COND_PARENT, "1");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetInverseParents(-1)));
//	pqlOneResult = pql.evaluateInverseOneDeclaredSet(TYPE_COND_PARENT, "_");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	// TWO user-declared entities
//	unordered_set<vector<string>*> pkbTwoResult = pql.convertSet(*(pkb.GetRelationManager()->GetAllParents()));
//	unordered_set<vector<string>*> pqlTwoResult = pql.evaluateTwoDeclaredSet(TYPE_COND_PARENT);
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//}
//
//TEST_CASE("ParentT") {
//	pkb.GetRelationManager()->AddParentStar(1, 2);
//	pkb.GetRelationManager()->AddParentStar(3, 4);
//
//	pkb.GetRelationManager()->AddParentStar(7, 8);
//	pkb.GetRelationManager()->AddParentStar(8, 9);
//	pkb.GetRelationManager()->AddParentStar(9, 10);
//
//	pkb.GetRelationManager()->AddParentStar(7, 9);
//	pkb.GetRelationManager()->AddParentStar(7, 10);
//	pkb.GetRelationManager()->AddParentStar(8, 10);
//
//	// NO user-declared entities
//	REQUIRE(pkb.GetRelationManager()->IsParentStar(1, 2) == pql.evaluateNoUserDeclaredSet(TYPE_COND_PARENT_T, "1", "2"));
//	REQUIRE(pkb.GetRelationManager()->IsParentStar(-1, 2) == pql.evaluateNoUserDeclaredSet(TYPE_COND_PARENT_T, "_", "2"));
//	REQUIRE(pkb.GetRelationManager()->IsParentStar(1, -1) == pql.evaluateNoUserDeclaredSet(TYPE_COND_PARENT_T, "1", "_"));
//	REQUIRE(pkb.GetRelationManager()->IsParentStar(1, -1) == pql.evaluateNoUserDeclaredSet(TYPE_COND_PARENT_T, "_", "_"));
//
//	// ONE user-declared entities
//	unordered_set<string> pkbOneResult = unordered_set<string>();
//	unordered_set<string> pqlOneResult = unordered_set<string>();
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetParentStars(1)));
//	pqlOneResult = pql.evaluateOneDeclaredSet(TYPE_COND_PARENT_T, "1");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetParentStars(-1)));
//	pqlOneResult = pql.evaluateOneDeclaredSet(TYPE_COND_PARENT_T, "_");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetInverseParentStars(1)));
//	pqlOneResult = pql.evaluateInverseOneDeclaredSet(TYPE_COND_PARENT_T, "1");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetInverseParentStars(-1)));
//	pqlOneResult = pql.evaluateInverseOneDeclaredSet(TYPE_COND_PARENT_T, "_");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	// TWO user-declared entities
//	unordered_set<vector<string>*> pkbTwoResult = pql.convertSet(*(pkb.GetRelationManager()->GetAllParentStars()));
//	unordered_set<vector<string>*> pqlTwoResult = pql.evaluateTwoDeclaredSet(TYPE_COND_PARENT_T);
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//}
//
//TEST_CASE("UsesS") {
//	/*
//	string* var = new string("v");
//	pkb.GetRelationManager()->AddStmtUses(1, var);
//	pkb.GetRelationManager()->AddStmtUses(3, var);
//	pkb.GetRelationManager()->AddStmtUses(7, var);
//
//	// ONE user-declared entities
//	unordered_set<string> pkbOneResult = unordered_set<string>();
//	unordered_set<string> pqlOneResult = unordered_set<string>();
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetStmtUses(1)));
//	pqlOneResult = pql.evaluateOneDeclaredSet(TYPE_COND_USES_S, "v");
//	REQUIRE(pkbOneResult == pqlOneResult);
//	*/
//
//	/*
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetParentStars(-1)));
//	pqlOneResult = pql.evaluateOneDeclaredSet(TYPE_COND_PARENT_T, "_");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetInverseParentStars(1)));
//	pqlOneResult = pql.evaluateInverseOneDeclaredSet(TYPE_COND_PARENT_T, "1");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetInverseParentStars(-1)));
//	pqlOneResult = pql.evaluateInverseOneDeclaredSet(TYPE_COND_PARENT_T, "_");
//	REQUIRE(pkbOneResult == pqlOneResult);
//
//	// TWO user-declared entities
//	unordered_set<vector<string>*> pkbTwoResult = pql.convertSet(*(pkb.GetRelationManager()->GetAllParentStars()));
//	unordered_set<vector<string>*> pqlTwoResult = pql.evaluateTwoDeclaredSet(TYPE_COND_PARENT_T);
//	REQUIRE(pkbOneResult == pqlOneResult);
//	*/
//}
//
//TEST_CASE("MODIFIES") {
//
//}
//
//TEST_CASE("PATTERN") {
//
//}