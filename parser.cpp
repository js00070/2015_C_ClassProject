#include "parser.h"
#include <stack>
#include <vector>
#include <hash_map>
#include <iostream>
using namespace std;

vector<string> funcNameList={"__NULL__","list","cons","cdr","car","let","if",
							 "eql","<",">","and","or","not",
							 "+", "-", "*", "/", "defun","NUM","ID","lambda"};
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
				break;
			case NUM:
				cout << "Func -> NUM" << endl;
				wordStack.pop();
				wordStack.push(NUM);
				break;
			case LeftBrack:
				switch (nip.wordclass)
				{
				case LeftBrack:
				case NUM:
					cout << "Func -> (Params)" << endl;
					wordStack.pop();
					wordStack.push(RightBrack);
					wordStack.push(Params);
					wordStack.push(LeftBrack);
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
					}
					else
					{
						cout << "Func -> (Params)" << endl;
						wordStack.pop();
						wordStack.push(RightBrack);
						wordStack.push(Params);
						wordStack.push(LeftBrack);
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
				break;
			case RightBrack:
				cout << "Params -> null" << endl;
				wordStack.pop();
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
