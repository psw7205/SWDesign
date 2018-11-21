#pragma once
#pragma once
#include "Monsters.h"
typedef struct Bullet
{
	int x;
	int y;
	int speed;
	int damage;
	struct Bullet *next;
}Bullet;

Bullet* makeBullet(int type, int x, int y)
{
	Bullet *newBullet = (Bullet*)malloc(sizeof(Bullet));

	newBullet->x = x;
	newBullet->y = y;
	newBullet->next = NULL;

	switch (type)
	{
	case 'q':
		newBullet->speed = 10;
		newBullet->damage = 10;
		break;
	case 'w':
		newBullet->speed = 15;
		newBullet->damage = 20;
		break;
	case 'e':
		newBullet->speed = 15;
		newBullet->damage = 20;
		break;
	case 'r':
		newBullet->speed = 20;
		newBullet->damage = 30;
		break;
	default:
		break;
	}

	return newBullet;
}

void Shot(int x, int y, int speed)
{
	x += speed;
	y += speed;
}

void addBullet(Bullet **head, int type, int x, int y)
{
	Bullet *newBullet = makeBullet(type, x, y);
	Bullet *it = *head;

	if (*head == NULL)
	{
		*head = newBullet;
		return;
	}

	while (it->next != NULL)
	{
		it = it->next;
	}

	it->next = newBullet;
	return;
}