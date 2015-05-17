#include "parser.h"
#include <vector>
using namespace std;


extern SyntaxTree *sTree;
extern int numStates;
extern int numNeighbors;
extern int ruleTree[2048][32];
extern int num_nodes;

void FuncInit(char* src,int numstat,int numneigh);
int Function(int nw,int n,int ne,int w,int c,int e,int sw,int s,int se);
void GetRuleTree();

inline int CheckRule(int* a)//nw, ne, sw, se, n, w, e, s, c
{
	int p = num_nodes - 1;
	int end = numNeighbors + 1;
	for (int i = 0; i < end; ++i)
		p = ruleTree[p][a[i] + 1];
	return p;
}
