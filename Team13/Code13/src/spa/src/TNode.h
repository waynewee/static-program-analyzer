#include <string>
#include <list>

using namespace std;

enum NodeType
{
	program,
	procedure,
	stmtList,
	readStmt,
	printStmt,
	callStmt,
	whileStmt,
	ifStmt,
	assignStmt,
	condExpr,
	relExpr,
	relFactor,
	expr,
	term,
	factor,
	varName,
	procName,
	constValue
};

enum Operator
{
	not,
	and,
	or ,
	greater,
	geq,
	lesser,
	leq,
	equal,
	unequal,
	plus,
	minus,
	times,
	divide,
	mod
};

typedef double Value;
typedef string Name;
typedef int Index;

class TNode
{
public:
	TNode(void);
	TNode(NodeType _type, Name _name, Value _value, Operator _op, Index _index, list<TNode*>* _children);
	bool AddChild(TNode* child);
	bool SetParent(TNode* newParent);
	bool SetName(string newName);
	bool SetValue(double newValue);
	bool SetOp(Operator newOp);

private:
	NodeType type;
	TNode* parent;
	list<TNode*>* children;
	Value value;
	Name name;
	Operator op;
	Index index;
};