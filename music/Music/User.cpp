#define _CRT_SECURE_NO_WARNINGS
#include "User.h"
#include "md5.h"
#include "mytime.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <memory.h>
#include <string.h>
#include <iostream>
#include "UserInteraction.h"

const char* USER_FILE = "users.dat";

//записать пользователей в файл
void usersWrite(struct User* head)
{
	FILE* outfile;

	// открываем файл для записи
	outfile = fopen(USER_FILE, "wb");
	if (outfile == NULL)
	{
		fprintf(stderr, "\nОшибка записи в файл\n");
		PressAnyKey();
		return;
	}

	struct User* iterator = head;

	// записываем структуру в файл 
	while (iterator != NULL)
	{
		//fwrite(&iterator, sizeof(struct User), 1, outfile);
		fwrite(iterator->login, sizeof(iterator->login), 1, outfile);
		fwrite(iterator->pass_hash, sizeof(iterator->pass_hash), 1, outfile);
		fwrite(&iterator->is_admin, sizeof(iterator->is_admin), 1, outfile);
		fwrite(&iterator->date, sizeof(iterator->date), 1, outfile);
		if (iterator->next == NULL) //конец списка
			break;
		iterator = iterator->next;
	}

	if (fwrite == 0)
		printf("\nОшибка записи в файл пользователей!\n");

	// закрываем файл
	fclose(outfile);
}

struct User* usersRead()
{
	//struct User* head = (struct User*) malloc(sizeof(struct User));
	struct User* head = NULL; //вершина списка
	struct User* prev = NULL; //предыдущий считанный пользователь
	struct User* tail; //последний считанный пользователь
	FILE* fp = fopen(USER_FILE, "rb"); //открываем файл
	if (fp == NULL) //файл не открыт
	{
		fprintf(stderr, "\nОшибка чтения файла пользователей\n");
		PressAnyKey();
		exit(0);
		
		
	}
	while (!feof(fp))
	{
		tail = (struct User*) malloc(sizeof(struct User)); //выделяем память
		//считываем данные
		fread(tail->login, sizeof(head->login), 1, fp);
		if (tail->login[0] == -51) break; //конец файла
		//fread(tail->password_hash, sizeof(head->password_hash), 1, fp);
		fread(tail->pass_hash, sizeof(head->pass_hash), 1, fp);
		fread(&tail->is_admin, sizeof(head->is_admin), 1, fp);
		fread(&tail->date, sizeof(head->date), 1, fp);
		tail->next = NULL;
		//помещаем считанного пользователя в список и перемещаем указатель на вершину списка
		if (head == NULL) //При первом запуске head == NULL
		{
			tail->next = head;
			head = tail;
			prev = head;
		}
		else
		{
			prev->next = tail;
			prev = tail;
		}
	}
	fclose(fp);

	return head;
}

void addUser(struct User** head, char* log, char* pass, bool isAdmin)
{
	if (!isValidLogin(log)) { //если логин не валидный не добавляем пользователя
		PressAnyKey();
		return;
	}

	if (isExists(*head, log)) {
		printf("\nПользователь с данным логином уже существует\n");
		PressAnyKey();
		return;
	}

	//добавление нового пользователя(в начало списка)
	//выделяем память для нового пользователя
	struct User* link = (struct User*) malloc(sizeof(struct User));

	//копируем логин
	memcpy(link->login, log, strlen(log));
	//добавляем пустой символ
	if (strlen(log) < LOG_IN)
		link->login[strlen(log)] = '\0';
	/*//копируем пароль
	memcpy(link->password_hash, pass, strlen(pass));
	//добавляем пустой символ
	if (strlen(pass) < PASS_IN)
		link->password_hash[strlen(pass)] = '\0';*/

	uint8_t hash[16];
	md5_algo((uint8_t*)pass, strlen(pass), hash);
	memcpy(link->pass_hash, hash, sizeof(hash));

	//копируем признак администратора
	link->is_admin = isAdmin;
	//получаем текущую дату
	link->date = getCurrentTime();
	link->next = NULL;

	if (*head == NULL)
	{
		*head = link;
	}
	else {
		//помещаем указатель на вершину старого списка
		link->next = *head;
		//меняем указатель на вершину списка на созданного пользователя
		*head = link;
	}

	//перезаписываем файл с пользователями
	usersWrite(*head);
	printf("\nПользователь добавлен!\n");
	PressAnyKey();
}

