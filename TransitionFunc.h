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
