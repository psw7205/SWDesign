#pragma once
#include <stdlib.h>
typedef struct Tower {
	int ID;
	int curx;
	int cury;
	int range;
	Tower *next;
}Tower;

Tower* SetTower(int ID, int x, int y) {
	Tower * tmp = (Tower *)malloc(sizeof(Tower));
	tmp->ID = ID;
	tmp->curx = x;
	tmp->cury = y;
	tmp->next = NULL;

	switch (ID)
	{
	case 'q':
		tmp->range = 2;
		break;
	case 'w':
		tmp->range = 4;
		break;
	case 'e':
		tmp->range = 4;
		break;
	case 'r':
		tmp->range = 8;
		break;
	}

	return tmp;
}

void RemoveTowerNum(Tower *head, int i) {
	Tower *cur = head->next;
	Tower *prev = head;
	for (int a = 0; a < i; a++) {
		cur = cur->next;
		prev = prev->next;
	}
	prev->next = cur->next;
	free(cur);
}

void addTower(Tower **head, int type, int x, int y)
{
	Tower *newTower = SetTower(type, x, y);
	Tower *it = *head;

	if (*head == NULL)
	{
		*head = newTower;
		return;
	}

	while (it->next != NULL)
	{
		it = it->next;
	}

	it->next = newTower;
	return;
}