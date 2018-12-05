#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "ItemDesign.h"
#include "Monsters.h"
#include "Tower.h"

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
void KeyInput(NPC *mon);
void PrintHelpMenu();
void PrintLife();
void SelectTower(int type);
int isGameOver();
void ShowMonster(char monsterInfo[2][2], int shape, int hp, int mx, int my);
int curPosX, curPosY;
int start_flag = 0;
int DetectCollision(int posX, int posY);
int MoveMonster(NPC *mon);
void DC_chk(NPC *mon);
void InitMonster(NPC *mon, int i);
void DeleteTower();
void bomb(NPC *mon);
void DeleteMonster(char monsterInfo[2][2], int mx, int my);

COORD start, end, first_corner;

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
	system("mode con cols=86 lines=44");
	while (1) {
		MySetCursor(16, 5);
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 28; j++) {
				MySetCursor(16 + j * 2, 5 + i);
				if (title[i][j] == 1)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					printf("■");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
			}
			printf("\n");
		}//show title
		MySetCursor(36, 20);
		printf("1 : PLAY GAME");

		MySetCursor(36, 22);
		printf("2 : HELP");

		MySetCursor(36, 24);
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
	MySetCursor(60, 35);
	PrintLife();
	MySetCursor(0, 32);
	PrintHelpMenu();

	NPC *mon;
	mon = MakeMonster();


	while (1)
	{
		if (isGameOver())
		{
			ExitGame();
			break;
		}

		KeyInput(mon);
		if (start_flag == 1)
		{
			start_flag = MoveMonster(mon);
		}
	}

}

int isGameOver()
{

	if (life <= 0)
		return 1;

	return 0;
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
	for (int i = 0; i < 33; i++) {
		for (int j = 0; j < 43; j++) {
			MySetCursor(j * 2, i);
			if (mapModel[i][j] == 1)
				printf("#");
		}
		printf("\n");
	}
} //draw the road of game screen

void ShiftRight() {
	curPosX += 2;
	if (curPosX > 84)
	{
		curPosX -= 2;
	}
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
	if (curPosY > 30)
	{
		curPosY--;
	}
	MySetCursor(curPosX, curPosY);
	return;
}

void ShiftUp() {
	curPosY--;
	if (curPosY < 0)
	{
		curPosY++;
	}
	MySetCursor(curPosX, curPosY);
}

void KeyInput(NPC *mon) {
	int key;
	for (int i = 0; i < 10; i++) {
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
				SelectTower(key);
				break;
			case 'd':
				DeleteTower();
				break;
			case 's':
				if (life == 0)
				{
					life = 10;
				}
				MySetCursor(60, 35);
				PrintLife();
				start_flag = 1;
				break;
			case 'a':
				if (start_flag == 1)
				{
					bomb(mon);
				}
				break;
			}
		}
		Sleep(1);
	}
	PrintCurPos();
}

