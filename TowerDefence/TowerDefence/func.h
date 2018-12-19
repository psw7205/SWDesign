#pragma once

#pragma comment(lib,"winmm.lib")
#pragma warning (disable:4996)
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27

#define SPEED 10
#define TIME 150

COORD MyGetCursor();
void MySetCursor(int x, int y);
void RemoveCursor();
void StartMenu();
void HelpGame();
void RunGame();
void StartGame();
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
void GameOver();
void ShowMonster(char monsterInfo[2][2], int shape, int hp, int max_hp, int mx, int my);
int curPosX, curPosY;
int start_flag = 0;
int DetectCollision(int posX, int posY);
int MoveMonster(NPC *mon);
void DC_chk(NPC *mon);
void InitMonster(NPC *mon, int i);
void DeleteTower();
void Bomb(NPC *mon);
void DeleteMonster(char monsterInfo[2][2], int mx, int my);
void PrintHelp();
void GameWin();
void GamePause();
NPC* MakeMonster();
void DeleteBullet(int tx, int ty);
void BuildTower(int type, int tx, int ty);

COORD start, end, first_corner;
int gold;
int life;
int Time;
int Stage;