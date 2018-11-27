#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "ItemDesign.h"
#include "Monsters.h"
#include "Tower.h"
#include "Bullet.h"

#pragma warning (disable:4996)
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

COORD MyGetCursor();
void MySetCursor(int x, int y);
void RemoveCursor();
void RunGame();
void StartGame();
void ShowHelp();
void ExitGame();
void DrawGameBoard();
void ShiftRight();
void ShiftLeft();
void ShiftDown();
void ShiftUp();
void PrintCurPos();
void KeyInput();
void MakeTower(int type);
void PrintHelpMenu();
void Hit(int bx, int by, int ex, int ey, int id, int *hp, int damage);

NPC* MakeMonster();
int MoveMonster(NPC *mon);

int curPosX, curPosY; //these variables are coordinates of the 'mapModel' array, not the coordinates of console screen
int start_flag = 0;

COORD start, end, first_corner;
Tower *TowerList;
Bullet *BulletList;
int gold = 9999;
int life = 10;

int main() {
	RunGame();
	getchar();
	return 0;
}

void RemoveCursor() {
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void MySetCursor(int x, int y) {
	COORD pos = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD MyGetCursor() {
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}
void RunGame() {
	system("mode con cols=150 lines=44");
	while (1) {
		MySetCursor(50, 5);
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 28; j++) {
				MySetCursor(50 + j * 2, 5 + i);
				if (title[i][j] == 1)
					printf("■");
			}
			printf("\n");
		}//show title
		MySetCursor(70, 20);
		printf("1 : PLAY GAME");

		MySetCursor(70, 22);
		printf("2 : HELP");

		MySetCursor(70, 24);
		printf("3 : EXIT");

		char selectMenu = getch();
		//show menu and receive key input
		if (selectMenu == '1') {
			StartGame();
			break;
		}
		else if (selectMenu == '2') {
			// 도움말
		}
		else if (selectMenu == '3') {
			ExitGame();
			break;
		}
		else {
			continue;
		}
	}
}
void StartGame() {
	system("cls");
	DrawGameBoard();//draw the road of game screen
	MySetCursor(0, 26);
	PrintHelpMenu();
	MySetCursor(96, 31);
	for (int i = 0; i < life; i++)
	{
		printf("♥");
	}

	NPC *mon;
	mon = MakeMonster();
	while (1)
	{
		//if (isGameOver()) { break; }
		while (1)
		{
			KeyInput();
			if (start_flag == 1)
			{
				start_flag = MoveMonster(mon);
			}
		}
	}
}

void ShowHelp() {
	system("cls");
}

void ExitGame() {
	system("cls");
	printf("소프트웨어 기초설계 2반 6조\n");
	printf("박상우\n이다훈\n이종원\n박철우\n");
	printf("===================================\n");
	printf("contact us : psw7205@gmail.com\n");
	printf("https://github.com/psw7205/SWDesign\n");
}
void DrawGameBoard() {
	for (int i = 0; i < 44; i++) {
		for (int j = 0; j < 60; j++) {
			MySetCursor(j * 2, i);
			if (mapModel[i][j] == 1)
				printf("#");
		}
		printf("\n");
	}
} //draw the road of game screen

void ShiftRight() {
	curPosX += 2;
	MySetCursor(curPosX, curPosY);
	return;
}

void ShiftLeft() {
	if (!(curPosX - 2 < 0))curPosX -= 2;
	MySetCursor(curPosX, curPosY);
	return;
}

void ShiftDown() {
	curPosY++;
	MySetCursor(curPosX, curPosY);
	return;
}

void ShiftUp() {
	curPosY--;
	MySetCursor(curPosX, curPosY);
}

void KeyInput() {
	int key;
	for (int i = 0; i < 20; i++) {
		if (_kbhit() != 0) {
			key = _getch();
			switch (key) {
			case LEFT:
				ShiftLeft();
				break;
			case RIGHT:
				ShiftRight();
				break;
			case UP:
				ShiftUp();
				break;
			case DOWN:
				ShiftDown();
				break;
			case 'q':
			case 'w':
			case 'e':
			case 'r':
				MakeTower(key);
				break;
			case 's':
				start_flag = 1;
				break;
			}
		}
		Sleep(1);
	}
	PrintCurPos();
}

///// 몬스터 부분.
NPC* MakeMonster() {
	NPC *mon;
	int i;
	int num = 0;
	int size = 10; //
	mon = (NPC*)malloc(size * sizeof(NPC));
	for (i = 0; i < size; i++)
	{
		mon[i].curx = num;
		mon[i].cury = 4;
		mon[i].hp = 100;
		mon[i].shape = 0;
		mon[i].move_flag = 1;
		num = num - 10;
	}
	return mon;
}

void ShowMonster(char monsterInfo[2][2], int mx, int my) { // 1로 되어있는 곳을 보여준다.
	int x, y;

	for (y = 0; y < 2; y++) {
		for (x = 0; x < 2; x++) {
			MySetCursor(mx + (x * 2), my + y);

			if (monsterInfo[y][x] == 1) {
				printf("★");
			}
		}
	}
	MySetCursor(mx, my);
}

