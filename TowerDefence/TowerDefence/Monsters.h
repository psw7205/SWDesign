#pragma once
#include<stdlib.h>

typedef struct node {
	int HP;
	int curx;
	int cury;
	int prevx;
	int prevy;
	int corner = -1;
	char shape = '¢²';
	node*next;
}node;

node* makemob(int hp, int x, int y, char s) {
	node * make = (node *)malloc(sizeof(node));
	make->HP = hp;
	make->curx = x;
	make->cury = y;
	make->shape = s;
	make->next = NULL;
	make->corner = -1;
	return make;
}

void removeall(node*head) {
	node*cur = head->next;
	while (cur->next != NULL) {
		node*tmp = cur;
		cur = cur->next;
		free(tmp);
	}
}

void removenum(node*head, int i) {
	node *cur = head->next;
	node *prev = head;
	for (int a = 0; a < i; a++) {
		cur = cur->next;
		prev = prev->next;
	}
	prev->next = cur->next;
	free(cur);
}