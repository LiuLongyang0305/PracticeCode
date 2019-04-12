#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <termios.h>
#include <pthread.h>
#include <sys/types.h>
#define ECHOFLAGS (ECHO|ECHOE|ECHOK|ECHONL)

char screen[24][90];
int del_flag = 0;
int exit_flag = 1;

typedef struct bullet
{
	int x;
	int y;
	struct bullet *pNext;
}BULLET,*pBULLET;
pBULLET head = NULL;//存储子弹的链表表头，全局变量

typedef struct enermy_air
{
	int x;
	int y;
	int exist_flag;
	int bomb_flag;
}ENERMY_AIR;

typedef struct air
{
	int x;
	int y;
}AIRPLANE;

AIRPLANE* self_air = NULL;
ENERMY_AIR* enermy_air = NULL;//默认只有一架敌机器，全局变量

void ShowScreen()
{
	system("clear");
	int i,j;
	for(i = 0;i < 24;i++)
	{
		for(j = 0;j < 90;j++)
			printf("%c",screen[i][j]);
		printf("\n");
	}
}

void InitiateScreen()
{
	int i,j;
	for(i = 0;i< 24;i ++)
	{
		for(j = 0;j < 90;j++)
		{
			screen[i][j] = ' ';
			if(i == 1 || i == 22)
			{
				if(j >= 4 && j <= 84)
					screen[i][j] = '*';
			}
			if(j == 4 || j == 84)
			{
				if(i >=1 && i <= 22)
					screen[i][j] = '*';
			}
		}
	}
}

void SetScreen(int x,int y)
{
	if(x < 22 && x > 1)
		screen[x][y] = 'o';
}

void DrawSelfAirplane()
{
	int x = self_air->x;
	int y = self_air->y;
	if(x + 1 >= 22)
	{
		x = 20;
		self_air->x = 20;
	}
	if(x - 1 <= 2)
	{
		x = 4;
		self_air->x = 4;
	}
	if(y + 1 >= 84)
	{
		y = 82;
		self_air->y = 82;
	}
	if(y - 1 <= 4)
	{
		y = 6;
		self_air->y = 6;
	}
	screen[x - 1][y] = '#'; 
	screen[x + 1][y] = '#'; 
	screen[x][y + 1] = '#'; 
	screen[x][y - 1] = '#'; 
	screen[x][y] = '#'; 
}

void CreatEnermyAirplane()
{
	enermy_air->exist_flag = 1;
	enermy_air->bomb_flag = 0;
	enermy_air->x = rand() % 13 + 3;
	enermy_air->y = rand()%76 + 6;
}

void DrawEnermyAirplane()
{
	if(enermy_air->exist_flag == 0)
	{
		return;
	}
	int x = enermy_air->x;
	int y = enermy_air->y;
	if(y > 4)
	{
		screen[x - 1][y] = '#';
		screen[x + 1][y] = '#'; 
		screen[x][y] = '#'; 
	}
	if(y + 1  > 4)
	{
		screen[x][y + 1] = '#'; 
	}
	if(y - 1 > 4)
	{
		screen[x][y - 1] = '#'; 
	}
	if(y + 1 <= 4)
		enermy_air->exist_flag = '#';
}

void ModifyEnermyAirplane()
{
	if(enermy_air->exist_flag == 0)
	{
		return;
	}
	enermy_air->y -= 1;
}

void  AddBullet(pBULLET head,int bx,int by)
{
	pBULLET tmp = (pBULLET)malloc(sizeof(BULLET));
	tmp->pNext = NULL;
	tmp->x = bx;
	tmp->y = by;
	pBULLET p = head;
	while(p->pNext != NULL)
	{
		p = p->pNext;
	}
	p->pNext = tmp;
}

void DeleteBullet(pBULLET head)
{
	pBULLET p = head;
	 del_flag = 0;
	 pBULLET q = head->pNext;
	 if(q == NULL)
		 return;
		while(1)
		{
			if(q == NULL)
				break;
			if(q->x <= 1)
			{
				del_flag = 1;
				p->pNext = q->pNext;
				free(q);
				break;
			}
			p = p->pNext;
			q = q->pNext;
		}
}

void ModifyBulletPosition(pBULLET head)
{
	pBULLET tmp = head->pNext;
	while(1)
	{
		if(tmp == NULL)
			break;
		tmp->x -= 1;
		tmp = tmp->pNext;
	}
}

