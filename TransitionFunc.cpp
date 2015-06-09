#include "TransitionFunc.h"
#include <map>
#include <string>
#include <cstdio>
#include <list>
#include <utility>
using namespace std;

SyntaxTree *sTree;
int numStates;
int numNeighbors;
int ruleTree[4096][32];
int num_nodes;
map<Coordinate, Point*> pSet;
_BYTE tmpByteArray[9];//[nw, ne, sw, se,] n, w, e, s, c
short inverseNeighbors[8];
int tmpDx[3][8] = { { 0 }, { 0, -1, 1, 0 }, { -1, 1, -1, 1, 0, -1, 1, 0 } };
int tmpDy[3][8] = { { 0 }, { -1, 0, 0, 1 }, { -1, -1, 1, 1, -1, 0, 0, 1 } };

void FuncInit(char* src, int numstat, int numneigh)
{
	if (sTree)
		delete sTree;
	pSet.clear();
	sTree = LL_parser(src);
	numStates = numstat;
	num_nodes = 0;
	numNeighbors = numneigh;
	if (numNeighbors == 8)
	{
		inverseNeighbors[0] = 3; inverseNeighbors[1] = 2;
		inverseNeighbors[2] = 1; inverseNeighbors[3] = 0;
		inverseNeighbors[4] = 7; inverseNeighbors[5] = 6;
		inverseNeighbors[6] = 5; inverseNeighbors[7] = 4;
	}
	else
	{
		inverseNeighbors[0] = 3; inverseNeighbors[1] = 2;
		inverseNeighbors[2] = 1; inverseNeighbors[3] = 0;
	}
}

int Function(int nw, int n, int ne, int w, int c, int e, int sw, int s, int se)
{
	varIdMap["nw"] = Var(nw); varIdMap["n"] = Var(n); varIdMap["ne"] = Var(ne);
	varIdMap["w"] = Var(w); varIdMap["c"] = Var(c); varIdMap["e"] = Var(e);
	varIdMap["sw"] = Var(sw); varIdMap["s"] = Var(s); varIdMap["se"] = Var(se);

	Var res = FuncCalc(sTree);

	//	varIdMap.erase("nw"); varIdMap.erase("n"); varIdMap.erase("ne");
	//	varIdMap.erase("w"); varIdMap.erase("c"); varIdMap.erase("e");
	//	varIdMap.erase("sw"); varIdMap.erase("s"); varIdMap.erase("se");
	if (res.type != lispIntType)
		throw runtime_error("Wrong Function");
	return res.value.lispInt;
}

int params[9];//nw, ne, sw, se, n, w, e, s, c
int numParams = numNeighbors + 1;
map<string, int> nodeMap;

inline int f(int *p)
{
	return Function(p[0], p[4], p[1], p[5], p[8], p[6], p[2], p[7], p[3]);
	//	return Function(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]);
}

void iprintf(int* a, const char* s)
{
	char tmp[64];
	const char* p = s;
	for (int i = 0; i < numStates + 1; i++)
	{
		sscanf_s(p, "%d", &a[i]);
		sscanf_s(p, "%s", tmp, 64);
		p += strlen(tmp) + 1;
	}
}

int GetNode(string s)
{
	auto it = nodeMap.find(s);
	if (it != nodeMap.end())
		return it->second;
	nodeMap[s] = num_nodes;
	iprintf(ruleTree[num_nodes], s.c_str());
	return num_nodes++;
}

int dfs(int at)
{
	if (at == 0)
		return f(params);
	char buf[1024];
	sprintf_s(buf, 1024, "%d ", at);
	for (int i = 0; i < numStates; ++i)
	{
		params[numParams - at] = i;
		sprintf_s(buf + strlen(buf), 1024 - strlen(buf), "%d ", dfs(at - 1));
	}
	return GetNode(buf);
}

void GetRuleTree()
{
	numParams = numNeighbors + 1;
	dfs(numParams);
	printf("num_states=%d\n", numStates);
	printf("num_neighbors=%d\n", numNeighbors);
	printf("num_nodes=%d\n", num_nodes);
	for (int i = 0; i < num_nodes; ++i)
	{
		//		printf("s%d :",i);
		for (int j = 0; j < numStates + 1; ++j)
			printf(" %d", ruleTree[i][j]);
		printf("\n");
	}
}

