#define _CRT_SECURE_NO_WARNINGS
#include "Person.h"
#include "Group.h"
#include <stdlib.h>
#include <cstdio>
#include <string.h>

int last_id = 0;

int get_last_id() {
	return last_id;
}

void writePerson(struct Person* head, char* datafile)
{
	FILE* outfile;

	// ��������� ���� ��� ������
	outfile = fopen(datafile, "ab");
	if (outfile == NULL)
	{
		fprintf(stderr, "\n������ ������ �����\n");
		return;
	}

	struct Person* iterator = head;

	// ���������� ��������� � ���� 
	while (iterator != NULL)
	{
		fwrite(iterator->surname, sizeof(iterator->surname), 1, outfile);
		fwrite(iterator->name, sizeof(iterator->name), 1, outfile);
		fwrite(iterator->patronymic, sizeof(iterator->patronymic), 1, outfile);
		fwrite(&iterator->id, sizeof(iterator->id), 1, outfile);
		fwrite(&iterator->age, sizeof(iterator->id), 1, outfile);
		fwrite(&iterator->is_teacher, sizeof(iterator->is_teacher), 1, outfile);
		fwrite(&iterator->group->classe, sizeof(iterator->group->classe), 1, outfile);
		fwrite(&iterator->group->time, sizeof(iterator->group->time), 1, outfile);
		fwrite(&iterator->group->type, sizeof(iterator->group->type), 1, outfile);

		if (iterator->next == NULL) //����� ������
			break;
		iterator = iterator->next;
	}

	if (fwrite == 0)
		printf("������ ������ � ���� �������������!\n");

	// ��������� ����
	fclose(outfile);
}

//������ ���� ����� �� �����, ���������� ������
struct Person* readPerson(char* datafile, struct School* school)
{
	//struct User* head = (struct User*) malloc(sizeof(struct User));
	struct Person* head = NULL; //������� ������
	struct Person* prev = NULL; //���������� ��������� ������������
	struct Person* iterator; //��������� ��������� ������������
	struct Group* iteratorGroup; //��������� ��������� ������
	FILE* fp = fopen(datafile, "rb"); //��������� ����
	if (fp == NULL) //���� �� ������
	{
		fprintf(stderr, "\n������ ������ �����\n");
		return nullptr;
	}


	char buffer[255];
	fgets(buffer, 255, fp); //���������� ������ ������ (� ��� ���������� ���������� � �����)

	while (!feof(fp))
	{
		iterator = (struct Person*) malloc(sizeof(struct Person)); //�������� ������
		//��������� ������
		
		fread(iterator->surname, sizeof(iterator->surname), 1, fp);
		if (iterator->surname[0] == -51) break; //����� �����

		fread(iterator->name, sizeof(iterator->name), 1, fp);
		fread(iterator->patronymic, sizeof(iterator->patronymic), 1, fp);
		fread(&iterator->id, sizeof(iterator->id), 1, fp);
		if (iterator->id > last_id) //���� ������� id ������ ������������� ������ ������������
			last_id = iterator->id;
		fread(&iterator->age, sizeof(iterator->id), 1, fp);
		fread(&iterator->is_teacher, sizeof(iterator->is_teacher), 1, fp);

		iteratorGroup = (struct Group*) malloc(sizeof(struct Group)); //�������� ������
		iterator->group = iteratorGroup;

		fread(&iterator->group->classe, sizeof(iterator->group->classe), 1, fp);
		fread(&iterator->group->time, sizeof(iterator->group->time), 1, fp);
		fread(&iterator->group->type, sizeof(iterator->group->type), 1, fp);

		iterator->group->school = school;

		iterator->next = NULL;
		//�������� ���������� ������������ � ������ � ���������� ��������� �� ������� ������
		if (head == NULL) //��� ������ ������� head == NULL
		{
			iterator->next = head;
			head = iterator;
			prev = head;
		}
		else
		{
			prev->next = iterator;
			prev = iterator;
		}
	}
	fclose(fp);

	return head;
}

Person* findById(Person* head, int id)
{
	//�������� � ������� ������
	struct Person* current = head; //�������

	//�������� ������ ���� �� ������ �������� ��������
	while (current != NULL) {
		//���� ������
		if (current->id == id) {
			return current;
		}
		current = current->next;
	}

	return nullptr; //�� ������
}

