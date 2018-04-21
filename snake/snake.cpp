#include <stdio.h>
#include "stdafx.h"
#include <stdlib.h>
#include <windows.h>
#include "math.h"


/*
#include <stdio.h>
int main()
{
int a=2,b=3;
int c;

__asm{
MOV EAX,a;
MOV EBX,b;


//CMP EAX,EBX;
//JAE yy


ADD EAX,EBX;
MOV c,EAX;
}

printf("%d\n",c);
return 0;

}*/


/*//测试rand随机函数
#include <stdio.h>
#include <stdlib.h>

int main()
{
int x;
x=rand() % 10;
printf("%d\n",x);
x=rand() % 10;
printf("%d\n",x);
x=rand() % 10;
printf("%d\n",x);
return 0;
}*/




LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

struct S_list
{
	int x;
	int y;
	struct S_list *next;
}*head;

enum Direc
{
	Up, Down, Left, Right
};
int direc = 0;
RECT rect;
#define  X 40;
#define  Y 40;

int Apple[2] = { 0 };
int Suface[40][40] = { 0 };
int w = 0, xxx, yyy, flag = 0;

//创建蛇
void CreateS()
{
	int i;
	struct S_list *p1, *p2;
	head = p1 = p2 = (struct S_list *)malloc(sizeof(struct S_list));
	p1->x = 21;
	p1->y = 17;

	for (i = 0; i < 4; i++)
	{
		p1 = (struct S_list *)malloc(sizeof(struct S_list));
		p1->x = 21;
		p1->y = 18 + i;
		p2->next = p1;
		p2 = p1;
	};
	p2->next = NULL;
}
//创建苹果
void CreateA()
{
	Apple[0] = rand() % X;
	Apple[1] = rand() % Y;
}

