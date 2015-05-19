#include "parser.h"
#include <map>
#include <utility>
using namespace std;

typedef char _BYTE;
struct Point;
typedef pair<int, int> Coordinate;

extern SyntaxTree *sTree;
extern int numStates;
extern int numNeighbors;
extern int ruleTree[2048][32];
extern int num_nodes;
extern map < Coordinate , Point* > pSet;

void FuncInit(char* src,int numstat,int numneigh);
int Function(int nw,int n,int ne,int w,int c,int e,int sw,int s,int se);
void GetRuleTree();
inline short CheckRule(_BYTE* a)//[nw, ne, sw, se,] n, w, e, s, c
{
	short i;
	short p = 0;
	short end = numNeighbors + 1;
	for (i = 0; i < end; ++i)
		p=p||a[i];
	if (!p)
		return -1; //四周邻居以及自己的状态都为NULL
	p = num_nodes - 1;
	for (i = 0; i < end; ++i)
		p = ruleTree[p][a[i] + 1];
	return p;
}

struct Point
{
	Coordinate pos;
	_BYTE state;
	Point* neighbors[8];//[nw, ne, sw, se,] n, w, e, s
	map<Coordinate, Point*>::iterator it;
	Point(int x,int y,_BYTE in);
	~Point();
	void Update();
};
