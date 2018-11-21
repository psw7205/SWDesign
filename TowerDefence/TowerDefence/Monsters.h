#pragma once
#include<stdlib.h>

typedef struct NPC {
	int HP;
	int curx;
	int cury;
	int prevx;
	int prevy;
	int corner = -1;
	char shape;
	NPC*next;
}NPC;

NPC* MakeMob(int hp, int x, int y, char s) {
	NPC * make = (NPC *)malloc(sizeof(NPC));
	make->HP = hp;
	make->curx = x;
	make->cury = y;
	make->shape = s;
	make->next = NULL;
	make->corner = -1;
	return make;
}

void RemoveAll(NPC*head) {
	NPC*cur = head->next;
	while (cur->next != NULL) {
		NPC*tmp = cur;
		cur = cur->next;
		free(tmp);
	}
}

void RemoveNum(NPC*head, int i) {
	NPC *cur = head->next;
	NPC *prev = head;
	for (int a = 0; a < i; a++) {
		cur = cur->next;
		prev = prev->next;
	}
	prev->next = cur->next;
	free(cur);
}