///// 몬스터 부분.
void ShowMonster(char monsterInfo[2][2], int shape, int hp, int mx, int my) { // 1로 되어있는 곳을 보여준다.
	int x, y;

	for (y = 0; y < 2; y++) {
		for (x = 0; x < 2; x++) {
			MySetCursor(mx + (x * 2), my + y);
			switch (shape)
			{
			case 2:
				if (monsterInfo[y][x] == 1) {
					if (hp <= 120 && hp > 60) // %로 체력 설정 할 수 있으면 바꾸시오. 60%. 30%
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
						printf("火");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
					else if (hp <= 60)
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
						printf("火");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
					else
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
						printf("火");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				break;
			case 3:
				if (monsterInfo[y][x] == 1) {
					if (hp <= 120 && hp > 60)
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
						printf("木");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
					else if (hp <= 60)
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
						printf("木");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
					else
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
						printf("木");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				break;
			}

		}
	}
	MySetCursor(mx, my);
}

void bomb(NPC*mon) {
	int i;
	int bombprice = 1000;
	if (gold < bombprice) return;
	gold -= bombprice;

	for (int i = 0; i < 33; i++) {
		for (int j = 0; j < 43; j++) {
			MySetCursor(j * 2, i);
			if (mapModel[i][j] == 9)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				printf("★");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
		}
		printf("\n");
	}
	Sleep(100);
	for (int i = 0; i < 33; i++) {
		for (int j = 0; j < 43; j++) {
			MySetCursor(j * 2, i);
			if (mapModel[i][j] == 9)
				printf(" ");
		}
		printf("\n");
	}

	for (i = 0; i < 10; i++) {
		DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
		InitMonster(mon, i);// set up the in
	}

	MySetCursor(0, 32);
	PrintHelpMenu();
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

void InitMonster(NPC *mon, int i)
{
	mon[i].move_flag = 0;
	mon[i].curx = (-16 * i);
	mon[i].cury = 3;
	mon[i].hp = 200;
}

void DC_chk(NPC *mon)
{
	int bullet;
	int type;
	bullet = DetectCollision(mon->curx, mon->cury);
	type = bullet / 10;
	type %= 10;
	if (type >= 4) { type = mon->shape; }
	if (bullet != 0)
	{
		if (mon->shape == type)
		{
			mon->hp -= (bullet / 100);
			if (mon->hp <= 0) {
				DeleteMonster(monsterModel[0], mon->curx, mon->cury);
				gold += 50;
				mon->move_flag = 0;

				MySetCursor(0, 32);
				PrintHelpMenu();
			}
		}

		printf("%d", mon->hp);
	}
}

int MoveMonster(NPC *mon) {
	int i = 0;
	int bullet = 0;
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
				ShowMonster(monsterModel[0], mon[i].shape, mon[i].hp, mon[i].curx, mon[i].cury);
				DC_chk(&mon[i]);
			}
			if (mon[i].curx == 76) { mon[i].move_flag = 2; }
		}
		if (mon[i].move_flag == 2)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].cury = mon[i].cury + 1;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].shape, mon[i].hp, mon[i].curx, mon[i].cury);
			DC_chk(&mon[i]);
			if (mon[i].cury == 19) { mon[i].move_flag = 3; }
		}
		if (mon[i].move_flag == 3)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].curx = mon[i].curx - 2;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].shape, mon[i].hp, mon[i].curx, mon[i].cury);
			DC_chk(&mon[i]);
			if (mon[i].curx == 8) { mon[i].move_flag = 4; }
		}
		if (mon[i].move_flag == 4)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].cury = mon[i].cury - 1;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].shape, mon[i].hp, mon[i].curx, mon[i].cury);
			DC_chk(&mon[i]);
			if (mon[i].cury == 11) { mon[i].move_flag = 5; }
		}
		if (mon[i].move_flag == 5)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].curx = mon[i].curx + 2;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].shape, mon[i].hp, mon[i].curx, mon[i].cury);
			DC_chk(&mon[i]);
			if (mon[i].curx == 42) { mon[i].move_flag = 6; }
		}
		if (mon[i].move_flag == 6)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].cury = mon[i].cury + 1;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].shape, mon[i].hp, mon[i].curx, mon[i].cury);
			DC_chk(&mon[i]);
			if (mon[i].cury == 27) { mon[i].move_flag = 7; }
		}
		if (mon[i].move_flag == 7)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].curx = mon[i].curx + 2;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].shape, mon[i].hp, mon[i].curx, mon[i].cury);
			DC_chk(&mon[i]);
			if (mon[i].curx >= 80) // 몬스터가 끝까지 갔을 경우.
			{
				DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
				InitMonster(mon, i);
				start_flag = 0;

				life--;
				MySetCursor(60, 35);
				PrintLife();
			}
		}
	}
	if (mon[9].move_flag == 0)
	{
		for (i = 0; i < 10; i++)
		{
			InitMonster(mon, i);
			mon[i].move_flag = 1;
		}
		return 0;
	}
	return 1;
}
///// 몬스터 부분.

void PrintCurPos() {
	MySetCursor(78, 37);
	printf("(%2d, %2d)", curPosX, curPosY);
	MySetCursor(curPosX, curPosY);
}

int DetectCollision(int posX, int posY) { // 충돌체크.
	int x, y;
	int dX, dY;
	int tmp;
	dX = posX / 2;
	dY = posY;
	for (x = 0; x < 2; x++) {
		for (y = 0; y < 2; y++) {
			tmp = TowerModel[dY + y][dX + x];
			if (tmp != 1) {
				return tmp;
			}
		}
	}
	return 0;
}

