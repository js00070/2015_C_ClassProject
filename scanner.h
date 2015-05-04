#include "my_re.h"
#include <list>
#include <vector>
#include <hash_map>
using namespace std;

enum WordClass{
	NIL,ID,NUM,Params,Func,FuncName,LeftBrack,RightBrack,_E
};

extern RegularExpr patternNUM;
extern RegularExpr getNextWord;

struct Element
{
	string keyword;
	WordClass wordclass;

	Element()
	{
		keyword.clear();
		wordclass = NIL;
	}
	void GetWordClass();
	char* GetNextElement(char* src);
};
