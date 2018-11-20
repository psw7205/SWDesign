#pragma once
#include <stdlib.h>
typedef struct Tower {
	int ID;
	int curx;
	int cury;
	int bulletSpeed;
	int range;
	Tower *next;
}Tower;

Tower* makeTower(int ID, int x, int y) {
	Tower * tmp = (Tower *)malloc(sizeof(Tower));
	tmp->ID = ID;
	tmp->curx = x;
	tmp->cury = y;
	tmp->next = NULL;

	switch (ID)
	{
	case 'q':
		tmp->bulletSpeed = 10;
		break;
	case 'w':
		tmp->bulletSpeed = 20;
		break;
	case 'e':
		tmp->bulletSpeed = 10;
		break;
	case 'r':
		tmp->bulletSpeed = 30;
		break;
	}

	return tmp;
}
