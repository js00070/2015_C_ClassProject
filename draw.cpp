#define _CRT_SECURE_NO_WARNINGS
#define NDEBUG

#include <graphics.h>
#include <conio.h>
#include <time.h>
#include "TransitionFunc.h"
using namespace std;

void DebugInit()
{
	FuncInit("(let((cnt (+ nw n ne w e sw s se)))(if (eql cnt 3) 1 (if (eql cnt 2) c 0)))", 2, 8);
	GetRuleTree();
	for (auto &it : pSet)
	{
		it.first.first;//x
		it.first.second;//y
		it.second->state;
	}
}

int length = 640, width = 480;
int clockTime = 0;
bool bPause = true;
int topleft[2] = { 0, 0 };//记录左上角格子的逻辑坐标
MOUSEMSG msg;
bool bRender = true;
int k0 = 1;//格子宽度
int initialx = 320, initialy = 240;
TCHAR debugLog[64],debugLog2[64];
int generations=0;
int x, y;


void drawLine(int length, int width, int k0, int initialx, int initialy)
{
	cleardevice();
	BeginBatchDraw();
	outtextxy(0, 0, debugLog);
	outtextxy(0, 20, debugLog2);
	int i;
	switch (k0)
	{
	case 1:
		for (auto &it : pSet)
		{
			if (it.first.first >= topleft[0] && it.first.first<(topleft[0] + length) && it.first.second >= topleft[1] && it.first.second<(topleft[1] + width))
			{
				switch (it.second->state)
				{
				case 1:
					putpixel(it.first.first - topleft[0] + initialx, it.first.second - topleft[1] + initialy, BLUE);
					break;

				}
			}
		}
		break;

	default:
		setlinecolor(0xC0C0C0);
		for (i = 0; i <= (length - initialx) / k0; i++)
			line(initialx + i*k0, initialy, initialx + i*k0, width - 1);
		for (i = 0; i <= (width - initialy) / k0; i++)
			line(initialx, initialy + i*k0, length - 1, initialy + i*k0);
		line(0, width - 1, length - 1, width - 1);
		line(length - 1, 0, length - 1, width - 1);
		for (auto &it : pSet)
		{
			if (it.first.first >= topleft[0] - 1 && it.first.first <= (topleft[0] + length) && it.first.second >= topleft[1] - 1 && it.first.second <= (topleft[1] + width))
			{
				switch (it.second->state)
				{
				case 1:
					setfillcolor(BLUE);
					floodfill(initialx + (it.first.first - topleft[0] + 1)*k0 + k0 / 3, initialy + (it.first.second - topleft[1] + 1)*k0 + k0 / 3, 0xC0C0C0);
				}
			}
		}
		break;
	}
	FlushBatchDraw();

	//		outtextxy(0, 0, debugLog);

	Sleep(20);
}

void drawScreen(int length,int width,int k0,int initialx,int initialy,int speed)
{
	
	
	DebugInit();
	
	initgraph(length, width);//建立窗口
	setbkcolor(0x708069);
	int i;
	while (true)
	{
		_stprintf_s(debugLog, _T("fuck %d,generations:%d,speed:%d  (%d,%d)"), clock(),generations,speed,x,y);


		{
			if (MouseHit())
			{
				msg = GetMouseMsg();
				FlushMouseMsgBuffer();

				x = topleft[0] + (msg.x - initialx) / k0;
				y = topleft[1] + (msg.y - initialy) / k0;

				_stprintf_s(debugLog2, _T("(%d,%d)"), x, y);
				if (msg.x>initialx&&msg.x<length&&msg.y>initialy&&msg.y<width)
				{
					switch (msg.uMsg)
					{
					case WM_MOUSEMOVE:break;
					case WM_MOUSEWHEEL:
						cleardevice();

						if (msg.wheel > 0)
						{
							switch (k0)
							{
							case 1:
								topleft[0] += ((msg.x - initialx) / k0) * 9 / 10;
								topleft[1] += ((msg.y - initialy) / k0) * 9 / 10;
								k0 = 10;
								break;
							case 40:
								break;
							default:
								topleft[0] += ((msg.x - initialx) / k0) / 2;
								topleft[1] += ((msg.y - initialy) / k0) / 2;
								k0 *= 2;
								break;
							}
						}//放大
						else if (msg.wheel < 0)
						{
							switch (k0)
							{
							case 1:break;
							case 10:
								topleft[0] -= ((msg.x - initialx) / k0) * 9;
								topleft[1] -= ((msg.y - initialy) / k0) * 9;
								k0 = 1;
								break;
							default:
								topleft[0] -= ((msg.x - initialx) / k0);
								topleft[1] -= ((msg.y - initialy) / k0);
								k0 /= 2;
								break;
							}
						}//缩小
						break;
					case WM_LBUTTONDOWN:
						switch (getpixel(msg.x, msg.y))
						{
						case 0x708069:
							AddPoint(topleft[0] + (msg.x - initialx) / k0 - 1, topleft[1] + (msg.y - initialy) / k0 - 1, 1);
							break;
						case BLUE:
							DeletePoint(topleft[0] + (msg.x - initialx) / k0 - 1, topleft[1] + (msg.y - initialy) / k0 - 1, 0);
							break;
						}
						break;
					case WM_RBUTTONDOWN:
						bPause = !bPause;
						break;
					default:
						break;
					}
				}
			}
		}
		
			
		drawLine(length,width,k0,initialx,initialy);

		clockTime += 20;
		if (clockTime >= 320/speed)
		{
			clockTime = 0;
			if (!bPause)
			{
				UpdatePoints();
				generations++;
			}
		}
	}
}
