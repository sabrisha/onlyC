#define _CRT_SECURE_NO_WARNINGS
#include "Menus.h"
#include <iostream>
#include "mytime.h"
#include "User.h"
#include <time.h>
#include "Person.h"
#include "Group.h"
#include "School.h"
#include "UserInteraction.h"
#include <Windows.h>


struct User* headUser; //������������
char* datafile; //������� ���� � �������
struct Person* headPersons; //������ �������� � ��������
struct School* currentSchool; //������� �����

bool file_exists(char* filename) {
	struct stat   buffer;
	return (stat(filename, &buffer) == 0);
}


struct Group* EntryGroup() {
	struct Group* group = (struct Group*) malloc(sizeof(struct Group)); //�������� ������
	EntryIntValue("������� ����� ���������� �������", group->classe, 1, 100);
	while (true) {
		EntryIntValue("������� ����� ������ �������", group->time[0], 8, 18);
		EntryIntValue("������� ����� ����� �������", group->time[1], 9, 19);
		if (group->time[0] >= group->time[1])
		{
			printf("\n������� ������ ������� ��� ��� �����!\n");
		}
		else {
			break;
		}
	}
	printGroupTypes();
	EntryIntValue("������� �������������", group->type, 0, 3);
	group->school = currentSchool;
	return group;
}

//���������� ������ � ����
void writeFile(bool entry) { //���������� � ���� ����� � �����
	if (entry) { //���� ���������� - ����������� ���� ����� ����� � ������������
		EntryCharValue("������� ��� �����", datafile, 1, 300, false);
	}
	if (datafile == NULL || currentSchool == NULL)
	{
		printf("\n���� �� ����� ��� ����� �����������!\n");
		return;
	}
	writeSchool(*currentSchool, datafile);
	writePerson(headPersons, datafile);
}

//����������� ������
void freeAll(bool only_persons) {
	struct Person* tmp;

	while (headPersons != NULL)
	{
		tmp = headPersons;
		headPersons = headPersons->next;
		free(tmp->group);
		free(tmp);
	}
	headPersons = NULL;

	if (!only_persons) {
		free(currentSchool);
		currentSchool = NULL;
	}
}

void createSchool()
{
	EntryCharValue("������� �������� �����", datafile, 1, 300, false);
	freeAll();

	char* name;
	EntryCharValue("������� �������� �����", name, 5, 100);
	char* address;
	EntryCharValue("������� ����� �����", address, 5, 100, false);
	int n;
	EntryIntValue("������� ����� �����", n, 1, 300);

	currentSchool = (struct School*) malloc(sizeof(struct School));

	memcpy(currentSchool->sch_name, name, sizeof(currentSchool->sch_name));
	memcpy(currentSchool->address, address, sizeof(currentSchool->sch_name));
	currentSchool->id = n;

	writeFile();
	printf("\n���� �� ������ ������� ������!\n");
}


//������ ������ �� �����
void readFile(bool entry) {
	if (entry) { //���� ���������� - ����������� ���� ����� ����� � ������������
		char* checker;
		EntryCharValue("������� ��� �����", checker, 1, 300, false);
		if (datafile == NULL) {
			datafile = (char*)malloc(strlen(checker) + 1);
			strcpy(datafile, checker);
			free(checker);
		}
		else if (strcmp(checker, datafile) != 0) {
			free(datafile);
			datafile = (char*)malloc(strlen(checker) + 1);
			strcpy(datafile, checker);
			free(checker);
		}
		else {
			printf("\n������ ���� ��� ������!\n");
			return;
		}
	}

	if (!file_exists(datafile))
	{
		printf("\n���� �� ������!\n");
		freeAll(true);
		datafile = NULL;
		currentSchool = NULL;
		return;
	}
	if (datafile == NULL)
	{
		printf("\n���� �� �����!\n");
		return;
	}

	currentSchool = readSchool(datafile);
	headPersons = readPerson(datafile, currentSchool);
	printf("\n������ ����� ������� ���������!\n");

}


//�������� � ������� ���� ������� ������
void _menu1Print() {
	system("cls"); //������� �������
	printf("1. ���� ��� ���������������\n");
	printf("2. ���� ��� �������������\n");
	printf("3. �����\n\n");
}

//����������� � ������������ ���������� �������
void addPersonUI() {
	char* sn, * n, * p;
	int age, isT;
	EntryCharValue("������� �������", sn, 2, NAME_LEN);
	EntryCharValue("������� ���", n, 2, NAME_LEN);
	EntryCharValue("������� ��������", p, 2, NAME_LEN);
	EntryIntValue("������� �������", age, 3, 75);
	EntryIntValue("0 � ������. 1 � �������.", isT, 0, 1);
	struct Group* g = EntryGroup();
	addPerson(&headPersons, sn, n, p, age, (bool)isT, g, datafile); 
	writeFile();
	printf("\n������ ������� ���������!\n");
	
}