//移动蛇，判断
void Move(int direc)
{
	struct S_list *temp = head->next;
	int x, y;
	x = head->x;
	y = head->y;
	while (temp)
	{
		xxx = temp->x;
		yyy = temp->y;
		temp->x = x;
		temp->y = y;
		x = xxx;
		y = yyy;
		temp = temp->next;
	}

	direc = (enum Direc)direc;
	switch (direc)
	{
	case 0:
		head->y--;				//向上是y--
		break;
	case 1:
		head->y++;
		break;
	case 2:
		head->x--;
		break;
	case 3:
		head->x++;
		break;
	default:
		break;
	}

}
//判断吃到苹果、吃到自己、越界
int Charge()
{
	//咬自己,越界
	struct S_list *temp = head, *body = head->next,*q;
	while (body)
	{
		if (body->x == temp->x && body->y == temp->y) return 1;
		if (temp->x<0 || temp->x>40 || temp->y<0 || temp->y>40)
		{
			flag = 1;
			return 1;
		}
		body = body->next;
	}
	//吃苹果
	temp = head;
	/*
	while (temp)
	{
		if (temp->x == Apple[0], Apple[1] == temp->y)
		{
			temp = (struct S_list *)malloc(sizeof(struct S_list));
			temp->x = Apple[0];
			temp->y = Apple[1];
			temp->next = head;
			head = temp;
			return 2;
		}
		temp = temp->next;
	}*/
	if (head->x==Apple[0] && head->y==Apple[1])
	{
		while (temp->next)
		{
			temp = temp->next;
		}
		q = (struct S_list *)malloc(sizeof(struct S_list));
		q->x = xxx;					//添加尾巴
		q->y = yyy;
		temp->next = q;
		q->next = NULL;
		CreateA();					//再建苹果
	}
	return 0;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;


	int i = 0;
	static HBRUSH hbrSnack;
	static HBRUSH hbrApple;
	static HPEN hpenSurface;
	struct S_list *temp = head;

	switch (message)
	{
	case WM_SIZE:
		w = LOWORD(lParam) / 40;
		break;
	case WM_CREATE:
		SetTimer(hwnd, 1, 500, NULL);				//time
		hbrSnack = CreateSolidBrush(RGB(0, 255, 0));
		hbrApple = CreateSolidBrush(RGB(255, 0, 0));
		hpenSurface = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			direc = 0;
			break;
		case VK_DOWN:
			direc = 1;
			break;
		case VK_LEFT:
			direc = 2;
			break;
		case VK_RIGHT:
			direc = 3;
			break;
		}
		Move(direc);
		if (Charge() == 1)
		{
			MessageBox(hwnd, TEXT("游戏结束"), TEXT("提示"), MB_OK);
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_TIMER:						//important
		if (!flag) {					//flag判断是否死亡
			Move(direc);
			if (Charge() == 1)
			{
				MessageBox(hwnd, TEXT("游戏结束"), TEXT("提示"), MB_OK);

			}
			InvalidateRect(hwnd, NULL, TRUE);			//TRUE
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//先画蛇
		SelectObject(hdc, hbrSnack);
		while (temp)
		{
			Rectangle(hdc, temp->x * w - w, temp->y * w - w, temp->x * w, temp->y * w);
			temp = temp->next;
		}

		//再画苹果
		SelectObject(hdc, hbrApple);
		Ellipse(hdc, Apple[0] * w - w, Apple[1] * w - w, Apple[0] * w, Apple[1] * w);

		//画桌面
		SelectObject(hdc, hpenSurface);
		for (i = 0; i < 40; i++)
		{
			MoveToEx(hdc, i*w, 0, NULL);
			LineTo(hdc, i*w, w * 40);
		}
		for (i = 0; i < 40; i++)
		{
			MoveToEx(hdc, 0, i*w, NULL);
			LineTo(hdc, w * 40, i*w);
		}
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);				//time
		PostQuitMessage(0);
		DeleteObject(hbrApple);
		DeleteObject(hbrSnack);
		DeleteObject(hpenSurface);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;
	static TCHAR szClassName[] = TEXT("helloWin");

	SetRect(&rect, 0, 0, 400, 400);
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	wndclass.style = CS_HREDRAW | CS_VREDRAW;  //窗口风格
	wndclass.lpfnWndProc = WndProc;  //窗口过程
	wndclass.cbClsExtra = 0;  //暂时不需要理解
	wndclass.cbWndExtra = 0;  //暂时不需要理解
	wndclass.hInstance = hInstance;  //当前窗口句柄
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);  //窗口图标
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);  //鼠标样式
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //窗口背景画刷
	wndclass.lpszMenuName = NULL;  //窗口菜单
	wndclass.lpszClassName = szClassName;  //窗口类名

	RegisterClass(&wndclass);
	hwnd = CreateWindow(
		szClassName,  //窗口类的名字
		TEXT("Welcome"),  //窗口标题（出现在标题栏）
		WS_OVERLAPPEDWINDOW,  //窗口风格
		CW_USEDEFAULT,  //初始化时x轴的位置
		CW_USEDEFAULT,  //初始化时y轴的位置
		rect.right - rect.left,  //窗口宽度
		rect.bottom - rect.top,  //窗口高度
		NULL,  //父窗口句柄
		NULL,  //窗口菜单句柄
		hInstance,  //当前窗口的句柄
		NULL  //不使用该值
	);
	CreateA();
	CreateS();

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);



	while (GetMessage(&msg, hwnd, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	for (;;)
	{
		if (true)
		{

		}
	}

	/*
	for (;;)
	{
		if (state == game_start)
		{
			run_game();
		}
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				break;
			}
		}

	}
	*/


	return msg.wParam;
}



/*//测试按键控制移动
#include <windows.h>
#include <stdio.h>
int X=0,Y=0;
RECT rect;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
HINSTANCE hInstance,
HINSTANCE hPrevInstance,
PSTR szCmdLine,
int iCmdShow
)
{
static TCHAR szClassName[] = TEXT("HelloWin");  //窗口类名
HWND     hwnd;  //窗口句柄
MSG      msg;  //消息
WNDCLASS wndclass;  //窗口类



SetRect(&rect,0,0,400,400);				//创建我们需要的客户区大小
AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,false);		//调整窗口

/**********第①步：注册窗口类
//为窗口类的各个字段赋值

wndclass.style = CS_HREDRAW | CS_VREDRAW;  //窗口风格
wndclass.lpfnWndProc  = WndProc;  //窗口过程
wndclass.cbClsExtra   = 0;  //暂时不需要理解
wndclass.cbWndExtra   = 0;  //暂时不需要理解
wndclass.hInstance    = hInstance;  //当前窗口句柄
wndclass.hIcon        = LoadIcon (NULL, IDI_APPLICATION);  //窗口图标
wndclass.hCursor      = LoadCursor (NULL, IDC_ARROW);  //鼠标样式
wndclass.hbrBackground= (HBRUSH) GetStockObject (WHITE_BRUSH);  //窗口背景画刷
wndclass.lpszMenuName = NULL ;  //窗口菜单
wndclass.lpszClassName= szClassName;  //窗口类名

//注册窗口
RegisterClass(&wndclass);

/ *****第②步：创建窗口(并让窗口显示出来)***** /
hwnd = CreateWindow(
szClassName,  //窗口类的名字
TEXT("Welcome"),  //窗口标题（出现在标题栏）
WS_OVERLAPPEDWINDOW,  //窗口风格
CW_USEDEFAULT,  //初始化时x轴的位置
CW_USEDEFAULT,  //初始化时y轴的位置
rect.right-rect.left,  //窗口宽度
rect.bottom-rect.top,  //窗口高度
NULL,  //父窗口句柄
NULL,  //窗口菜单句柄
hInstance,  //当前窗口的句柄
NULL  //不使用该值
);

//显示窗口
ShowWindow (hwnd, iCmdShow);
//更新（绘制）窗口
UpdateWindow (hwnd);

/ **********第③步：消息循环********** /
while( GetMessage(&msg, NULL, 0, 0) ){
TranslateMessage(&msg);  //翻译消息
DispatchMessage (&msg);  //分派消息
}

return msg.wParam;
}

/ **********第④步：窗口过程********** /
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
HDC         hdc;  //设备环境句柄
PAINTSTRUCT ps;
RECT        rect;

static HBRUSH hbrRect;

switch (message)
{
//窗口绘制消息
case WM_SIZE:			//记住只有使用WM_SIZE消息才能获得客户区大小（通过lParam来获得）
X=LOWORD(lParam)/10;
Y=HIWORD(lParam)/10;
break;
case WM_CREATE:
hbrRect=CreateSolidBrush(RGB(0,255,0));
break;
case WM_KEYDOWN:
if (wParam==VK_UP)
{
X+=40;
Y+=40;
/ *
char str[]="";
sprintf(str,"%d",X);
MessageBox(hwnd,TEXT(str),NULL,MB_OK);* /
InvalidateRect(hwnd,NULL,TRUE);		//记住中间的那个值是NULL，而不是&rect!!!
}
case WM_PAINT:
hdc = BeginPaint (hwnd, &ps) ;
GetClientRect(hwnd,&rect);
SelectObject(hdc,hbrRect);
Rectangle(hdc,X,Y,X+40,Y+40);
EndPaint (hwnd, &ps) ;
return 0 ;

//窗口销毁消息
case WM_DESTROY:
PostQuitMessage(0) ;
return 0 ;
}
return DefWindowProc(hwnd, message, wParam, lParam) ;
}*/




/*//测试游戏是否结束、是否吃到苹果
#include <stdio.h>
#include <stdlib.h>  //malloc必加

struct S_list
{
int x;
struct S_list *next;
}*head;

void Create()
{
int i=0;
struct S_list *p1,*p2;
head=p2=p1=(struct S_list *)malloc(sizeof(struct S_list));
p1->x=1;
for (i=0;i<3;i++)
{
p1=(struct S_list *)malloc(sizeof(struct S_list));
p2->next=p1;
p1->x=i+2;
p2=p1;
}
p2->next=NULL;
}

int Charge()
{
struct S_list *temp=head;
while(temp)
{
if (temp->x=2) return 1;		//return 1就不会再return 0了
temp=temp->next;
}
printf("hello none\n");
return 0;			//两者只能选其一
}

int main()
{
int x;
Create();
x=Charge();
printf("%d\n",x);
return 0;
}
*/




/*//测试枚举类型的使用
#include <stdio.h>
enum color{red,green,blue};

int main()
{
int x;
x=(enum color)2;	//这一步转换很重要
switch (x)
{
case red:
printf("red\n");
break;
case blue:
printf("%d\n",blue);
break;
default:
printf("none\n");
break;
}
return 0;
}
*/