void SelectTower(int type) {

	if (TowerModel[curPosY + 1][curPosX / 2] % 10 != 1
		&& TowerModel[curPosY][curPosX / 2] % 10 != 1
		&& TowerModel[curPosY][curPosX / 2 + 1] % 10 != 1
		&& TowerModel[curPosY + 1][curPosX / 2 + 1] % 10 != 1
		&& TowerModel[curPosY + 1][curPosX / 2] % 10 != 2
		&& TowerModel[curPosY][curPosX / 2] % 10 != 2
		&& TowerModel[curPosY][curPosX / 2 + 1] % 10 != 2
		&& TowerModel[curPosY + 1][curPosX / 2 + 1] % 10 != 2
		&& TowerModel[curPosY + 1][curPosX / 2] % 10 != 3
		&& TowerModel[curPosY][curPosX / 2] % 10 != 3
		&& TowerModel[curPosY][curPosX / 2 + 1] % 10 != 3
		&& TowerModel[curPosY + 1][curPosX / 2 + 1] % 10 != 3
		&& TowerModel[curPosY + 1][curPosX / 2] % 10 != 4
		&& TowerModel[curPosY][curPosX / 2] % 10 != 4
		&& TowerModel[curPosY][curPosX / 2 + 1] % 10 != 4
		&& TowerModel[curPosY + 1][curPosX / 2 + 1] % 10 != 4
		&& TowerModel[curPosY + 1][curPosX / 2] % 10 != 5
		&& TowerModel[curPosY][curPosX / 2] % 10 != 5
		&& TowerModel[curPosY][curPosX / 2 + 1] % 10 != 5
		&& TowerModel[curPosY + 1][curPosX / 2 + 1] % 10 != 5)
	{
		switch (type)
		{
		case 'q':
			if (gold < 100)
				break;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
			BuildTower(type, curPosX, curPosY);
			printf(" ◆");
			MySetCursor(curPosX, curPosY + 1);
			printf("◆◆");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			gold -= 100;
			break;
		case 'w':
			if (gold < 100)
				break;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			BuildTower(type, curPosX, curPosY);
			printf(" ◆");
			MySetCursor(curPosX, curPosY + 1);
			printf("◆◆");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			gold -= 100;
			break;
		case 'e':
			if (gold < 200)
				break;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
			BuildTower(type, curPosX, curPosY);
			printf(" ★");
			MySetCursor(curPosX, curPosY + 1);
			printf("★★");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			gold -= 200;
			break;
		case 'r':
			if (gold < 300)
				break;
			BuildTower(type, curPosX, curPosY);
			printf(" ◈");
			MySetCursor(curPosX, curPosY + 1);
			printf("◈◈");
			gold -= 300;
			break;
		}
		MySetCursor(0, 32);
		PrintHelpMenu();
	}
}
void DeleteTower()
{
	int tx, ty;
	tx = curPosX / 2;
	ty = curPosY;
	if (TowerModel[ty][tx] % 10 != 1 &&
		TowerModel[ty][tx + 1] % 10 != 1 &&
		TowerModel[ty + 1][tx] % 10 != 1 &&
		TowerModel[ty + 1][tx + 1] % 10 != 1 &&
		TowerModel[ty][tx] % 10 != 0 &&
		TowerModel[ty][tx + 1] % 10 != 0 &&
		TowerModel[ty + 1][tx] % 10 != 0 &&
		TowerModel[ty + 1][tx + 1] % 10 != 0)
	{
		printf("    ");
		MySetCursor(curPosX, curPosY + 1);
		printf("    ");
		DeleteBullet(curPosX, curPosY);
		gold += 50;
		MySetCursor(0, 32);
		PrintHelpMenu();
	}
}
void PrintHelpMenu()
{
	printf("┌─────────────────────────────────────────────────────┐\n");
	printf("│ 물 타워   - Q  100 골드           현재 골드  - %4d │\n", gold);
	printf("│ 불 타워   - W  100 골드           스테이지   -  1   │\n");
	printf("│ 뇌 타워   - E  200 골드                             │\n");
	printf("│ 올 타워   - R  300 골드                             │\n");
	printf("│ 타워 삭제 - D  +50 골드                             │\n");
	printf("│ 폭탄      - A 1000 골드                             │\n");
	printf("└─────────────────────────────────────────────────────┘\n");
}

void PrintLife()
{
	int i;
	MySetCursor(60, 35);
	if (life == 10)
	{
		for (i = 0; i < life; i++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			printf("♥");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
	else
	{
		for (i = 0; i < 10 - life; i++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			printf("♡");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
}
