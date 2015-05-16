#include "TransitionFunc.h"

SyntaxTree sTree;
int numState;

void FuncInit(char* src,int numstat)
{
	sTree = LL_parser(src);
	numState = numstat;
}

int Function(int nw, int n, int ne, int w, int c, int e, int sw, int s, int se)
{
	varIdMap["nw"] = Var(nw); varIdMap["n"] = Var(n); varIdMap["ne"] = Var(ne);
	varIdMap["w"] = Var(w); varIdMap["c"] = Var(c); varIdMap["e"] = Var(e);
	varIdMap["sw"] = Var(sw); varIdMap["s"] = Var(s); varIdMap["se"] = Var(se);

	Var res = FuncCalc(&sTree);

//	varIdMap.erase("nw"); varIdMap.erase("n"); varIdMap.erase("ne");
//	varIdMap.erase("w"); varIdMap.erase("c"); varIdMap.erase("e");
//	varIdMap.erase("sw"); varIdMap.erase("s"); varIdMap.erase("se");
	if (res.type != lispIntType)
		throw runtime_error("Wrong Function");
	return res.value.lispInt;
}