void changeSchool() {
	int id;
	EntryIntValue("������� id ��������, �������� ���������� ���������", id,1, get_last_id());
	struct Person* finded = findById(headPersons, id);
	if (finded == NULL) {
		printf("\n������� � ��������� id �� ������!\n");
		return;
	}
	else
	{
		char pos[10];
		if (finded->is_teacher)
			strcpy(pos, "�������\0");
		else
			strcpy(pos, "������\0");
		printf("������ %s %s %s,%s\n", finded->surname, finded->name, finded->patronymic, pos);
	}
	char* newSchool;
	EntryCharValue("������� ��� ����� ����� �����.", newSchool,1,300,false);
	if (!file_exists(newSchool)) {
		printf("���� �� ������!");
		return;
	}
	if (!appendToFile(finded, newSchool))
		printf("\n���������� �������� �������� � ����� �����!\n");
	free(newSchool);
	deletePerson(&headPersons, id);
	writeFile();
	printf("\n������� � ������ ����� ����������� �������!\n");
}

void editPersonUI() {
	if (headPersons == NULL) {
		printf("\n������ ���\n");
		return;
	}
	int id;
	EntryIntValue("������� id ��������, �������� ���������� �������������", id, 1, get_last_id());
	struct Person* finded = findById(headPersons, id);
	if (finded == NULL) {
		printf("\n������� � ��������� id �� ������!\n");
		return;
	}
	else
	{
		char pos[10];
		if (finded->is_teacher)
			strcpy(pos, "�������\0");
		else
			strcpy(pos, "������\0");
		printf("������ %s %s %s,%s\n", finded->surname, finded->name, finded->patronymic, pos);
	}
	struct Group* group = EntryGroup();
	finded->group = group;

	char* sn, * n, * p;
	int age, isT;
	EntryCharValue("������� �������", sn, 2, NAME_LEN);
	EntryCharValue("������� ���", n, 2, NAME_LEN);
	EntryCharValue("������� ��������", p, 2, NAME_LEN);
	EntryIntValue("������� �������", age, 3, 75);
	EntryIntValue("0 � ������. 1 � �������.", isT, 0, 1);

	memcpy(finded->surname, sn, sizeof(finded->surname));
	memcpy(finded->name, n, sizeof(finded->name));
	memcpy(finded->patronymic, p, sizeof(finded->patronymic));
	finded->age = age;
	finded->is_teacher = (bool)isT;
	writeFile();
	printf("\n������ ������� ���������������!\n");
	//free(sn); free(n); free(p);
}

void changeGroup() {
	int id;
	EntryIntValue("������� id ��������, �������� ���������� ���������", id,1, get_last_id());
	struct Person* finded = findById(headPersons, id);
	if (finded == NULL) {
		printf("\n������� � ��������� id �� ������!\n");
		return;
	}
	else
	{
		char pos[10];
		if (finded->is_teacher)
			strcpy(pos, "�������\0");
		else
			strcpy(pos, "������\0");
		printf("������ %s %s %s,%s\n", finded->surname, finded->name, finded->patronymic, pos);
	}
	struct Group* group = EntryGroup();
	free(finded->group);
	finded->group = group;
	writeFile();
	printf("\n������� � ������ ������ ����������� �������!\n");
}

void filterMenu() {
	if (headPersons == NULL) {
		printf("\n������ ���\n");
		return;
	}
	system("cls"); //������� �������
	printf("1. ���������� �� Id\n");
	printf("2. ���������� �� �������\n");
	printf("3. ���������� �� ��������\n");
	printf("4. ���������� �� �������������\n");
	printf("5. �������� ������ ��������\n");
	printf("6. �������� ������ ��������\n");
	printf("7. �����\n");
	int choose;
	EntryIntValue("�������� �����", choose, 1, 7);
	switch (choose) {
	case 1:
		headPersons = sortId(headPersons);
		printPersonsList(headPersons);
		break;
	case 2:
		headPersons = sortSurname(headPersons);
		printPersonsList(headPersons);
		break;
	case 3:
		headPersons = sortAge(headPersons);
		printPersonsList(headPersons);
		break;
	case 4:
		headPersons = sortTypeGroup(headPersons);
		printPersonsList(headPersons);
		break;
	case 5:
		printOnlyTeacherOrStudent(headPersons, true);
		break;
	case 6:
		printOnlyTeacherOrStudent(headPersons, false);
		break;
	}
}

void userManagmentMenu() {
	system("cls"); //������� �������
	printf("1. ������ ���� �������������\n");
	printf("2. �������� ������������\n");
	printf("3. ������� ������������ �� ������ � ������\n");
	int choose;
	EntryIntValue("�������� �����", choose, 1, 3);
	switch (choose) {
	case 1:
		printUsersList(headUser);
		PressAnyKey();
		break;
	case 2:
		char* log, * pass;
		int isAdmin;
		EntryCharValue("������� �����", log, 3, LOG_IN, false);
		EntryCharValue("������� ������", pass, 3, PASS_IN, false);
		EntryIntValue("0 � ������������, 1 � �������������", isAdmin, 0, 1);
		addUser(&headUser, log, pass, isAdmin == 1 ? true : false);
		break;
	case 3:
		int key;
		int max = printUsersList(headUser);
		EntryIntValue("������� ����� � ������", key, 1, max - 1);
		do_not_delete_main_admin(&headUser, key);
		break;
	}
}