void DrawBullet(pBULLET head)
{
	pBULLET p = head->pNext;
	while(1)
	{
		if(p == NULL)
			break;
		SetScreen(p->x,p->y);
		p = p->pNext;
	}
}

void CheckIsBomb(pBULLET head)
{

	pBULLET tmp = head->pNext;
	if(enermy_air->exist_flag == 0)
		return;
	int bx = 0;
	int by = 0;
	int x = enermy_air->x;
	int y = enermy_air->y;
	int  dis = 0;
	while(1)
	{
		if(tmp == NULL)
			break;
		bx = tmp->x;
		by = tmp->y;
		dis = abs(bx - x) + abs(by - y);
		if(dis == 0 || dis == 1)
		{
			tmp->x = 1;
			enermy_air->exist_flag = 0;
			enermy_air->bomb_flag = 1;
			break;
		}
		tmp = tmp->pNext;
	}
}

void BombProcess()
{
	if(enermy_air->bomb_flag == 0)
		return;
	int ey = enermy_air->y;
	char bomb[] = "BOMB!";
	InitiateScreen();
	while(1)
	{
		 DeleteBullet(head);
		if(del_flag == 0)
			break;
	}
	for(int j = 0;j < 5;j ++ )
	{
		screen[enermy_air->x][ey + 2 + j] = bomb[j];
	}
	DrawBullet(head);
	DrawSelfAirplane();
	ShowScreen();
	usleep(200000);
}

void ExitSystem(pBULLET head)
{
	pBULLET p = NULL;
	while(1)
	{
		p = head->pNext;
		if(p == NULL)
			break;
		head->pNext = p->pNext;
		free(p);
	}
	free(head);
	free(enermy_air);
	free(self_air);
}
/************************************************/
//新建进程函数。大多操作全局变量
struct termios new, old;//两个全局结构体，保存终端属性
//设置终端属性
int set_term()
{
	if (tcgetattr(STDIN_FILENO, &old) == -1)
		exit(1);
	new = old;
	new.c_lflag &= ~ECHOFLAGS;//不带回显
	new.c_lflag &= ~ICANON ;//无缓冲
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &new) == -1) 
	{
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &old);
	}
    exit(1);
}
//恢复终端属性
int reset_term()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &old);
	exit(1);
}

void* AirplaneControl()
{
	char con_ch;

		set_term();
	while(exit_flag)
	{
		//set_term();
		con_ch = getchar();
	   // reset_term();
	//	printf("%d\n",con_ch);
		switch(con_ch)
		{
			case 'a':
				self_air->y --;
				break;
			case 'w':
				self_air->x --; 
				break;
			case 'd':
				self_air->y ++;
				break;
			case 's':
				self_air->x ++;
				break;
			case ' ':
				AddBullet(head,self_air->x - 1,self_air->y);
				break;
			case '0':
				exit_flag = 0;
				break;
			default:
				break;
		}
	    //reset_term();
		usleep(50000);
	}
	    reset_term();
		return (void*)NULL;
}
/*********************************************/
int main()
{
	//初始化
	enermy_air = (ENERMY_AIR*)malloc(sizeof(ENERMY_AIR));
	enermy_air->exist_flag = 0;
	enermy_air->bomb_flag = 0;
	self_air = (AIRPLANE*)malloc(sizeof(AIRPLANE));
	self_air->x = 20;
	self_air->y = 44;

	head = (pBULLET)malloc(sizeof(BULLET));
	head->pNext = NULL;
	pthread_t new_thread = 0;
    int result = pthread_create(&new_thread,NULL,(void*)AirplaneControl,NULL);
	if(result != 0)
	{
		printf("Failed to create new pthread!\n");
		return 0;
	}

	while(1)
	{
		if(enermy_air->exist_flag == 0){
			CreatEnermyAirplane();
		}
		CheckIsBomb(head);
		if(enermy_air->bomb_flag == 1){
			BombProcess();
		}
		while(1)
		{
			DeleteBullet(head);
			if(del_flag == 0)
				break;
		}
		InitiateScreen();
		DrawBullet(head);
		DrawEnermyAirplane();
		DrawSelfAirplane();
		ShowScreen();
		usleep(200000);
		ModifyBulletPosition(head);
		ModifyEnermyAirplane();
		if(exit_flag == 0)
		{
			ExitSystem(head);
			break;
		}
	}
	return 0;
}
