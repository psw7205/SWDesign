#pragma once

typedef struct NPC {
	int hp;
	int curx;
	int cury;
	int move_flag;
	char shape; // ���� ���� �÷���.
}NPC;

char monsterModel[][2][2] = {
	{
		{ 1, 1 },
		{ 1, 1 }
	},
	{
		{ 1, 1 },
		{ 1, 1 }
	},
	{
		{ 1, 1 },
		{ 1, 1 }
	},
	{
		{ 1, 1 },
		{ 1, 1 }
	}
}; // ���� ���� �����.

NPC* MakeMonster() {
	NPC *mon;
	int i;
	int num = 0;
	int size = 10; //
	mon = (NPC*)malloc(size * sizeof(NPC));
	for (i = 0; i < size; i++)
	{
		mon[i].curx = num;
		mon[i].cury = 3;
		mon[i].hp = 200;
		if (i < 5)
		{
			mon[i].shape = 2;
		}
		else
		{
			mon[i].shape = 3;
		}
		mon[i].move_flag = 1;
		num = num - 16;
	}
	return mon;
}