void menuAdmin() { //���� ��������������
	while (true) {
		system("cls"); //������� �������
		if (currentSchool != NULL)
			printf("ID-%d:%s, %s\n\n", currentSchool->id, currentSchool->sch_name, currentSchool->address);
		else
			printf("����� �� �������\n\n");
		printf("1. ������� ���� � ������� �����\n");
		printf("2. ������� ���� � ������� �����\n");
		printf("3. �������� ������\n");
		printf("4. �������������� ������\n");
		printf("5. �������� ������\n");
		printf("6. �������� ���� ������ � ��������� �����\n");
		printf("7. ��������� ���������� � ���������� ������\n");
		printf("8. ���������� ��������������\n");
		printf("9. ����� � ���� ������� ������\n");
		int choose;
		EntryIntValue("�������� �����", choose, 1, 9);
		if (datafile == NULL && (choose != 1 && choose != 2 && choose != 9 && choose != 8))
		{
			printf("\n�������� �����\n");
			PressAnyKey();
			continue;
		}
		switch (choose) {
		case 1:
			createSchool();
			PressAnyKey();
			break;
		case 2:
			readFile(true);
			PressAnyKey();
			break;
		case 3:
			addPersonUI();
			PressAnyKey();
			break;
		case 4:
			editPersonUI();
			PressAnyKey();
			break;
		case 5:
			if (headPersons == NULL) {
				printf("\n������ ���\n");
				PressAnyKey();
				break;
			}
			int id;
			EntryIntValue("������� id ��������", id, 1, get_last_id());
			deletePerson(&headPersons, id);
			writeFile();
			printf("\n������ ������� �������!\n");
			PressAnyKey();
			break;
		case 6:
			printPersonsList(headPersons);
			PressAnyKey();
			break;
		case 7:
			filterMenu();
			PressAnyKey();
			break;
		case 8:
			userManagmentMenu();
			break;
		case 9:
			return;
		}
	}
}

void menuUser() {
	while (true) {
		system("cls"); //������� �������
		if (currentSchool != NULL)
			printf("�%d:%s, %s\n\n", currentSchool->id,currentSchool->sch_name,currentSchool->address);
		else
			printf("����� �� �������\n\n");
		printf("1. ������� ���� � ������� �����\n");
		printf("2. �������� ���� ������ � ��������� �����\n");
		printf("3. �������������� �������\n");
		printf("4. ��������� ���������� � ���������� ������\n");
		printf("5. ����� � ���� ������� ������\n");
		int choose;
		EntryIntValue("�������� �����", choose, 1, 5);
		if (datafile == NULL && (choose != 1 && choose != 5))
		{
			printf("�������� �����\n");
			PressAnyKey();
			continue;
		}
		switch (choose) {
		case 1:
			readFile(true);
			PressAnyKey();
			break;
		case 2:
			printPersonsList(headPersons);
			PressAnyKey();
			break;
		case 3:
			indMenu();
			break;
		case 4:
			filterMenu();
			PressAnyKey();
			break;
		case 5:
			return;
		}
	}
}

void indMenu() {
	printf("1. ������� � ������ �����\n");
	printf("2. ������� � ������ ������\n");
	int choose;
	EntryIntValue("�������� �����", choose, 1, 2);
	if (choose == 1) {
		changeSchool();
		PressAnyKey();
	}
	else {
		changeGroup();
		PressAnyKey();
	}
}

void menu1() //���� ������� ������
{
	while (true) {
		_menu1Print();
		int choose;
		EntryIntValue("�������� �����", choose, 1, 3);
		char* login;
		char* password;
		switch (choose) {
		case 1:
			EntryCharValue("������� �����", login, 3, LOG_IN, false); // ���� ������
			EntryPass("������� ������", password, 3, PASS_IN, false); // ���� ������
			if (!authorize(headUser, login, password, true)) { // ������ �� ����������� �������
				//if(false){
				PressAnyKey(); // ������� ������� ��� �����������
			}
			else {
				printf("\n\n�� ����� ��� ���������������\n");
				PressAnyKey();
				menuAdmin(); // ������� � ���� ������
			}
			break;
		case 2:
			EntryCharValue("������� �����", login, 3, LOG_IN, false); // ���� ������
			EntryPass("������� ������", password, 3, PASS_IN, false); // ���� ������
			if (!authorize(headUser, login, password, false)) { // ������ �� ����������� �������
				//if(false){
				PressAnyKey(); // ������� ������� ��� �����������
			}
			else {
				printf("\n\n�� ����� ��� �������������\n");
				PressAnyKey();
				menuUser(); // ������� � ���� ������������
			}
			break;
		case 3: 
			freeAll(); // ������������ ������
			exit(0); // ����� � 0
			break;
		}

	}
}

void ini()
{
	datafile = NULL; // ������������� ��������� �� ���� � �������
	headUser = usersRead(); // ������ � �������� �������������
}

