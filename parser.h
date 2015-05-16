#include "scanner.h"
#include <list>
#include <set>
#include <utility>
using namespace std;

/*
Func     ->  ID                |
			 NUM               |
			 (FuncName Params) |
			 (Params)

Params   ->  Func Params       |
			 null

*///LL(1)

/*
	E        ->  Func
	Func     ->  ID                |
			     NUM               |
			     (FuncName Params) |
			     (params)           
	
	params   ->  Func Params       |
			     null
	
	FuncName ->  ID
*///LR(1)

#define _list_     1
#define _cons_     2
#define _cdr_      3
#define _car_      4
#define _let_      5
#define _if_       6
#define _eql_      7
#define _smaller_  8
#define _greater_  9
#define _and_     10
#define _or_      11
#define _not_     12
#define _plus_    13
#define _minus_   14
#define _times_   15
#define _divide_  16
#define _defun_   17
#define _NUM_     18
#define _ID_      19
#define _lambda_  20
#define _cond_    21

struct SyntaxTree;
struct Var;

typedef int FuncDef;
extern int funcNum;
extern vector<string> funcNameList;
extern hash_map<string, size_t> keyWordMap;
extern hash_map<string, Var> varIdMap;

enum LispType
{
	lispIntType,lispBoolType,lispFloatType,lispPairType,
	lispStrType,lispVecType,lispFuncType,lispNil
};

struct SyntaxTree
{
	FuncDef funcHead;
	string funcContext;
	list<SyntaxTree*> paramsList;
	void copy(SyntaxTree &in);
	SyntaxTree()
	{
		funcHead = 0;
		funcContext.clear();
		paramsList.clear();
	}
	~SyntaxTree();
//	SyntaxTree& operator=(SyntaxTree);
	
};

SyntaxTree LR_parser(char* src);

SyntaxTree* LL_parser(char* src);


struct Var
{
	LispType type;
	union 
	{
		int lispInt;
		bool lispBool;
		float lispFloat;
		const char* lispStr;
		struct{ Var* car; Var* cdr; } lispPair;
		/* cdr is the next pointer with the type "pair" */
		struct{ int len; Var* src; } lispVec;
		SyntaxTree* lispFunc;//for lambda expression
	}value;
	Var();
	Var(int in);
	Var(bool in);
	Var(float in);
	Var(string& in);
	Var(vector<Var>& in);
	Var(list<Var>& in);
	Var(SyntaxTree in);
	bool operator==(Var& in);
	bool operator<(Var& in);
	bool operator>(Var& in);
	friend ostream& operator<<(ostream&,Var&);
};

Var FuncCalc(SyntaxTree *sTree);