//удаляет пользователя по номеру
void deleteUser(struct User** head, int key)
{
	int n = 1;
	if (key == 1) {
		deleteFirstUser(head);
		return;
	}

	//Начинаем с вершины списка
	struct User* current = *head;
	struct User* previous = NULL;

	//список пуст
	if (head == NULL) {
		return;
	}

	//проходим до n-го пользователя
	while (n != key && current != NULL) {
		//если искомый пользователь - последний
		if (current->next == NULL && n == key) {
			free(current); //освобождаем память
			previous->next = NULL;
			usersWrite(*head);
			return;
		}
		else {
			//сохраняем текущую ссылку
			previous = current;
			//переходим к следующей ссылке
			current = current->next;
		}
		n++;
	}

	//обходим текущую ссылку
	if (current != NULL) {
		previous->next = current->next;
		usersWrite(*head); //сохраняем изменения
	}
	printf("\nЗапись о пользователе успешно удалена!\n");
	return;
}

void do_not_delete_main_admin(struct User** head, int key)
{
	int n = 1;
	if (key == 1) {
		deleteFirstUser(head);
		printf("\nПользователь удален!\n");
		PressAnyKey();
		return;
	}

	//Начинаем с вершины списка
	struct User* current = *head;
	struct User* previous = NULL;

	//список пуст
	if (head == NULL) {
		return;
	}

	//проходим до n-го пользователя
	while (n != key && current != NULL) {
		
			//сохраняем текущую ссылку
			previous = current;
			//переходим к следующей ссылке
			current = current->next;
		n++;
	}
	if (current->next == NULL && n == key) {
		printf("\nНевозможно удалить главного администратора!\n");
		PressAnyKey();
		return;
	}
	//обходим текущую ссылку
	if (current != NULL) {
		previous->next = current->next;
		usersWrite(*head); //сохраняем изменения
	}
	printf("\nПользователь удален!\n");
	PressAnyKey();
	return;
}

// удаляет первого пользователя в списке
void deleteFirstUser(struct User** head)
{
	//Помечаем следующего пользователя за первым как вершину списка 
	//head = head->next;
	//usersWrite(head); //записываем изменения в файл
	struct User* tmp = (*head)->next;
	free(*head); //очищаем, чтобы не было утечки памяти
	*head = tmp;
	usersWrite(*head);
}

//печатает в консоль список пользователей, возвращает количество пользователей
int printUsersList(struct User* head)
{
	if (head == NULL) {
		printf("Пользователи не были созданы.");
		return 0;
	}
	struct User* ptr = head;
	printf("\n");
	printf("\n\n");
	printf("+-----+--------------------+--------------------+-----------------+\n");
	printf("|Номер|       Логин        |        Роль        | Дата регистрации|\n");
	printf("+-----+--------------------+--------------------+-----------------+\n");
	int n = 1; //номер пользователя в списке
	while (ptr != NULL) {
		char pos[14];
		if (ptr->is_admin)
			strcpy(pos, "Администратор\0");
		else
			strcpy(pos, "Пользователь\0");
		printf("|%5d|%20s|%20s|%17s|\n", n, ptr->login, pos, timeToChar(ptr->date));
		ptr = ptr->next; //переходим на следующего пользователя
		n++;
	}
	printf("+-----+--------------------+--------------------+-----------------+\n");
	return n++;
}

bool authorize(struct User* head, char login[], char password[], bool isAdmin)
{
	//администратор может быть авторизован и как администратор, и как обычный пользователь.
	//обычный пользователь может быть авторизован только как пользователь.
	struct User* ptr = head; //указатель на пользователей в списке
	while (ptr != NULL) {
		if (strcmp(login, ptr->login) == 0) {
			uint8_t hash[16];
			md5_algo((uint8_t*)password, strlen(password), hash);
			bool passwords_equals = true;
			for (int i = 0; i < 16; i++) {
				if (hash[i] != ptr->pass_hash[i]) {
					passwords_equals = false;
					break;
				}
			}
			if (passwords_equals) { //пароль верный
				if (isAdmin == false || ptr->is_admin == true) {
					return true;
				}
				else
				{
					printf("\n\nНедостаточно прав\n");
					return false;
				}
			}
			else {
				printf("\nНеверный пароль\n");
				return false;
			}
		}
		ptr = ptr->next; //переходим на следующего пользователя
	}
	printf("\nНе найден пользователь с данным логином\n");
	return false;
}

//проверяет правильность логина, все символы которого должны быть буквами
bool isValidLogin(char* log)
{
	if (strlen(log) < 3) //длина логина меньше трех символов
		return false;
	int i = 0;
	for (int i = 0; i < strlen(log); i++) //проходим по символам
	{
		if (!((log[i] >= 65 && log[i] <= 90) || (log[i] >= 97 && log[i] <= 122))) {
			printf("\nОшибка. Все символы логина и пароля должны быть буквами английского алфавита.\n", log[i]);
			return false;
		}
		
	}
	return true;
}


bool isExists(User* head, char* log)
{
	struct User* ptr = head;
	while (ptr != NULL) {
		if (strcmp(log, ptr->login) == 0)
			return true;
		ptr = ptr->next;
	}

	return false;
}

