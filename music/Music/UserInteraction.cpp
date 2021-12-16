#define _CRT_SECURE_NO_WARNINGS
#include "UserInteraction.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <iostream>
#include <windows.h> 
#include <conio.h>

//заправшивает у пользователя переменную типа int и заносит в `value`
bool _EntryIntValue(const char* name, int& value, int min, int max)
{
	// Число для ввода, статус проверки ввода
	int temp, check;
	// Вывести информацию о параметре
	if (name != NULL && name != "") printf("\n%s", name);

	// Если диапазон не задан, то любое значение
	if (min == INT_MAX && max == INT_MIN)
		printf(":>");
	// Если задан только максимум, то значение может быть не больше заданного
	else if (min == INT_MAX && max != INT_MIN)
		printf(" [< %d]:>", max);
	// Если задан только минимум, то значение может быть не меньше заданного
	else if (min != INT_MAX && max == INT_MIN)
		printf(" [> %d]:>", min);
	// Если задан максимум и минимум, то значение находится в диапазоне
	else
		printf(" [%d..%d]:>", min, max);

	// Ввести значение числа
	check = scanf_s("%d", &temp);

	//неверный ввод, очищаем буфер
	if (check != 1 || _fgetchar() != 10) {
		int c = 0;
		while (getchar() != '\n');
		printf("\nОшибка: Значение должно быть ЦЕЛЫМ числом от %d до %d\n", min, max);
		return false;
	}

	// Проверить корректность ввода и допустимость диапазона
	if (temp < min || temp > max)
	{
		fflush(stdin);
		// Вывести сообщение об ошибке
		printf("\nОшибка: Значение должно быть ЦЕЛЫМ числом от %d до %d\n", min, max);
		return false;
	}
	else
	{
		// Вернуть значене через аргумент
		value = temp;
		return true;
	}
}
//Вызывает `EntryIntegerValue` пока не будет дан верный ответ
void EntryIntValue(const char* name, int& value, int min, int max)
{
	while (!_EntryIntValue(name, value, min, max)) {}
}

//Запрашивает ввод любой клавиши
void PressAnyKey()
{
	printf("\nНажмите любую клавишу, чтобы продолжить...\n");
	getchar();
}

bool myIsUpper(char c) {
	if (c >= 65 && c <= 90) //символы английского алфавита большими буквами
		return true;
	if (c >= -64 && c <= -33) //символы русского алфавита большими буквами
		return true;
	printf("\n%c не является заглавной буквой.\n", c);
	return false;
}

char* ToRus(wchar_t* str) {
	static char s[1024];
	CharToOem(LPCWSTR(str), s);
	return s;
}

//запрашивает у пользователя переменную типа char[] и заносит в `value`
void EntryCharValue(const char* request, char*& value, int min, int max, bool needUpper) {
	while (true) {//пока не введено
		printf("\n%s[%d...%d]", request, min, max); //печатаем запрос
		char* a = (char*)malloc(500); //выделяем память
		if (fgets(a, max, stdin) == NULL) {
			printf("\nОшибка чтения");
			free(a);
			continue;
		}
		char c;
		int i = strlen(a);
		if (a[0] == '\n') {
			free(a);
			printf("\nОшибка ввода. Необходимо ввести строку длиной от %d до %d\n", min, max);
			continue;
		}
		if (strlen(a)>max) { //обработка переполнения буфера
			while ((c = getchar()) != '\n' && c != EOF)	{}
			printf("\nОшибка ввода. Необходимо ввести строку длиной от %d до %d\n", min, max);
			free(a);
			continue;
		}
		char first = a[0];

		if ((!needUpper || (needUpper && myIsUpper(a[0]))) && strlen(a) < max && strlen(a) > min) { //если верная длина строки

			value = (char*)malloc(strlen(a) / sizeof(a[0])); //выделяем память
			memcpy(value, a, strlen(a)); //копируем в `value`
			if (strlen(value) < max)
				value[strlen(a) - 1] = '\0'; //добавляем пустой символ
			else
				value[strlen(a)] = '\0';
			while (value[strlen(value)] == '\n') //убираем перенос строки
				value[strlen(value)] = '\0';
			while (value[strlen(value) - 1] == '\n')//убираем перенос строки
				value[strlen(value) - 1] = '\0';
			free(a); //освобождаем память
			return;
		}
		printf("\n->ошибка ввода.\n", min, max);
		//освобождаем память
		free(a);
	}
}

//запрашивает у пользователя пароль
void EntryPass(const char* request, char*& value, int min, int max, bool needUpper) {

	while (true) {//пока не введено
		printf("\n%s[%d...%d]", request, min, max); //печатаем запрос
		char* a = (char*)malloc(500); //выделяем память
		char c;
		int k, i = strlen(a);
		for (k = 0; (a[k] = _getch()) != '\r';) {
			if (a[k] == '\b' && k != 0) {
				printf("\b \b");
				k--;
			}
			else if (a[k] != '\b') {
				printf("*");
				k++;
			}
		}
		a[k + 1] = '\0';

		if (a[0] == '\n') {
			free(a);
			printf("\nВы ничего не ввели. Необходимо ввести строку длиной от %d до %d\n", min, max);
			continue;
		}


		if (strlen(a) > max) { //обработка переполнения буфера
			while ((c = getchar()) != '\n' && c != EOF) {}
			printf("\nОшибка ввода. Необходимо ввести строку длиной от %d до %d\n", min, max);
			free(a);
			continue;
		}
		char first = a[0];

		if ((!needUpper || (needUpper && myIsUpper(a[0]))) && strlen(a) < max && strlen(a) > min) { //если верная длина строки

			value = (char*)malloc(strlen(a) / sizeof(a[0])); //выделяем память
			memcpy(value, a, strlen(a)); //копируем в `value`
			if (strlen(value) < max)
				value[strlen(a) - 1] = '\0'; //добавляем пустой символ
			else
				value[strlen(a)] = '\0';
			while (value[strlen(value)] == '\n') //убираем перенос строки
				value[strlen(value)] = '\0';
			while (value[strlen(value) - 1] == '\n')//убираем перенос строки
				value[strlen(value) - 1] = '\0';
			free(a); //освобождаем память
			return;
		}
		printf("\nОшибка ввода. Необходимо ввести строку длиной от %d до %d\n", min, max);
		//освобождаем память
		free(a);
	}
}
