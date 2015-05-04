#include "scanner.h"

RegularExpr patternNUM("[0-9]*");
RegularExpr getNextWord("<|>|+|-|\\*|/|\\(|\\)|([A-Z]|[a-z]|[0-9])*");

void Element::GetWordClass()
{
	const char* p = keyword.c_str();
	if (patternNUM.GetMathchedPstr((char*)p) - (char*)p)
		wordclass = NUM;
	else
		switch (keyword[0])
		{
		case '(':
			wordclass = LeftBrack;
			break;
		case ')':
			wordclass = RightBrack;
			break;
		default:
			wordclass = ID;
			break;
		}
}

char* Element::GetNextElement(char* src)
{
	if (!src)
	{
		keyword.clear();
		wordclass = NIL;
		return NULL;
	}
	while (*src)
	{
		keyword.clear();
		char* pend = getNextWord.GetMathchedPstr(src);
		for (char* p = src; p != pend; p++)
			keyword.push_back(*p);
		if (keyword.size())
		{
			GetWordClass();
			return pend;
		}
		src++;
	}
	wordclass = NIL;
	return NULL;
}
