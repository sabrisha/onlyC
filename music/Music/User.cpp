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

//�������� ������������� � ����
void usersWrite(struct User* head)
{
	FILE* outfile;

	// ��������� ���� ��� ������
	outfile = fopen(USER_FILE, "wb");
	if (outfile == NULL)
	{
		fprintf(stderr, "\n������ ������ � ����\n");
		PressAnyKey();
		return;
	}

	struct User* iterator = head;

	// ���������� ��������� � ���� 
	while (iterator != NULL)
	{
		//fwrite(&iterator, sizeof(struct User), 1, outfile);
		fwrite(iterator->login, sizeof(iterator->login), 1, outfile);
		fwrite(iterator->pass_hash, sizeof(iterator->pass_hash), 1, outfile);
		fwrite(&iterator->is_admin, sizeof(iterator->is_admin), 1, outfile);
		fwrite(&iterator->date, sizeof(iterator->date), 1, outfile);
		if (iterator->next == NULL) //����� ������
			break;
		iterator = iterator->next;
	}

	if (fwrite == 0)
		printf("\n������ ������ � ���� �������������!\n");

	// ��������� ����
	fclose(outfile);
}

struct User* usersRead()
{
	//struct User* head = (struct User*) malloc(sizeof(struct User));
	struct User* head = NULL; //������� ������
	struct User* prev = NULL; //���������� ��������� ������������
	struct User* tail; //��������� ��������� ������������
	FILE* fp = fopen(USER_FILE, "rb"); //��������� ����
	if (fp == NULL) //���� �� ������
	{
		fprintf(stderr, "\n������ ������ ����� �������������\n");
		PressAnyKey();
		exit(0);
		
		
	}
	while (!feof(fp))
	{
		tail = (struct User*) malloc(sizeof(struct User)); //�������� ������
		//��������� ������
		fread(tail->login, sizeof(head->login), 1, fp);
		if (tail->login[0] == -51) break; //����� �����
		//fread(tail->password_hash, sizeof(head->password_hash), 1, fp);
		fread(tail->pass_hash, sizeof(head->pass_hash), 1, fp);
		fread(&tail->is_admin, sizeof(head->is_admin), 1, fp);
		fread(&tail->date, sizeof(head->date), 1, fp);
		tail->next = NULL;
		//�������� ���������� ������������ � ������ � ���������� ��������� �� ������� ������
		if (head == NULL) //��� ������ ������� head == NULL
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
	if (!isValidLogin(log)) { //���� ����� �� �������� �� ��������� ������������
		PressAnyKey();
		return;
	}

	if (isExists(*head, log)) {
		printf("\n������������ � ������ ������� ��� ����������\n");
		PressAnyKey();
		return;
	}

	//���������� ������ ������������(� ������ ������)
	//�������� ������ ��� ������ ������������
	struct User* link = (struct User*) malloc(sizeof(struct User));

	//�������� �����
	memcpy(link->login, log, strlen(log));
	//��������� ������ ������
	if (strlen(log) < LOG_IN)
		link->login[strlen(log)] = '\0';
	/*//�������� ������
	memcpy(link->password_hash, pass, strlen(pass));
	//��������� ������ ������
	if (strlen(pass) < PASS_IN)
		link->password_hash[strlen(pass)] = '\0';*/

	uint8_t hash[16];
	md5_algo((uint8_t*)pass, strlen(pass), hash);
	memcpy(link->pass_hash, hash, sizeof(hash));

	//�������� ������� ��������������
	link->is_admin = isAdmin;
	//�������� ������� ����
	link->date = getCurrentTime();
	link->next = NULL;

	if (*head == NULL)
	{
		*head = link;
	}
	else {
		//�������� ��������� �� ������� ������� ������
		link->next = *head;
		//������ ��������� �� ������� ������ �� ���������� ������������
		*head = link;
	}

	//�������������� ���� � ��������������
	usersWrite(*head);
	printf("\n������������ ��������!\n");
	PressAnyKey();
}

//������� ������������ �� ������
void deleteUser(struct User** head, int key)
{
	int n = 1;
	if (key == 1) {
		deleteFirstUser(head);
		return;
	}

	//�������� � ������� ������
	struct User* current = *head;
	struct User* previous = NULL;

	//������ ����
	if (head == NULL) {
		return;
	}

	//�������� �� n-�� ������������
	while (n != key && current != NULL) {
		//���� ������� ������������ - ���������
		if (current->next == NULL && n == key) {
			free(current); //����������� ������
			previous->next = NULL;
			usersWrite(*head);
			return;
		}
		else {
			//��������� ������� ������
			previous = current;
			//��������� � ��������� ������
			current = current->next;
		}
		n++;
	}

	//������� ������� ������
	if (current != NULL) {
		previous->next = current->next;
		usersWrite(*head); //��������� ���������
	}
	printf("\n������ � ������������ ������� �������!\n");
	return;
}

void do_not_delete_main_admin(struct User** head, int key)
{
	int n = 1;
	if (key == 1) {
		deleteFirstUser(head);
		printf("\n������������ ������!\n");
		PressAnyKey();
		return;
	}

	//�������� � ������� ������
	struct User* current = *head;
	struct User* previous = NULL;

	//������ ����
	if (head == NULL) {
		return;
	}

	//�������� �� n-�� ������������
	while (n != key && current != NULL) {
		
			//��������� ������� ������
			previous = current;
			//��������� � ��������� ������
			current = current->next;
		n++;
	}
	if (current->next == NULL && n == key) {
		printf("\n���������� ������� �������� ��������������!\n");
		PressAnyKey();
		return;
	}
	//������� ������� ������
	if (current != NULL) {
		previous->next = current->next;
		usersWrite(*head); //��������� ���������
	}
	printf("\n������������ ������!\n");
	PressAnyKey();
	return;
}

// ������� ������� ������������ � ������
void deleteFirstUser(struct User** head)
{
	//�������� ���������� ������������ �� ������ ��� ������� ������ 
	//head = head->next;
	//usersWrite(head); //���������� ��������� � ����
	struct User* tmp = (*head)->next;
	free(*head); //�������, ����� �� ���� ������ ������
	*head = tmp;
	usersWrite(*head);
}

//�������� � ������� ������ �������������, ���������� ���������� �������������
int printUsersList(struct User* head)
{
	if (head == NULL) {
		printf("������������ �� ���� �������.");
		return 0;
	}
	struct User* ptr = head;
	printf("\n");
	printf("\n\n");
	printf("+-----+--------------------+--------------------+-----------------+\n");
	printf("|�����|       �����        |        ����        | ���� �����������|\n");
	printf("+-----+--------------------+--------------------+-----------------+\n");
	int n = 1; //����� ������������ � ������
	while (ptr != NULL) {
		char pos[14];
		if (ptr->is_admin)
			strcpy(pos, "�������������\0");
		else
			strcpy(pos, "������������\0");
		printf("|%5d|%20s|%20s|%17s|\n", n, ptr->login, pos, timeToChar(ptr->date));
		ptr = ptr->next; //��������� �� ���������� ������������
		n++;
	}
	printf("+-----+--------------------+--------------------+-----------------+\n");
	return n++;
}

bool authorize(struct User* head, char login[], char password[], bool isAdmin)
{
	//������������� ����� ���� ����������� � ��� �������������, � ��� ������� ������������.
	//������� ������������ ����� ���� ����������� ������ ��� ������������.
	struct User* ptr = head; //��������� �� ������������� � ������
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
			if (passwords_equals) { //������ ������
				if (isAdmin == false || ptr->is_admin == true) {
					return true;
				}
				else
				{
					printf("\n\n������������ ����\n");
					return false;
				}
			}
			else {
				printf("\n�������� ������\n");
				return false;
			}
		}
		ptr = ptr->next; //��������� �� ���������� ������������
	}
	printf("\n�� ������ ������������ � ������ �������\n");
	return false;
}

//��������� ������������ ������, ��� ������� �������� ������ ���� �������
bool isValidLogin(char* log)
{
	if (strlen(log) < 3) //����� ������ ������ ���� ��������
		return false;
	int i = 0;
	for (int i = 0; i < strlen(log); i++) //�������� �� ��������
	{
		if (!((log[i] >= 65 && log[i] <= 90) || (log[i] >= 97 && log[i] <= 122))) {
			printf("\n������. ��� ������� ������ � ������ ������ ���� ������� ����������� ��������.\n", log[i]);
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