void DeleteMonster(char monsterInfo[2][2], int mx, int my) { // 1로 되어있는 곳을 지운다.
	int x, y;

	for (y = 0; y < 2; y++) {
		for (x = 0; x < 2; x++) {
			MySetCursor(mx + (x * 2), my + y);

			if (monsterInfo[y][x] == 1) {
				printf(" ");
			}
		}
	}
	MySetCursor(mx, my);
}

int MoveMonster(NPC *mon) {
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		if (mon[i].move_flag == 1)
		{
			if (mon[i].curx >= 0)
			{
				DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			}
			mon[i].curx = mon[i].curx + 2;
			MySetCursor(mon[i].curx, mon[i].cury);
			if (mon[i].curx >= 0)
			{
				ShowMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			}
			if (mon[i].curx == 110) { mon[i].move_flag = 2; }
		}
		if (mon[i].move_flag == 2)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].cury = mon[i].cury + 1;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			if (mon[i].cury == 19) { mon[i].move_flag = 3; }
		}
		if (mon[i].move_flag == 3)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].curx = mon[i].curx - 2;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			if (mon[i].curx == 8) { mon[i].move_flag = 4; }
		}
		if (mon[i].move_flag == 4)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].cury = mon[i].cury - 1;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			if (mon[i].cury == 11) { mon[i].move_flag = 5; }
		}
		if (mon[i].move_flag == 5)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].curx = mon[i].curx + 2;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			if (mon[i].curx == 60) { mon[i].move_flag = 6; }
		}
		if (mon[i].move_flag == 6)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].cury = mon[i].cury + 1;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			if (mon[i].cury == 27) { mon[i].move_flag = 7; }
		}
		if (mon[i].move_flag == 7)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].curx = mon[i].curx + 2;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			if (mon[i].curx == 118)
			{
				mon[i].move_flag = 0;
				DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
				mon[0].curx = 0; // 사라지면 다시 x, y좌표 원래대로.
				mon[i].curx = mon[i - 1].curx - 10;
				mon[i].cury = 4;
			}
		}
		if (mon[9].move_flag == 0)
		{
			for (i = 0; i < 10; i++)
			{
				mon[i].move_flag = 1;
			}

			///// if 마지막 몬스터가 없어지거나 모든 몬스터가 없을 경우
			start_flag = 0;
			/////
			return 0;
		}
	}
	return 1;
}
///// 몬스터 부분.

void PrintCurPos() {
	MySetCursor(0, 40);
	printf("%d %d", curPosX, curPosY);
	MySetCursor(curPosX, curPosY);
}

void MakeTower(int type) {

	if (mapModel[curPosY + 2][curPosX / 2] == 0
		&& mapModel[curPosY + 1][curPosX / 2] == 0
		&& mapModel[curPosY + 1][curPosX / 2 + 1] == 0
		&& mapModel[curPosY + 2][curPosX / 2 + 1] == 0) {
		switch (type)
		{
		case 'q':
			if (gold < 100)
				break;
			printf("q★");
			MySetCursor(curPosX, curPosY + 1);
			printf("■■");
			AddTowerMap('q', curPosY, curPosX);
			gold -= 100;
			addTower(&TowerList, 'q', curPosY + 1, curPosX / 2);
			break;
		case 'w':
			if (gold < 200)
				break;
			printf("w☆");
			MySetCursor(curPosX, curPosY + 1);
			printf("■■");
			AddTowerMap('w', curPosY, curPosX);
			addTower(&TowerList, 'w', curPosY + 1, curPosX / 2);
			gold -= 200;
			break;
		case 'e':
			if (gold < 200)
				break;
			printf("e◎");
			MySetCursor(curPosX, curPosY + 1);
			printf("■■");
			AddTowerMap('e', curPosY, curPosX);
			addTower(&TowerList, 'e', curPosY + 1, curPosX / 2);
			gold -= 200;
			break;
		case 'r':
			if (gold < 300)
				break;
			printf("r◈");
			MySetCursor(curPosX, curPosY + 1);
			printf("■■");
			AddTowerMap('r', curPosY, curPosX);
			addTower(&TowerList, 'r', curPosY + 1, curPosX / 2);
			gold -= 300;
			break;
		}
		MySetCursor(0, 26);
		PrintHelpMenu();
	}
}

void PrintHelpMenu()
{
	printf("┌─────────────────────────────────────────────────────┐\n");
	printf("│ 화살타워   - Q 100골드             현재 골드  - %4d│\n", gold);
	printf("│ 대포타워   - W 200골드             스테이지   -  1  │\n");
	printf("│ 슬로우타워 - E 200 골드                             │\n");
	printf("│ 미사일타워 - R 300 골드            폭탄       -  A  │\n");
	printf("└─────────────────────────────────────────────────────┘\n");
}

void Hit(int bx, int by, int ex, int ey, int id, int *hp, int damage)
{

}