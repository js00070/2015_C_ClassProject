#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640,680);
	circle(200,200,100);
	_getch();
	closegraph();
	return 0;
}