//���������� ������ �������� � ����
//����� ��������� �������� � ������� ������
void addPerson(Person** head, char* surname, char* name, char* patronymic, int age, bool is_teacher, Group* group, char* datafile)
{
	//���������� ������ �������� � ����
	//����� ��������� �������� � ������� ������

	//�������� ������ ��� ������ ��������
	struct Person* link = (struct Person*) malloc(sizeof(struct Person));

	//�������� ����������
	memcpy(link->surname, surname, strlen(surname));
	//��������� ������ ������
	if (strlen(surname) < NAME_LEN)
		link->surname[strlen(surname)] = '\0';

	//�������� ����������
	memcpy(link->name, name, strlen(name));
	//��������� ������ ������
	if (strlen(name) < NAME_LEN)
		link->name[strlen(name)] = '\0';

	//�������� ����������
	memcpy(link->patronymic, patronymic, strlen(patronymic));
	//��������� ������ ������
	if (strlen(patronymic) < NAME_LEN)
		link->patronymic[strlen(patronymic)] = '\0';

	link->age = age;
	link->is_teacher = is_teacher;
	link->group = group;
	link->id = ++last_id;
	link->next = NULL;

	writePerson(link, datafile); //������������ � ���� ������ ��������

	if (*head == NULL) //���� ��� ������ ������� � �����
	{
		*head = link;
	}
	else {
		//�������� ��������� �� ������� ������� ������
		link->next = *head;
		//������ ��������� �� ������� ������ �� ���������� ������������
		*head = link;
	}
}

void printPersonsList(struct Person* head)
{
	if (head == NULL) {
		printf("\n������ ���\n");
		return;
	}

	struct Person* iter = head;
	printf("\n\n");
	printf("=========================================================================================================================\n");
	printf("| Id|      �������       |         ���        |      ��������      |�������|  ������� |�����|�������������|����� �������|\n");
	printf("=========================================================================================================================\n");
	
	while (iter) {
		char timeStr[12]; //����� �������
		snprintf(timeStr, sizeof(timeStr), "%d:00-%d:00", iter->group->time[0], iter->group->time[1]);
		char pos[10];
		if (iter->is_teacher) 
			strcpy(pos, "�������\0");
		else
			strcpy(pos, "������\0");
		printf("|%3d|%20s|%20s|%20s|%7d|%10s|%5d|%13s| %11s |\n",
			iter->id, iter->surname, iter->name, iter->patronymic, iter->age, pos,
			iter->group->classe, groupType(iter->group->type), timeStr);
		iter=iter->next;
	}
	printf("=========================================================================================================================\n");
}

void deletePerson(struct Person** head, int id) //������� �������� �� id
{
	//�������� � ������� ������
	struct Person* current = *head; //�������
	struct Person* previous = NULL; //����������

	if (current->id == id) { //������� ������� ��������� �� ������� ������
		deleteFirstPerson(head);
		return;
	}

	//������ ����
	if (head == NULL) {
		return;
	}

	//�������� ������ ���� �� ������ �������� ��������
	while (current != NULL && current->id != id) {
		//���� ��� ��������� �������
		if (current->next == NULL && current->id == id) {
			free(current); //����������� ������
			previous->next = NULL;
			return;
		}
		else {
			//��������� ������� ������
			previous = current;
			//��������� � ��������� ������
			current = current->next;
		}
	}

	//������� ������� ������
	if (current != NULL && current->id == id) {
		previous->next = current->next;
		free(current); //����������� ������
	}
	return;
}

void deleteFirstPerson(struct Person** head) //������� �������� �� ������� ������
{
	//�������� ���������� ������������ �� ������ ��� ������� ������ 
	//head = head->next;
	//usersWrite(head); //���������� ��������� � ����
	struct Person* tmp = (*head)->next;
	free(*head); //�������, ����� �� ���� ������ ������
	*head = tmp;
}

bool appendToFile(Person* person, char* datafile)
{
	FILE* outfile;

	// ��������� ���� ��� ������
	outfile = fopen(datafile, "ab");
	if (outfile == NULL)
	{
		fprintf(stderr, "\n������ ������ �����\n");
		return false;
	}

	fwrite(person->surname, sizeof(person->surname), 1, outfile);
	fwrite(person->name, sizeof(person->name), 1, outfile);
	fwrite(person->patronymic, sizeof(person->patronymic), 1, outfile);
	fwrite(&person->id, sizeof(person->id), 1, outfile);
	fwrite(&person->age, sizeof(person->id), 1, outfile);
	fwrite(&person->is_teacher, sizeof(person->is_teacher), 1, outfile);
	fwrite(&person->group->classe, sizeof(person->group->classe), 1, outfile);
	fwrite(&person->group->time, sizeof(person->group->time), 1, outfile);
	fwrite(&person->group->type, sizeof(person->group->type), 1, outfile);

	if (fwrite == 0) {
		printf("\n������ ������ � ���� �������������!\n");
		fclose(outfile);
		return false;
	}

	// ��������� ����
	fclose(outfile);
	return true;
}

//������� ������� ������ ������� ���� 'currX' � 'currY' �� ����� ������� �� ����������
void swapNodes(Person** head_ref, Person* currX, Person* currY, Person* prevY)
{
	// ������ 'currY' ����� �������� ������ 
	*head_ref = currY;

	// ������ ������ 
	prevY->next = currX;

	// ������ ������� ���������
	struct Person* temp = currY->next;
	currY->next = currX->next;
	currX->next = temp;
}

