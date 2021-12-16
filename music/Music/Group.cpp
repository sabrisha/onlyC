#include "Group.h"
#include <stdio.h>

//struct Group group;

const char* groupType(int type)
{
	switch (type)
	{
	case 0:
		return "Пианино";
	case 1:
		return "Гитара";
	case 2:
		return "Скрипка";
	case 3:
		return "Вокал";
	default:
		return "Не определено";
	}
	return nullptr;
}

void printGroupTypes()
{
	printf("0 — Пианино\n");
	printf("1 — Гитара\n");
	printf("2 — Скрипка\n");
	printf("3 — Вокал\n");
}
