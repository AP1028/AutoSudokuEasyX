#define boxlength 500
#define boxheight 640
#define dpixel 25
#define backcolor BLACK
#define GREY RGB(64, 64, 64)
#define textshiftx 15
#define textshifty 12
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
#include <graphics.h>      // 引用图形库头文件
#include <conio.h>
#include <time.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>  
#include <iostream>
MOUSEMSG mouse;
MOUSEMSG _mouse;
COLORREF pixel[boxlength+5][boxheight+5];
COLORREF colortable[0xe + 2] = { WHITE };
struct met //定义表格主结构体 
{
	int val = 0; //存图 
	int x[10] = { 0,0,0,0,0,0,0,0,0,0 }; //存行 
	int y[10] = { 0,0,0,0,0,0,0,0,0,0 }; //存列 
	int z[10] = { 0,0,0,0,0,0,0,0,0,0 }; //存表格 
	int sx[10] = { 0,0,0,0,0,0,0,0,0,0 }; //存行（算法预处理） 
	int sy[10] = { 0,0,0,0,0,0,0,0,0,0 }; //存列（算法预处理）
	int sum = 0; //存三个占据方向的占据数量 
	bool array = 0;
	bool hash[10] = { 0,0,0,0,0,0,0,0,0,0 }; //存三个占据方向的占据情况 
	int color = 2;
}sud[10][10];
struct met2 //¶¨Òå±í¸ñ½á¹¹Ìå 
{
	bool hash[10] = { 0,0,0,0,0,0,0,0,0,0 }; //´æ±í¸ñÇé¿ö 
	int sum = 0; //´æÕ¼¾ÝÊý¾Ý×ÜÁ¿ 
}ar[4][4];
int tempcolor = 2; //ÁÙÊ±ÑÕÉ«Êý¾Ý 
int cchain[(0xe) + 2]; //¶¨ÒåÑÕÉ«Êý¾ÝÁ´ 
bool pa = 0; //ÊÇ·ñ×Ô¶¯ÑÓÊ± 
int cnt = 0; //¼ÇÂ¼×Ü²Ù×÷Êý 
int delay2 = 0; //×ªÎªintÀàÐÍÑÓÊ±Êý¾Ý 
double delay; //¶¨ÒåÑÓÊ±±äÁ¿ 
int dmode; //¼ÇÂ¼²Ù×÷Êý¾Ý 
int total;
int nview[10][10];
void nviewupdate()
{
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			nview[i][j] = sud[i][j].val;
}
void array();
void Output();
bool check() //¼ì²éËã·¨ 
{
	for (int i = 1; i <= 9; i++)
	{
		bool check[10] = { 0,0,0,0,0,0,0,0,0,0 };
		for (int j = 1; j <= 9; j++)
		{
			if (check[sud[i][j].val] == 1) return 1;
			check[sud[i][j].val] = 1;
		}
	}
	return 0;
}
void update() //Ë¢ÐÂº¯Êý 
{
	//Ë¢ÐÂÐÐºÍÁÐ 
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			for (int k = 1; k <= 9; k++)
			{
				sud[i][j].hash[k] = 0;
				sud[i][j].x[k] = 0;
				sud[i][j].y[k] = 0;
				sud[i][j].z[k] = 0;
				sud[i][j].sum = 0;
			}
	//Ë¢ÐÂ¾Å¹¬¸ñ 
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
		{
			for (int k = 1; k <= 9; k++)
			{
				sud[i][j].x[sud[k][j].val] = 1;
				sud[i][j].y[sud[i][k].val] = 1;
				sud[i][j].hash[sud[k][j].val] = 1;
				sud[i][j].hash[sud[i][k].val] = 1;
			}
			int x, y;
			x = int((i - 1) / 3) + 1;
			y = int((j - 1) / 3) + 1;
			for (int k = 3 * x - 2; k <= 3 * x; k++)
			{
				for (int l = 3 * y - 2; l <= 3 * y; l++)
				{
					sud[i][j].z[sud[k][l].val] = 1;
					sud[i][j].hash[sud[k][l].val] = 1;
				}
			}
			for (int k = 1; k <= 9; k++)
			{
				sud[i][j].sum += sud[i][j].hash[k];
				total += sud[i][j].hash[k];
			}
		}
	return;
}
bool basic() //µÝ¹éÇ°Ô¤´¦Àí»ù±¾Ëã·¨ £¨ÐÐ£¬ÁÐ£© 
{
	bool flag = 0;
	int min = 0x7fffffff;
	int x, y;
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			if (sud[i][j].val == 0 && sud[i][j].sum == 8)
			{
				for (int k = 1; k <= 9; k++)
				{
					if (!sud[i][j].hash[k])
					{
						flag = 1;
						sud[i][j].val = k;
						cnt++;
						update();
						Output();
						break;
					}
				}
			}
	return flag;
}
void array() //Êý×éÏÔÊ¾ 
{
	int sum[9][3]; //1=row, 2=lane, 3=block
	int ans_x[9];
	int ans_y[9];
	int T = 0;
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
		{
			if (sud[i][j].color == 6)
			{
				sud[i][j].color = 7;
			}
		}
	T = 1;
	for (int i = 1; i <= 9; i++)//iÐÐ 
	{
		memset(ans_x, 0, sizeof(ans_x));
		memset(ans_y, 0, sizeof(ans_y));
		int tot = 0;//´æ´¢ÕâÒ»ÐÐµÄ´ð°¸ 
		for (int j = 1; j <= 9; j++) //jÁÐ 
		{
			if (sud[i][j].val == 0) //Ã»Ìî 
			{
				sum[i][T]++;
				ans_x[++tot] = i;
				ans_y[tot] = j;
			}
		}
		if (sum[i][T] >= 2 && sum[i][T] <= 3)
		{
			for (int k = 1; k <= tot; k++)
			{
				sud[ans_x[k]][ans_y[k]].array = 1;
			}
		}
	}
	T = 2;
	for (int j = 1; j <= 9; j++)//jÁÐ 
	{
		memset(ans_x, 0, sizeof(ans_x));
		memset(ans_y, 0, sizeof(ans_y));
		int tot = 0;//´æ´¢ÕâÒ»ÐÐµÄ´ð°¸ 
		for (int i = 1; i <= 9; i++) //jÁÐ 
		{
			if (sud[i][j].val == 0) //Ã»Ìî 
			{
				sum[j][T] ++;
				ans_x[++tot] = i;
				ans_y[tot] = j;
			}
		}
		if (sum[j][T] >= 2 && sum[j][T] <= 3)
		{
			for (int k = 1; k <= tot; k++)
			{
				sud[ans_x[k]][ans_y[k]].array = 1;
			}
		}
	}
}
void arupdate()
{
	for (int i = 1; i <= 3; i++)
		for (int j = 1; j <= 3; j++)
			for (int k = 1; k <= 9; k++)
			{
				ar[i][j].hash[k] = sud[3 * i][3 * j].z[k];
				ar[i][j].sum += sud[3 * i][3 * j].z[k];
			}
}
void dfs() //ÉîËÑ 
{
	int t = 0;
	for (int j = 1; j <= 9; j++)
		for (int k = 1; k <= 9; k++)
			if (sud[j][k].val) t++; //¼ÇÂ¼×ÜÊý 
	if (t >= 81) //È·ÈÏÔ½½çºó·µ»Ø 
	{
		return;
	}
	int min = 0x7fffffff;
	int x, y; //¶¨ÒåÄ¿±ê×ø±ê 
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			if (sud[i][j].val == 0 && 9 - sud[i][j].sum < min && sud[i][j].sum < 9) //Èç¹ûÕÒµ½Ã»ÌîÂúµÄ¿Õ¸ñ 
			{
				x = i; y = j; min = 9 - sud[i][j].sum; //¼ÇÂ¼¸Ã×ø±ê£¬Ë¢ÐÂmin 
			}
			else if (sud[i][j].val == 0 && sud[i][j].sum >= 9) return; //Èç¹ûÕÒµ½²»·ûÊý×ÖÖ±½Ó½áÊø 
	for (int i = 1; i <= 9; i++) //Ã¶¾Ù¸ÃÔªËØhashÊý×é 
	{
		if (!sud[x][y].hash[i]) //ÕÒµ½¿ÉÒÔÌîµÄÊý 
		{
			sud[x][y].val = i; //Ð´Èë 
			int ttcolor = tempcolor; //¸üÐÂÑÕÉ« 
			int tcolor = sud[x][y].color; //¼ÇÂ¼Ô­ÓÐÑÕÉ« 
			if (min > 1) //Èç¹û²»È·ÈÏÍÆ½øÑÕÉ« 
			{
				sud[x][y].color = cchain[tempcolor];
				tempcolor = sud[x][y].color;
			}
			else //Ð´ÈëÓ¦ÓÐÑÕÉ« 
				sud[x][y].color = tempcolor;
			cnt++; //¸üÐÂ¼ÆÊýÆ÷ 
			Output(); //Êä³ö 
			update(); //¸üÐÂ 
			dfs(); //¼ÌÐøÉîËÑ 
			int t = 0;
			for (int j = 1; j <= 9; j++)
				for (int k = 1; k <= 9; k++)
					if (sud[j][k].val) t++;

			if (t < 81)
			{
				sud[x][y].val = 0;
				if (min > 1)
				{
					sud[x][y].color = tcolor;
					tempcolor = ttcolor;
				}
				cnt++;
				update();
				Output();
			}
			else
			{
				return;
			}
		}
	}
	return;
}
void Preprint()
{
	for (int i = 1; i <= boxlength; i++)
		for (int j = 1; j <= boxheight; j++)
			if (((i == dpixel || i == 150 + dpixel || i == 300 + dpixel || i == 450 + dpixel) && (j >= dpixel && j <= 450 + dpixel)) || ((j ==dpixel || j == 150 + dpixel || j == 300 + dpixel || j == 450 + dpixel) && (i >= dpixel && i <= 450 + dpixel)))
			{
				pixel[i][j] = WHITE;
			}
			else if ((i % 50 == dpixel || j % 50 ==  dpixel) && (i >= dpixel && j >= dpixel) && (i <= 450 + dpixel && j <= 450 + dpixel))
			{
				pixel[i][j] = GREY;
			}
	for (int i = 25; i <= 175; i++)
	{
		pixel[i][500] = WHITE;
		pixel[i][600] = WHITE;
	}
	for (int i = 500; i <= 600; i++)
	{
		pixel[25][i] = WHITE;
		pixel[175][i] = WHITE;
	}
	for (int i = 1; i <= boxlength; i++)
		for (int j = 1; j <= boxheight; j++)
		{
			putpixel(i, j, pixel[i][j]);
		}
	outtextxy(55,535,_T("START"));
	return;
}
void textprint(int nx, int ny,char tar)
{
	//char tar = num + '0';
	outtextxy(textshiftx + nx * 50 + 1 + dpixel, textshifty + ny * 50 + 1 + dpixel, tar);
	return;
}
void cls()
{
	for (int x = 0; x <= 8; x++)
		for (int y = 0; y <= 8; y++)
			for (int i = 1; i <= 48; i++)
				for (int j = 1; j <= 48; j++)
				{
					putpixel(i + x * 50 + 1 + dpixel, j + y * 50 + 1 + dpixel, BLACK);
				}
	return;
}
void clsxy(int x,int y)
{
	for (int i = 1; i <= 48; i++)
		for (int j = 1; j <= 48; j++)
		{
			putpixel(i + x * 50 + 1 + dpixel, j + y * 50 + 1 + dpixel, BLACK);
		}
	return;
}
void input()
{
	int val;
	int nx=0, ny=0;
	
	bool startact = false;
	bool startcheck = false;
	//int _nx=0, _ny=0;
	char charx, chary, _charx, _chary,mousex,mousey;
	mouse = GetMouseMsg();
	_mouse = mouse;
	while (1)
	{
		mouse = GetMouseMsg();
		_mouse = mouse;
		bool clicked = 0;
		while (1)
		{
			mouse = GetMouseMsg();
			nx = -1;
			ny = -1;
			if(mouse.x>25 && mouse.x<175 && mouse.y>500 && mouse.y<600 && mouse.mkLButton==0 && getpixel(25, 500) != YELLOW)
			{	
				if (startact) startcheck = 1;
				for (int i = 25; i <= 175; i++)
				{
					putpixel(i, 500, YELLOW);
					putpixel(i, 600, YELLOW);
				}
				for (int i = 500; i <= 600; i++)
				{
					putpixel(25,i, YELLOW);
					putpixel(175, i, YELLOW);
				}
			}
			else if (mouse.x > 25 && mouse.x < 175 && mouse.y>500 && mouse.y < 600 && mouse.mkLButton && getpixel(25, 500) != RED)
			{
				startact = true;
				for (int i = 25; i <= 175; i++)
				{
					putpixel(i, 500, RED);
					putpixel(i, 600, RED);
				}
				for (int i = 500; i <= 600; i++)
				{
					putpixel(25, i, RED);
					putpixel(175, i, RED);
				}
			}
			else if (getpixel(25, 500) == YELLOW || getpixel(25, 500) == RED && !mouse.mkLButton)
			{
				for (int i = 25; i <= 175; i++)
				{
					putpixel(i, 500, WHITE);
					putpixel(i, 600, WHITE);
				}
				for (int i = 500; i <= 600; i++)
				{
					putpixel(25, i, WHITE);
					putpixel(175, i, WHITE);
				}
			}
			else if (mouse.x < 450 + dpixel && mouse.x>dpixel && mouse.y < 450 + dpixel && mouse.y>dpixel && (mouse.x - dpixel) % 50 != 0 && (mouse.y - dpixel) % 50 != 0)
			{
				nx = (int)((mouse.x - dpixel) / 50);
				ny = (int)((mouse.y - dpixel) / 50);
				//outtextxy(600, 50, charx);
				//outtextxy(650, 50, chary);
			}
			if (startcheck) break;
			mouse = GetMouseMsg();
			for (int x = 0; x <= 8; x++)
			{
				for (int y = 0; y <= 8; y++)
				{
					if ((x != nx || y != ny) && getpixel(x * 50 + 10 + dpixel, y * 50 + 10 + dpixel) != BLACK && sud[x + 1][y + 1].val == 0)
						for (int i = 1; i <= 48; i++)
							for (int j = 1; j <= 48; j++)
							{
								putpixel(i + x * 50 + 1 + dpixel, j + y * 50 + 1 + dpixel, BLACK);
							}
					else if (sud[x + 1][y + 1].val != 0 && !(getpixel(x * 50 + 10 + dpixel, y * 50 + 10 + dpixel) == RED && mouse.x >= x * 50 + 1 + dpixel + 1 && mouse.x <= x * 50 + 1 + dpixel + 48 && mouse.y >= y * 50 + 1 + dpixel + 1 && mouse.y <= y * 50 + 1 + dpixel + 48) && !(getpixel(x * 50 + 10 + dpixel, y * 50 + 10 + dpixel) != YELLOW && mouse.x >= x * 50 + 1 + dpixel + 1 && mouse.x <= x * 50 + 1 + dpixel + 48 && mouse.y >= y * 50 + 1 + dpixel + 1 && mouse.y <= y * 50 + 1 + dpixel + 48))
					{
					bool check = 0;
					for (int i = 15; i <= 33; i++)
						for (int j = 15; j <= 33; j++)
						{
							if (getpixel(i + x * 50 + 1 + dpixel, j + y * 50 + 1 + dpixel) == WHITE)
							{
								check = 1;
								break;
							}
						}
					if (check == 0)
					{
						for (int i = 1; i <= 48; i++)
							for (int j = 1; j <= 48; j++)
							{
								putpixel(i + x * 50 + 1 + dpixel, j + y * 50 + 1 + dpixel, BLACK);
							}
						textprint(x, y, sud[x + 1][y + 1].val + '0');
					}
					}
				}
			}
			//_charx = _nx + '0';
			//_chary = _ny + '0';
			//mousex = mouse.x / 100 + '0';
			//mousey = mouse.y / 100 + '0';

			//outtextxy(700, 50, _charx);
			//outtextxy(750, 50, _chary);
			//outtextxy(800, 50, mousex);
			//outtextxy(850, 50, mousey);
			mouse = GetMouseMsg();
			if (mouse.x < 450 + dpixel && mouse.x>dpixel && mouse.y < 450 + dpixel && mouse.y>dpixel && (mouse.x - dpixel) % 50 != 0 && (mouse.y - dpixel) % 50 != 0)
			{
				nx = (int)((mouse.x - dpixel) / 50);
				ny = (int)((mouse.y - dpixel) / 50);
				//outtextxy(600, 50, charx);
				//outtextxy(650, 50, chary);
			}
			else
			{
				nx = -1;
				ny = -1;
			}
			if (nx != -1 && ny != -1)
			{
				if (mouse.mkLButton || KEY_DOWN(VK_RETURN))
				{
					for (int i = 1; i <= 48; i++)
						for (int j = 1; j <= 48; j++)
						{
							putpixel(i + nx * 50 + 1 + dpixel, j + ny * 50 + 1 + dpixel, RED);
						}
					clicked = 1;
				}
				else
				{
					for (int i = 1; i <= 48; i++)
						for (int j = 1; j <= 48; j++)
						{
							putpixel(i + nx * 50 + 1 + dpixel, j + ny * 50 + 1 + dpixel, YELLOW);
						}
				}
			}
			if (clicked == 1)
			{
				break;
			}
			_mouse = mouse;
		}
		//for (int i = 500; i <= 900; i++)
		//{
		//	for (int j = 30; j <= 70; j++)
		//		putpixel(i, j, BLACK);
		//}
		if (nx == -1 || ny == -1) break;
		int maxn = 30000;
		int num = -2;
		for (int t = 1; t <= maxn; t++)
		{
			//system("");
			if (t == maxn / 2)
				for (int i = 1; i <= 48; i++)
					for (int j = 1; j <= 48; j++)
					{
						putpixel(i + nx * 50 + 1 + dpixel, j + ny * 50 + 1 + dpixel, BLACK);
					}
			else if (t == maxn - 1)
				textprint(nx, ny, '|');
			else if (t >= maxn) t = -1;
			for (int i = 0; i <= 9; i++)
			{
				if (KEY_DOWN(i + '0'))
				{
					num = i;
					break;
				}
			}
			if (KEY_DOWN(VK_NUMPAD0))
			{
				num = 0;
			}
			if (KEY_DOWN(VK_NUMPAD1))
			{
				num = 1;
			}
			if (KEY_DOWN(VK_NUMPAD2))
			{
				num = 2;
			}
			if (KEY_DOWN(VK_NUMPAD3))
			{
				num = 3;
			}
			if (KEY_DOWN(VK_NUMPAD4))
			{
				num = 4;
			}
			if (KEY_DOWN(VK_NUMPAD5))
			{
				num = 5;
			}
			if (KEY_DOWN(VK_NUMPAD6))
			{
				num = 6;
			}
			if (KEY_DOWN(VK_NUMPAD7))
			{
				num = 7;
			}
			if (KEY_DOWN(VK_NUMPAD8))
			{
				num = 8;
			}
			if (KEY_DOWN(VK_NUMPAD9))
			{
				num = 9;
			}
			if (KEY_DOWN(VK_ESCAPE) || KEY_DOWN(VK_BACK) || KEY_DOWN(VK_DELETE))
			{
				num = 0;
			}
			if (num != -2)
			{
				break;
			}
		}
		if (num != -2)
		{
			sud[nx + 1][ny + 1].val = num;
			for (int i = 1; i <= 48; i++)
				for (int j = 1; j <= 48; j++)
				{
					putpixel(i + nx * 50 + 1 + dpixel, j + ny * 50 + 1 + dpixel, BLACK);
				}
			if(num!=0)
				textprint(nx, ny, num + '0');
		}
		if (startact) break;
	}
	for (int i = 25; i <= 175; i++)
	{
		putpixel(i, 500, GREY);
		putpixel(i, 600, GREY);
	}
	for (int i = 500; i <= 600; i++)
	{
		putpixel(25, i, GREY);
		putpixel(175, i, GREY);
	}
	settextcolor(GREY);
	outtextxy(55, 535, _T("START"));
	settextcolor(WHITE);
	return;
}
void Output()
{
	//array();
	
	for(int x=0;x<=8;x++)
		for (int y = 0; y <= 8; y++)
		{
			if (nview[x+1][y+1]!=sud[x+1][y+1].val)
			{
				clsxy(x, y);
				if (sud[x + 1][y + 1].val)
				{
					settextcolor(colortable[sud[x + 1][y + 1].color]);
					textprint(x, y, sud[x + 1][y + 1].val + '0');
					settextcolor(WHITE);
				}
					
			}
		}
	nviewupdate();
	return;
}
int main()
{
	srand((unsigned)time(NULL));
	colortable[7] = WHITE;
	colortable[3] = RGB(128,128,255);
	colortable[2] = GREEN;
	colortable[9] = RGB(0,255,255);
	colortable[1] = BLUE;
	colortable[0xe] = RGB(128,255,0);
	colortable[0xc] = YELLOW;
	colortable[4] = RED;
	initgraph(boxlength, boxheight);// 创建绘图窗口，大小为 640x480 像素
	setbkcolor(backcolor);
	settextstyle(32, 16, _T("Courier"));
	settextcolor(WHITE);
	cchain[7] = 7;
	cchain[2] = 3;
	cchain[3] = 9;
	cchain[9] = 1;
	cchain[1] = 0xe;
	cchain[0xe] = 0xc;
	cchain[0xc] = 4;
	cchain[4] = 4;
	while(1)
	{
		for(int i=1;i<=9;i++)
			for (int j = 1; j <= 9; j++)
			{
				sud[i][j].val = 0;
				nview[i][j] = 0;
			}
		Preprint();
		input();
		nviewupdate();
		update();
		dfs(); //¿ªÊ¼ÉîËÑ 
		Output();
		/*int n = 0;
		for (int i = 1; i <= 9; i++)
			for (int j = 1; j <= 9; j++)
				if (sud[i][j].val) n++;
		//¸ù¾ÝÊÇ·ñÌîÂúÈ·ÈÏÊä³öÇé¿ö 
		if (n == 81) //È·ÈÏÌîÂú 
		{
			for (int i = 1; i <= 9; i++)
				for (int j = 1; j <= 9; j++)
				{
					if (sud[i][j].color != 7)
						sud[i][j].color = 2; //È«²¿³ýÔ­À´µÄÊýÊä³öÂÌÉ« 
				}
		}
		Output();*/
		//outtextxy(205,585,sud[7][8].color + '0');
		//outtextxy(255, 585,colortable[2]);
		settextcolor(RED);
		outtextxy(205, 535, _T("FINISHED!"));
		settextcolor(WHITE);
		system("pause");
	}
	system("pause");
}