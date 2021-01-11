#include<stdio.h>
#include<graphics.h>
#include<math.h>
#define WIDTH 1080
#define HEIGHT 640
#define PI 3.14159
#define Max_Angle 80
IMAGE img[15];
struct Role
{
	int x;
	int y;
	int width;
	int height;
	int coin;
}role;
struct Hook
{
	int x;
	int y;
	int endx;
	int endy;
	int len;
	double angle;
	int dir;
	int state;
	int dx;
	int dy;
	int index;
	int j;
}hook;
struct Mind
{
	int x;
	int y;
	int size;
	int type;
	int money;
	bool flag;
}mind[10];
enum Puture
{
	i_roledown = 2,
	i_roleup=4,
	i_gold=8,
	i_diamond=6,
	i_goldbox=0,
	i_fubox=12
};
enum hook
{
	RIGHT,
	LEFT,
	M_NORMAL,
	M_LONG,
	M_SHORT

};
enum mindtype
{
	gold,
    diamond,
	goldbox
};
void gameInit()
{
	role.width = 120;
	role.height = 110;
	role.x = WIDTH / 2 - role.width / 2;
	role.y = 0;

	hook.len = 50;
	hook.x = role.x + 40;
	hook.y = role.y + 80;
	hook.endx = hook.x;
	hook.endy = hook.y + hook.len;
	hook.angle = 0;
	hook.dir = RIGHT;
	hook.state = M_NORMAL;
	hook.index = -1;
	hook.j = 0;

	for (int i = 0; i < 10; i++)
	{
		mind[i].type = rand() % 3;
		mind[i].size = 60;
		mind[i].x = rand()%(WIDTH-mind[i].size);
		mind[i].y = rand() % (HEIGHT - 150)+120;
		mind[i].money = rand() % 200 + 1;
		mind[i].flag = true;
	}
}
void loadImg()
{
	char fileName[30]="";
	for (int i = 0; i < 14; i++)
	{
		sprintf(fileName, "./images/%d.jpg.jpg", i);
		loadimage(img+i, fileName,60,60);
		for (int i = i_roledown; i <= i_roledown + 1; i++)
		{
			sprintf(fileName, "./images/%d.jpg.jpg", i);
			loadimage(img + i, fileName, 120, 110);
		}
	}
	loadimage(img + 14, "./images/bk.jpg.jpg",WIDTH,HEIGHT-role.height);
}
void putImg()
{
	srand(GetTickCount());
	setfillcolor(YELLOW);
	solidrectangle(0, 0, WIDTH, role.height);
	putimage(0, role.height, img + 14);
	putimage(role.x, role.y, img + i_roledown+1,SRCAND);
	putimage(role.x, role.y, img + i_roledown, SRCPAINT);
	putimage(20, 20, img + i_fubox + 1, SRCAND);
	putimage(20, 20, img + i_fubox, SRCPAINT);
	setbkmode(TRANSPARENT);
	settextcolor(RED);
	settextstyle(25, 15, "黑体");
	char Text[30];
	sprintf(Text, ":  你已经集了%d的福气", role.coin);
	outtextxy(100, 45, Text);
	if (role.coin > 100)
	{
		outtextxy(800, 5, "牛气冲天!!");
	}
	if (role.coin > 300)
	{
		outtextxy(800, 30, "牛星高照!!");
	}
	if (role.coin > 500)
	{
		outtextxy(800, 55, "牛转乾坤!!");
	}
	if (role.coin > 700)
	{
		outtextxy(800, 80, "牛步青云!!");
	}
	if (role.coin > 900)
	{
		outtextxy(800, 105, "牛笔哄哄!!");
	}
	if (hook.j == 10)
	{
		outtextxy(800, 130, "游戏结束!!");
		outtextxy(800, 155, "你最棒啦!!");
	}
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 5);
	line(hook.x, hook.y, hook.endx, hook.endy);
	setfillcolor(WHITE);
	solidcircle(hook.endx, hook.endy, 5);

	//输出矿石
	for (int i = 0; i < 10; i++)
	{
		if(mind[i].flag)
		{ 
			switch (mind[i].type)
			{
			case gold: putimage(mind[i].x, mind[i].y, img + i_gold + 1, SRCAND);
				putimage(mind[i].x, mind[i].y, img + i_gold, SRCPAINT);
				break;
			case diamond: putimage(mind[i].x, mind[i].y, img + i_diamond + 1, SRCAND);
				putimage(mind[i].x, mind[i].y, img + i_diamond, SRCPAINT);
				break;
			case goldbox: putimage(mind[i].x, mind[i].y, img + i_goldbox + 1, SRCAND);
				putimage(mind[i].x, mind[i].y, img + i_goldbox, SRCPAINT);
				break;
			}
		}
	}

}
double transangle(double i)
{
	return PI / 180 * i;
}
void HookRock()
{
	if (hook.angle > Max_Angle)
	{
		hook.dir = LEFT;
	}
	if (hook.angle < -Max_Angle)
	{
		hook.dir = RIGHT;
	}
	if (hook.dir == RIGHT)
	{
		hook.angle++;
	}
	if (hook.dir == LEFT)
	{
		hook.angle--;
	}
	hook.endx = hook.x + sin(transangle(hook.angle)) * hook.len;
	hook.endy = hook.y + cos(transangle(hook.angle)) * hook.len;
}
void HookLong(int speed)
{
	hook.dx = sin(transangle(hook.angle)) * speed;
	hook.dy = cos(transangle(hook.angle)) * speed;
	hook.endx += hook.dx;
	hook.endy += hook.dy;
}
void HookShort(int speed)
{
	hook.dx = sin(transangle(hook.angle)) * speed;
	hook.dy = cos(transangle(hook.angle)) * speed;
	hook.endx -= hook.dx;
	hook.endy -= hook.dy;
}
double HookDisten()
{
	return sqrt((double)(hook.endx - hook.x) * (hook.endx - hook.x) + (double)(hook.endy - hook.y) * (hook.endy - hook.y));
}
void Hookwork()
{
	if (GetAsyncKeyState(VK_SPACE))
	{
		hook.state = M_LONG;
	}
	if (hook.endx > WIDTH || hook.endx<0 || hook.endy>HEIGHT)
	{
		hook.state = M_SHORT;
	}
	if (hook.state == M_NORMAL)
	{
		HookRock();
	}
	if (hook.state == M_LONG)
	{
		HookLong(20);
	}
	if (hook.state == M_SHORT)
	{
		HookShort(20);
	}
	if (HookDisten() <= hook.len)
	{
		hook.state = M_NORMAL;
	}

}
void JudgeGrap()
{
	for (int i = 0; i < 10; i++)
	{
		if (mind[i].flag == true &&
			hook.endx > mind[i].x &&
			hook.endx<mind[i].x + mind[i].size &&
			hook.endy>mind[i].y &&
			hook.endy < mind[i].y + mind[i].size)
		{
			hook.index = i;
			break;
		}
	}
	if (hook.index != -1)
	{
		hook.state = M_SHORT;
		mind[hook.index].x = hook.endx - 30;
		mind[hook.index].y = hook.endy - 30;
		if (HookDisten() <= hook.len)
		{
			mind[hook.index].flag = false;
			role.coin = role.coin + mind[hook.index].money;
			hook.index = -1;
			hook.j = hook.j + 1;
			hook.state = M_NORMAL;
		}
	}
}
int main()
{
	initgraph(WIDTH, HEIGHT, SHOWCONSOLE);
	gameInit();
	loadImg();
	DWORD t1, t2;
	t1 = t2 = GetTickCount();
	while (1)
	{
		BeginBatchDraw();
		putImg();
		EndBatchDraw();
		JudgeGrap();
		if (t2 - t1 > 20)
		{
			Hookwork();
			t1 = t2;
		}
		t2 = GetTickCount();
	}
	return 0;
}
