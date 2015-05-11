#include "gui.h"

bool isRunning = true;

void PutImgToImg(int dstX, int dstY, IMAGE* pDstImg,IMAGE* pSrcImg, DWORD dwRop = SRCCOPY) //把pSrcImg画到pDstImg上
{
	IMAGE* preWorkingImg = GetWorkingImage();
	SetWorkingImage(pDstImg);
	putimage(dstX, dstY, pSrcImg, dwRop);
	SetWorkingImage(preWorkingImg);
}

bool Fps(int fps)
{
	Sleep(1000/fps);
	return isRunning;
}

void MainWindow::Render()
{
	putimage(dstX,dstY,pSrcImg);
}

int MainLoop()
{
	MainWindow mainWin;
	ButtonWindow buttonWin;
	mainWin.Init();
	buttonWin.Init();
	while (Fps(50))
	{
		MOUSEMSG msg;
		while (MouseHit())
		{
			msg = GetMouseMsg();
			mainWin.Update(msg);
			buttonWin.Update(msg);
		}
		mainWin.Render();
		buttonWin.Render();
	}

	return 0;
}