//���������� ������� ������ �� id
Person* sortId(Person* head)
{
	// ���� 0 ��� 1 ������� � ������ 
	if (head == NULL || head->next == NULL)
		return head;

	// 'min' - ��������� �� ����, � ������� ����������� id 
	struct Person* min = head;

	// 'beforeMin' - ��������� �� ���������� �� 'min' ���� 
	struct Person* beforeMin = NULL;
	struct Person* ptr;

	// ������� ������ �� ���������� ����
	for (ptr = head; ptr->next != NULL; ptr = ptr->next) {

		// ���� ������ �������, ��������� 'min' � 'beforeMin' 
		if (ptr->next->id < min->id) {
			min = ptr->next;
			beforeMin = ptr;
		}
	}

	// ���� 'min' � 'head' �� �����, 
	// ������ ������� ������� ������ � ���� 'min' 
	if (min != head)
		swapNodes(&head, head, min, beforeMin);

	// ��������� ��������� ���������� ����� ������ 
	head->next = sortId(head->next);

	return head;
}

Person* sortSurname(Person* head)
{
	// ���� 0 ��� 1 ������� � ������ 
	if (head == NULL || head->next == NULL)
		return head;

	// 'min' - ��������� �� ����, � ������� ����������� id 
	struct Person* min = head;

	// 'beforeMin' - ��������� �� ���������� �� 'min' ���� 
	struct Person* beforeMin = NULL;
	struct Person* ptr;

	// ������� ������ �� ���������� ����
	for (ptr = head; ptr->next != NULL; ptr = ptr->next) {

		// ���� ������ �������, ��������� 'min' � 'beforeMin' 
		if (strcmp(ptr->next->surname, min->surname) < 0) {
			min = ptr->next;
			beforeMin = ptr;
		}
	}

	// ���� 'min' � 'head' �� �����, 
	// ������ ������� ������� ������ � ���� 'min' 
	if (min != head)
		swapNodes(&head, head, min, beforeMin);

	// ��������� ��������� ���������� ����� ������ 
	head->next = sortSurname(head->next);

	return head;
}

Person* sortAge(Person* head)
{
	// ���� 0 ��� 1 ������� � ������ 
	if (head == NULL || head->next == NULL)
		return head;

	// 'min' - ��������� �� ����, � ������� ����������� id 
	struct Person* min = head;

	// 'beforeMin' - ��������� �� ���������� �� 'min' ���� 
	struct Person* beforeMin = NULL;
	struct Person* ptr;

	// ������� ������ �� ���������� ����
	for (ptr = head; ptr->next != NULL; ptr = ptr->next) {

		// ���� ������ �������, ��������� 'min' � 'beforeMin' 
		if (ptr->next->age < min->age) {
			min = ptr->next;
			beforeMin = ptr;
		}
	}

	// ���� 'min' � 'head' �� �����, 
	// ������ ������� ������� ������ � ���� 'min' 
	if (min != head)
		swapNodes(&head, head, min, beforeMin);

	// ��������� ��������� ���������� ����� ������ 
	head->next = sortAge(head->next);

	return head;
}

Person* sortTypeGroup(Person* head)
{
	// ���� 0 ��� 1 ������� � ������ 
	if (head == NULL || head->next == NULL)
		return head;

	// 'min' - ��������� �� ����, � ������� ����������� id 
	struct Person* min = head;

	// 'beforeMin' - ��������� �� ���������� �� 'min' ���� 
	struct Person* beforeMin = NULL;
	struct Person* ptr;

	// ������� ������ �� ���������� ����
	for (ptr = head; ptr->next != NULL; ptr = ptr->next) {

		// ���� ������ �������, ��������� 'min' � 'beforeMin' 
		if (ptr->next->group->type < min->group->type) {
			min = ptr->next;
			beforeMin = ptr;
		}
	}

	// ���� 'min' � 'head' �� �����, 
	// ������ ������� ������� ������ � ���� 'min' 
	if (min != head)
		swapNodes(&head, head, min, beforeMin);

	// ��������� ��������� ���������� ����� ������ 
	head->next = sortTypeGroup(head->next);

	return head;
}

void printOnlyTeacherOrStudent(Person* head, bool is_teacher)
{
	struct Person* iter = head;
	printf("=========================================================================================================================\n");
	printf("| Id|      �������       |         ���        |      ��������      |�������|  ������� |�����|�������������|����� �������|\n");
	printf("=========================================================================================================================\n");

	while (iter) {
		if (iter->is_teacher != is_teacher) {
			iter = iter->next;
			continue;
		}
		char timeStr[12]; //����� �������
		snprintf(timeStr, sizeof(timeStr), "%d:00-%d:00", iter->group->time[0], iter->group->time[1]);
		char pos[10];
		if (iter->is_teacher)
			strcpy(pos, "�������\0");
		else
			strcpy(pos, "������\0");
		printf("|%3d|%20s|%20s|%20s|%7d|%10s|%5d|%13s| %11s |\n",
			iter->id, iter->surname, iter->name, iter->patronymic, iter->age, pos,
			iter->group->classe, groupType(iter->group->type), timeStr);
		iter = iter->next;
	}
	printf("=========================================================================================================================\n");
}
