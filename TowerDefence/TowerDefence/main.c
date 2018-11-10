#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "ItemDesign.h"
#pragma warning (disable:4996)

COORD MyGetCursor();
void MySetCursor(int x, int y);
void RemoveCursor();
void RunGame();
void StartGame();
void ShoewHelp();
void ExitGame();


int main()
{
	RemoveCursor();
	RunGame();

	return 0;
}

void RemoveCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void MySetCursor(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD MyGetCursor()
{
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}

void RunGame()
{
	system("mode con cols=100 lines=30");

	while (1)
	{
		MySetCursor(20, 5);
		for (int i = 0; i < 11; i++)
		{
			for (int j = 0; j < 28; j++)
			{
				MySetCursor(20+j*2, 5+i);
				if (title[i][j] == 1)
					printf("■");
			}
			printf("\n");
		}

	
		MySetCursor(40, 20);
		printf("1 : PLAY GAME");

		MySetCursor(40, 22);
		printf("2 : HELP");

		MySetCursor(40, 24);
		printf("3 : EXIT");
		
		char selectMenu = getch();
		
		if (selectMenu == '1')
		{
			// 게임시작
		}
		else if (selectMenu == '2')
		{
			// 도움말
		}
		else if (selectMenu == '3')
		{
			ExitGame();
			break;
		}
		else
		{
			continue;
		}
	}
}

void StartGame()
{
	system("cls");
}

void ShoewHelp()
{
	system("cls");
}

void ExitGame()
{
	system("cls");
	printf("소프트웨어 기초설계 2반 6조\n");
	printf("박상우\n이다훈\n이종원\n박철우\n");
	printf("===================================\n");
	printf("contact us : psw7205@gmail.com\n");
	printf("https://github.com/psw7205/SWDesign\n");
}