void AddPoint(int x, int y, _BYTE in)
{
	auto tmpit = pSet.find(make_pair(x, y));
	if (tmpit == pSet.end())
	{
		new Point(x, y, in);
		if (in)
		{
			if (numNeighbors == 8)
			{
				AddPoint(x - 1, y - 1, 0); AddPoint(x + 1, y - 1, 0);
				AddPoint(x - 1, y + 1, 0); AddPoint(x + 1, y + 1, 0);
				AddPoint(x, y - 1, 0); AddPoint(x - 1, y, 0);
				AddPoint(x + 1, y, 0); AddPoint(x, y + 1, 0);
			}
			else
			{
				AddPoint(x, y - 1, 0); AddPoint(x - 1, y, 0);
				AddPoint(x + 1, y, 0); AddPoint(x, y + 1, 0);
			}
		}
	}
	else
	{
		if (in)
		{
			tmpit->second->state = in;
			if (numNeighbors == 8)
			{
				AddPoint(x - 1, y - 1, 0); AddPoint(x + 1, y - 1, 0);
				AddPoint(x - 1, y + 1, 0); AddPoint(x + 1, y + 1, 0);
				AddPoint(x, y - 1, 0); AddPoint(x - 1, y, 0);
				AddPoint(x + 1, y, 0); AddPoint(x, y + 1, 0);
			}
			else
			{
				AddPoint(x, y - 1, 0); AddPoint(x - 1, y, 0);
				AddPoint(x + 1, y, 0); AddPoint(x, y + 1, 0);
			}
		}
	}
}

void DeletePoint(int x, int y, _BYTE in)
{
	auto it = pSet.find(make_pair(x, y));
	it->second->state = 0;
}

inline Point* MapFind(Coordinate pos)
{
	auto it = pSet.find(pos);
	if (it == pSet.end())
		return NULL;
	return it->second;
}

Point::Point(int x, int y, _BYTE in)
{
	pos = make_pair(x, y);
	pair<map<Coordinate, Point*>::iterator, bool> ret = pSet.insert(make_pair(pos, this));
	it = ret.first;
	state = in;
	newstate = in;
	if (numNeighbors == 8)
	{
		neighbors[0] = MapFind(make_pair(x - 1, y - 1)); neighbors[1] = MapFind(make_pair(x + 1, y - 1));
		neighbors[2] = MapFind(make_pair(x - 1, y + 1)); neighbors[3] = MapFind(make_pair(x + 1, y + 1));
		neighbors[4] = MapFind(make_pair(x, y - 1)); neighbors[5] = MapFind(make_pair(x - 1, y));
		neighbors[6] = MapFind(make_pair(x + 1, y)); neighbors[7] = MapFind(make_pair(x, y + 1));
	}
	else
	{
		neighbors[0] = MapFind(make_pair(x, y - 1)); neighbors[1] = MapFind(make_pair(x - 1, y));
		neighbors[2] = MapFind(make_pair(x + 1, y)); neighbors[3] = MapFind(make_pair(x, y + 1));
	}
	for (short i = 0; i < numNeighbors; ++i)
	{
		if (neighbors[i])
			neighbors[i]->neighbors[inverseNeighbors[i]] = this;
	}
}

Point::~Point()
{
	pSet.erase(it);
	for (short i = 0; i < numNeighbors; ++i)
	{
		if (neighbors[i])
			neighbors[i]->neighbors[inverseNeighbors[i]] = NULL;
	}
}

void Point::Update()
{
	for (short i = 0; i < numNeighbors; ++i)
	{
		if (neighbors[i])
			tmpByteArray[i] = neighbors[i]->state;
		else
			tmpByteArray[i] = NULL;
	}
	tmpByteArray[numNeighbors] = state;
	newstate = CheckRule(tmpByteArray);
	if (newstate == -1)
		delete this;
}
