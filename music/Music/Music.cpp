#define _CRT_SECURE_NO_WARNINGS
#include "Menus.h"
#include "UserInteraction.h"
#include "ConsoleSetUp.h"
#include <iostream>


int main()
{
	Console();//Настройки консоли
	ini();// инизиализация списков, загрузка пользователей
	menu1();//открытие главного меню
}
