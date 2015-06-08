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
extern int ruleTree[4096][32];
extern int num_nodes;
extern map < Coordinate , Point* > pSet;
extern int tmpDx[3][8];
extern int tmpDy[3][8];

struct Point
{
	Coordinate pos;
	_BYTE state;
	_BYTE newstate;
	Point* neighbors[8];//[nw, ne, sw, se,] n, w, e, s
	map<Coordinate, Point*>::iterator it;
	Point(int x, int y, _BYTE in);
	~Point();
	void Update();
};

void FuncInit(char* src,int numstat,int numneigh);
int Function(int nw,int n,int ne,int w,int c,int e,int sw,int s,int se);
void GetRuleTree();
void AddPoint(int x, int y, _BYTE in);
void DeletePoint(int x, int y, _BYTE in);

inline void UpdatePoints()
{
	int i;
	auto itnext = pSet.begin();
	++itnext;
	for (auto it = pSet.begin(); it != pSet.end();)
	{
		it->second->Update();
		it = itnext;
		if (itnext != pSet.end())
			++itnext;
	}
	for (auto &it : pSet)
	{
		it.second->state = it.second->newstate;
		if (it.second->state)
		{
			for (i = 0; i < numNeighbors; ++i)
			{
				if (it.second->neighbors[i] == NULL)
				{
					it.second->neighbors[i] = new Point(it.first.first + tmpDx[numNeighbors / 4][i], it.first.second + tmpDy[numNeighbors / 4][i],0);
				}
			}
		}
	}
}

inline _BYTE CheckRule(_BYTE* a)//[nw, ne, sw, se,] n, w, e, s, c
{
	static short i;
	_BYTE p = 0;
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

