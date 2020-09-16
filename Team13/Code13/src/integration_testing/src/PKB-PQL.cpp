#include "catch.hpp"
using namespace std;

#include "pkb/PKB.h"
#include "pql/PQLEvaluator.h"

PKB pkb = PKB();
PQLEvaluator pql = PQLEvaluator();

TEST_CASE("Follows") {
	pkb.GetRelationManager()->AddFollows(1, 2);
	pkb.GetRelationManager()->AddFollows(3, 4);

	pkb.GetRelationManager()->AddFollows(7, 8);
	pkb.GetRelationManager()->AddFollows(8, 9);
	pkb.GetRelationManager()->AddFollows(9, 10);
	
	// NO user-declared entities
	REQUIRE(pkb.GetRelationManager()->IsFollows(1, 2) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "1", "2"));
	REQUIRE(pkb.GetRelationManager()->IsFollows(-1, 2) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "_", "2"));
	REQUIRE(pkb.GetRelationManager()->IsFollows(1, -1) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "1", "_"));
	REQUIRE(pkb.GetRelationManager()->IsFollows(1, -1) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS, "_", "_"));

	// ONE user-declared entities
	unordered_set<string> pkbOneResult = unordered_set<string>();
	unordered_set<string> pqlOneResult = unordered_set<string>();

	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetFollows(1)));
	pqlOneResult = pql.evaluateOneDeclaredSet(TYPE_COND_FOLLOWS, "1");
	REQUIRE(pkbOneResult == pqlOneResult);
	
	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetFollows(-1)));
	pqlOneResult = pql.evaluateOneDeclaredSet(TYPE_COND_FOLLOWS, "_");
	REQUIRE(pkbOneResult == pqlOneResult);
	
	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetInverseFollows(1)));
	pqlOneResult = pql.evaluateInverseOneDeclaredSet(TYPE_COND_FOLLOWS, "1");
	REQUIRE(pkbOneResult == pqlOneResult);
	
	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetInverseFollows(-1)));
	pqlOneResult = pql.evaluateInverseOneDeclaredSet(TYPE_COND_FOLLOWS, "_");
	REQUIRE(pkbOneResult == pqlOneResult);

	// TWO user-declared entities
	unordered_set<vector<string>*> pkbTwoResult = pql.convertSet(*(pkb.GetRelationManager()->GetAllFollows()));
	unordered_set<vector<string>*> pqlTwoResult = pql.evaluateTwoDeclaredSet(TYPE_COND_FOLLOWS);
	REQUIRE(pkbOneResult == pqlOneResult);
}

TEST_CASE("FollowsT") {
	pkb.GetRelationManager()->AddFollowsStar(1, 2);
	pkb.GetRelationManager()->AddFollowsStar(3, 4);

	pkb.GetRelationManager()->AddFollowsStar(7, 8);
	pkb.GetRelationManager()->AddFollowsStar(8, 9);
	pkb.GetRelationManager()->AddFollowsStar(9, 10);

	pkb.GetRelationManager()->AddFollowsStar(7, 9);
	pkb.GetRelationManager()->AddFollowsStar(7, 10);
	pkb.GetRelationManager()->AddFollowsStar(8, 10);

	// NO user-declared entities
	REQUIRE(pkb.GetRelationManager()->IsFollowsStar(1, 2) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "1", "2"));
	REQUIRE(pkb.GetRelationManager()->IsFollowsStar(-1, 2) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "_", "2"));
	REQUIRE(pkb.GetRelationManager()->IsFollowsStar(1, -1) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "1", "_"));
	REQUIRE(pkb.GetRelationManager()->IsFollowsStar(1, -1) == pql.evaluateNoUserDeclaredSet(TYPE_COND_FOLLOWS_T, "_", "_"));

	// ONE user-declared entities
	unordered_set<string> pkbOneResult = unordered_set<string>();
	unordered_set<string> pqlOneResult = unordered_set<string>();

	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetFollowsStars(1)));
	pqlOneResult = pql.evaluateOneDeclaredSet(TYPE_COND_FOLLOWS_T, "1");
	REQUIRE(pkbOneResult == pqlOneResult);

	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetFollowsStars(-1)));
	pqlOneResult = pql.evaluateOneDeclaredSet(TYPE_COND_FOLLOWS_T, "_");
	REQUIRE(pkbOneResult == pqlOneResult);

	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetInverseFollowsStars(1)));
	pqlOneResult = pql.evaluateInverseOneDeclaredSet(TYPE_COND_FOLLOWS_T, "1");
	REQUIRE(pkbOneResult == pqlOneResult);

	pkbOneResult = pql.convertSet(*(pkb.GetRelationManager()->GetInverseFollowsStars(-1)));
	pqlOneResult = pql.evaluateInverseOneDeclaredSet(TYPE_COND_FOLLOWS_T, "_");
	REQUIRE(pkbOneResult == pqlOneResult);

	// TWO user-declared entities
	unordered_set<vector<string>*> pkbTwoResult = pql.convertSet(*(pkb.GetRelationManager()->GetAllFollowsStars()));
	unordered_set<vector<string>*> pqlTwoResult = pql.evaluateTwoDeclaredSet(TYPE_COND_FOLLOWS_T);
	REQUIRE(pkbOneResult == pqlOneResult);
}

TEST_CASE("Parent") {

}

TEST_CASE("ParentT") {

}

TEST_CASE("USES") {

}

TEST_CASE("MODIFIES") {

}

TEST_CASE("PATTERN") {

}