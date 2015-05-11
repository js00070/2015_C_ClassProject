#include "parser.h"
#include <stack>
#include <vector>
#include <hash_map>
#include <cctype>
#include <iostream>
using namespace std;

Var::Var()
{
	type = lispNil;
}

Var::Var(int in)
{
	type = lispIntType;
	value.lispInt = in;
}

Var::Var(bool in)
{
	type = lispBoolType;
	value.lispBool = in;
}

Var::Var(float in)
{
	type = lispFloatType;
	value.lispFloat = in;
}

Var::Var(string& in)
{
	type = lispStrType;
	char* p = new char[in.length()+1];
	p[in.length() + 1] = '\0';
	for (int i = 0; i < in.length(); ++i)
		p[i] = in[i];
//	in.copy(p,in.length(),0);
	value.lispStr = (const char*)p;
}

Var::Var(vector<Var>& in)
{
	type = lispVecType;
	Var* p = new Var[in.size()];
	value.lispVec.len = in.size();
	value.lispVec.src = p;
	for (size_t i = 0; i < in.size(); ++i)
	{
		p[i] = in.at(i);
	}
}

Var::Var(list<Var>& in)
{
	type = lispPairType;
	value.lispPair = { NULL, NULL };
	Var* pNow = this;
	for (auto vp : in)
	{
		pNow->value.lispPair.car = new Var;
		*(pNow->value.lispPair.car) = vp;
		pNow = pNow->value.lispPair.cdr;
		pNow = new Var;
		pNow->type = lispPairType;
		pNow->value.lispPair = { NULL, NULL };
	}
}

bool Var::operator==(Var& in)
{
	if (type == in.type)
	{
		switch (type)
		{
		case lispIntType:
			return value.lispInt == in.value.lispInt;
		default:
			throw runtime_error("Compile Error!");
			break;
		}
	}
	else
		return false;
}

vector<string> funcNameList={"__NULL__","list","cons","cdr","car","let","if",
							 "eql","<",">","and","or","not",
							 "+", "-", "*", "/", "defun","NUM","ID","lambda","cond"};
hash_map<string, size_t> keyWordMap;



void KeyWordListInit()
{
	for (size_t i = 0; i < funcNameList.size(); ++i)
	{
		keyWordMap[funcNameList[i]] = i;
	}
}

SyntaxTree LL_parser(char* src)
{
	SyntaxTree res;
	char* pStr = src;
	KeyWordListInit();
	Element ip;
	Element nip;
	pStr = ip.GetNextElement(pStr);
	nip.GetNextElement(pStr);
	stack<WordClass> wordStack;
	wordStack.push(Func);
	stack<SyntaxTree*> syntaxStack;
	syntaxStack.push(&res);
	SyntaxTree *tmpSyntaxTree;
	while (!wordStack.empty())
	{
		if (wordStack.top() == ip.wordclass)
		{
			wordStack.pop();
			pStr = ip.GetNextElement(pStr);
			ip = nip;
			nip.GetNextElement(pStr);
			continue;
		}
		if (wordStack.top() != Func && wordStack.top() != Params)
		{
			throw runtime_error("Compile Error");
			break;
		}
		switch (wordStack.top())
		{
		case Func:
			switch (ip.wordclass)
			{
			case ID:
				cout << "Func -> ID" << endl;
				wordStack.pop();
				wordStack.push(ID);
				syntaxStack.top()->funcHead = _ID_;
				syntaxStack.top()->funcContext = ip.keyword;
				syntaxStack.pop();
				break;
			case NUM:
				cout << "Func -> NUM" << endl;
				wordStack.pop();
				wordStack.push(NUM);
				syntaxStack.top()->funcHead = _NUM_;
				syntaxStack.top()->funcContext = ip.keyword;
				syntaxStack.pop();
				break;
			case LeftBrack:
				switch (nip.wordclass)
				{
				case LeftBrack:
				case NUM:
					cout << "Func -> (Params)" << endl;//Func -> (list Params)
					wordStack.pop();
					wordStack.push(RightBrack);
					wordStack.push(Params);
					wordStack.push(LeftBrack);
					syntaxStack.top()->funcHead = _list_;
					syntaxStack.top()->funcContext = "list";
					break;
				case ID:
					if (keyWordMap[nip.keyword])
					{
						cout << "Func -> (FuncName Params)" << endl;
						nip.wordclass = FuncName;
						wordStack.pop();
						wordStack.push(RightBrack);
						wordStack.push(Params);
						wordStack.push(FuncName);
						wordStack.push(LeftBrack);
						syntaxStack.top()->funcHead = keyWordMap[nip.keyword];
						syntaxStack.top()->funcContext = nip.keyword;
					}
					else
					{
						cout << "Func -> (Params)" << endl;//Func -> (list Params)
						wordStack.pop();
						wordStack.push(RightBrack);
						wordStack.push(Params);
						wordStack.push(LeftBrack);
						syntaxStack.top()->funcHead = _list_;
						syntaxStack.top()->funcContext = "list";
					}
					break;
				default:
					throw runtime_error("Compile Error");
					break;
				}
				break;
			default:
				throw runtime_error("Compile Error");
				break;
			}
			break;
		case Params:
			switch (ip.wordclass)
			{
			case LeftBrack:
			case ID:
			case NUM:
				cout << "Params -> Func Params" << endl;
				wordStack.pop();
				wordStack.push(Params);
				wordStack.push(Func);
				tmpSyntaxTree = new SyntaxTree;
				syntaxStack.top()->paramsList.push_back(tmpSyntaxTree);
				syntaxStack.push(tmpSyntaxTree);
				break;
			case RightBrack:
				cout << "Params -> null" << endl;
				wordStack.pop();
				syntaxStack.pop();
				break;
			default:
				throw runtime_error("Compile Error");
				break;
			}
			break;
		default:
			throw runtime_error("Compile Error");
			break;
		}
	}
	return res;
}

