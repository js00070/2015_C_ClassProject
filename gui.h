#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <list>
using namespace std;

struct MainWindow;
struct SubWindow;
struct Button;

extern bool isRunning;

void PutImgToImg(int dstX, int dstY, IMAGE* pDstImg,IMAGE* pSrcImg, DWORD dwRop); //把pSrcImg画到pDstImg上
bool Fps(int fps);
int MainLoop();

struct MainWindow
{
	int dstX, dstY;
	int width, height;
	IMAGE* pSrcImg;
	bool bPause;

	void Init();
	void Update(MOUSEMSG msg);
	void Render();
};

struct ButtonWindow
{
	int dstX, dstY;
	int width, height;
	IMAGE* pSrcImg;
	list<Button> buttonList;

	void Init();
	void Update(MOUSEMSG msg);
	void Render();
};

struct Button
{
	int dstX, dstY;
	int width, height;
	IMAGE* pSrcImg;
	bool bMouseIn;  //鼠标是否移动到了Button范围内

	void Update(MOUSEMSG msg);
	void Init();
	void Render();
};