hash_map<string, Var> varIdMap;

Var tmpCalc(SyntaxTree sTree)
{
	list<Var> varList;
	int tmpInt = 0;
	switch (sTree.funcHead)
	{
	case _list_:
		varList.clear();
		for (auto it : sTree.paramsList)
		{
			varList.push_back(tmpCalc(*it));
		}
		return Var(varList);
		break;
	case _cons_:
		break;
	case _cdr_:
		break;
	case _car_:
		break;
	case _let_:
		for (auto pTree : sTree.paramsList.front()->paramsList)
		{
			if (pTree->paramsList.front()->funcHead != _ID_)
				throw runtime_error("Compile Error!");
			varIdMap[pTree->paramsList.front()->funcContext] = tmpCalc(*(pTree->paramsList.back()));
		}
		return tmpCalc(*(sTree.paramsList.back()));
		break;
	case _if_:
		break;
	case _eql_:
		return Var(tmpCalc(*(sTree.paramsList.front())) == tmpCalc(*(sTree.paramsList.back())));
		break;
	case _smaller_:

		break;
	case _greater_:
		break;
	case _and_:
		break;
	case _or_:
		break;
	case _not_:
		break;
	case _plus_:
		tmpInt = 0;
		for (auto pTree : sTree.paramsList)
		{
			Var tmpVar = tmpCalc(*pTree);
			tmpInt += tmpVar.value.lispInt;
		}
		return Var(tmpInt);
		break;
	case _minus_:
		tmpInt = 0;
		for (auto it = sTree.paramsList.begin(); it != sTree.paramsList.end();++it)
		{
			Var tmpVar = tmpCalc(**it);
			if (it == sTree.paramsList.begin())
				tmpInt += tmpVar.value.lispInt;
			else
				tmpInt -= tmpVar.value.lispInt;
		}
		return Var(tmpInt);
		break;
	case _times_:
		tmpInt = 1;
		for (auto pTree : sTree.paramsList)
		{
			Var tmpVar = tmpCalc(*pTree);
			tmpInt *= tmpVar.value.lispInt;
		}
		return Var(tmpInt);
		break;
	case _divide_:
		break;
	case _defun_:
		break;
	case _NUM_:
		return Var(atoi((sTree.funcContext.c_str())));
		break;
	case _ID_:
		if (varIdMap.find(sTree.funcContext) != varIdMap.end())
			return varIdMap[sTree.funcContext];
		else
			return Var(sTree.funcContext);
		break;
	case _lambda_:
		break;
	case _cond_:
		break;
	default:
		throw runtime_error("UNKNOWN ERROR");
		break;
	}
}

SyntaxTree LR_parser(char* src)//To be continued
{
	KeyWordListInit();
	char* pStr = src;
	SyntaxTree res;
	stack<WordClass> wordStack;
	stack<int> stateStack;
	stateStack.push(1);
	Element nW;
	pStr = nW.GetNextElement(pStr);
	int tmpState;
	WordClass tmpWord;
	while (!wordStack.empty() && wordStack.top() != _E)
	{
		switch (stateStack.top())
		{
		case 1://E->.Func
			switch (nW.wordclass)
			{
			case LeftBrack:
				tmpState = 9;
				tmpWord = LeftBrack;
				break;
			case NUM:
			case ID:
				tmpState = 2;
				tmpWord = Func;
				break;
			default:
				break;
			}
			stateStack.push(tmpState);
			wordStack.push(tmpWord);
			pStr = nW.GetNextElement(pStr);
			break;
		case 2:
			switch (nW.wordclass)
			{
			case NIL:
				stateStack.pop();
				wordStack.pop();
				wordStack.push(_E);
				continue;
			case LeftBrack:
				wordStack.pop();
				wordStack.push(Params);
				continue;
			case ID:
			case NUM:
				tmpState = 2;
				tmpWord = Func;
				break;
			
			default:
				break;
			}
			stateStack.push(tmpState);
			wordStack.push(tmpWord);
			pStr = nW.GetNextElement(pStr);
			break;
		case 7://Func->(FuncName.Params)
			switch (nW.wordclass)
			{
			case ID:
			case NUM:
				tmpState = 2;
				tmpWord = Func;
				break;
			case LeftBrack:
				tmpState = 9;
				tmpWord = LeftBrack;
				break;
			default:
				break;
			}
			stateStack.push(tmpState);
			wordStack.push(tmpWord);
			pStr = nW.GetNextElement(pStr);
			break;
		case 9://Func->(.FuncName Params)
			   //Func->(.Params)
			switch (nW.wordclass)
			{
			case LeftBrack:
				tmpState = 9;
				tmpWord = LeftBrack;
				break;
			case NUM:
				tmpState = 2;
				tmpWord = Func;
				break;
			case ID:
				if (keyWordMap[nW.keyword])
				{
					tmpState = 7;
					tmpWord = FuncName;
				}
				else
				{
					
					tmpState = 2;
					tmpWord = Func;
				}
				break;
			default:
				break;
			}
			stateStack.push(tmpState);
			wordStack.push(tmpWord);
			pStr = nW.GetNextElement(pStr);
			break;
		case 5://Func->ID.     (if it's not a keyword then reduce this)
			   //FuncName->ID. (if it's a keyword then reduce this)
			switch (nW.wordclass)
			{

			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	return res;
}
