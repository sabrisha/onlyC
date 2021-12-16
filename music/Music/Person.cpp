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

	// открываем файл для записи
	outfile = fopen(datafile, "ab");
	if (outfile == NULL)
	{
		fprintf(stderr, "\nОшибка чтения файла\n");
		return;
	}

	struct Person* iterator = head;

	// записываем структуру в файл 
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

		if (iterator->next == NULL) //конец списка
			break;
		iterator = iterator->next;
	}

	if (fwrite == 0)
		printf("Ошибка записи в файл пользователей!\n");

	// закрываем файл
	fclose(outfile);
}

//читает всех людей из файла, возвращает список
struct Person* readPerson(char* datafile, struct School* school)
{
	//struct User* head = (struct User*) malloc(sizeof(struct User));
	struct Person* head = NULL; //вершина списка
	struct Person* prev = NULL; //предыдущий считанный пользователь
	struct Person* iterator; //последний считанный пользователь
	struct Group* iteratorGroup; //последний считанная группа
	FILE* fp = fopen(datafile, "rb"); //открываем файл
	if (fp == NULL) //файл не открыт
	{
		fprintf(stderr, "\nОшибка чтения файла\n");
		return nullptr;
	}


	char buffer[255];
	fgets(buffer, 255, fp); //пропускаем первую строку (в ней содержится информация о школе)

	while (!feof(fp))
	{
		iterator = (struct Person*) malloc(sizeof(struct Person)); //выделяем память
		//считываем данные
		
		fread(iterator->surname, sizeof(iterator->surname), 1, fp);
		if (iterator->surname[0] == -51) break; //конец файла

		fread(iterator->name, sizeof(iterator->name), 1, fp);
		fread(iterator->patronymic, sizeof(iterator->patronymic), 1, fp);
		fread(&iterator->id, sizeof(iterator->id), 1, fp);
		if (iterator->id > last_id) //если текущий id меньше максимального меняем максимальный
			last_id = iterator->id;
		fread(&iterator->age, sizeof(iterator->id), 1, fp);
		fread(&iterator->is_teacher, sizeof(iterator->is_teacher), 1, fp);

		iteratorGroup = (struct Group*) malloc(sizeof(struct Group)); //выделяем память
		iterator->group = iteratorGroup;

		fread(&iterator->group->classe, sizeof(iterator->group->classe), 1, fp);
		fread(&iterator->group->time, sizeof(iterator->group->time), 1, fp);
		fread(&iterator->group->type, sizeof(iterator->group->type), 1, fp);

		iterator->group->school = school;

		iterator->next = NULL;
		//помещаем считанного пользователя в список и перемещаем указатель на вершину списка
		if (head == NULL) //При первом запуске head == NULL
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
	//Начинаем с вершины списка
	struct Person* current = head; //текущий

	//проходим список пока не найдем искомого человека
	while (current != NULL) {
		//если найден
		if (current->id == id) {
			return current;
		}
		current = current->next;
	}

	return nullptr; //не найден
}

//записывает данные человека в файл
//затем добавляет человека в вершину списка
void addPerson(Person** head, char* surname, char* name, char* patronymic, int age, bool is_teacher, Group* group, char* datafile)
{
	//записывает данные человека в файл
	//затем добавляет человека в вершину списка

	//выделяем память для нового человека
	struct Person* link = (struct Person*) malloc(sizeof(struct Person));

	//копируем информацию
	memcpy(link->surname, surname, strlen(surname));
	//добавляем пустой символ
	if (strlen(surname) < NAME_LEN)
		link->surname[strlen(surname)] = '\0';

	//копируем информацию
	memcpy(link->name, name, strlen(name));
	//добавляем пустой символ
	if (strlen(name) < NAME_LEN)
		link->name[strlen(name)] = '\0';

	//копируем информацию
	memcpy(link->patronymic, patronymic, strlen(patronymic));
	//добавляем пустой символ
	if (strlen(patronymic) < NAME_LEN)
		link->patronymic[strlen(patronymic)] = '\0';

	link->age = age;
	link->is_teacher = is_teacher;
	link->group = group;
	link->id = ++last_id;
	link->next = NULL;

	writePerson(link, datafile); //дозаписываем в файл нового человека

	if (*head == NULL) //если это первый человек в школе
	{
		*head = link;
	}
	else {
		//помещаем указатель на вершину старого списка
		link->next = *head;
		//меняем указатель на вершину списка на созданного пользователя
		*head = link;
	}
}

void printPersonsList(struct Person* head)
{
	if (head == NULL) {
		printf("\nДанных нет\n");
		return;
	}

	struct Person* iter = head;
	printf("\n\n");
	printf("=========================================================================================================================\n");
	printf("| Id|      Фамилия       |         Имя        |      Отчество      |Возраст|  Позиция |Класс|Специальность|Время занятий|\n");
	printf("=========================================================================================================================\n");
	
	while (iter) {
		char timeStr[12]; //время занятий
		snprintf(timeStr, sizeof(timeStr), "%d:00-%d:00", iter->group->time[0], iter->group->time[1]);
		char pos[10];
		if (iter->is_teacher) 
			strcpy(pos, "Учитель\0");
		else
			strcpy(pos, "Ученик\0");
		printf("|%3d|%20s|%20s|%20s|%7d|%10s|%5d|%13s| %11s |\n",
			iter->id, iter->surname, iter->name, iter->patronymic, iter->age, pos,
			iter->group->classe, groupType(iter->group->type), timeStr);
		iter=iter->next;
	}
	printf("=========================================================================================================================\n");
}

void deletePerson(struct Person** head, int id) //удаляет человека по id
{
	//Начинаем с вершины списка
	struct Person* current = *head; //текущий
	struct Person* previous = NULL; //предыдущий

	if (current->id == id) { //искомый человек находится на вершине списка
		deleteFirstPerson(head);
		return;
	}

	//список пуст
	if (head == NULL) {
		return;
	}

	//проходим список пока не найдем искомого человека
	while (current != NULL && current->id != id) {
		//если это последний элемент
		if (current->next == NULL && current->id == id) {
			free(current); //освобождаем память
			previous->next = NULL;
			return;
		}
		else {
			//сохраняем текущую ссылку
			previous = current;
			//переходим к следующей ссылке
			current = current->next;
		}
	}

	//обходим текущую ссылку
	if (current != NULL && current->id == id) {
		previous->next = current->next;
		free(current); //освобождаем память
	}
	return;
}

void deleteFirstPerson(struct Person** head) //удаляет человека на вершине списка
{
	//Помечаем следующего пользователя за первым как вершину списка 
	//head = head->next;
	//usersWrite(head); //записываем изменения в файл
	struct Person* tmp = (*head)->next;
	free(*head); //очищаем, чтобы не было утечки памяти
	*head = tmp;
}

bool appendToFile(Person* person, char* datafile)
{
	FILE* outfile;

	// открываем файл для записи
	outfile = fopen(datafile, "ab");
	if (outfile == NULL)
	{
		fprintf(stderr, "\nОшибка чтения файла\n");
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
		printf("\nОшибка записи в файл пользователей!\n");
		fclose(outfile);
		return false;
	}

	// закрываем файл
	fclose(outfile);
	return true;
}

//функция которая меняет местами узлы 'currX' и 'currY' не меняя местами их содержимое
void swapNodes(Person** head_ref, Person* currX, Person* currY, Person* prevY)
{
	// делаем 'currY' новой вершиной списка 
	*head_ref = currY;

	// меняем ссылки 
	prevY->next = currX;

	// меняем местами указатели
	struct Person* temp = currY->next;
	currY->next = currX->next;
	currX->next = temp;
}

//сортировка методом выбора по id
Person* sortId(Person* head)
{
	// если 0 или 1 человек в списке 
	if (head == NULL || head->next == NULL)
		return head;

	// 'min' - указатель на узел, в котором минимальный id 
	struct Person* min = head;

	// 'beforeMin' - указатель на предыдущий до 'min' узел 
	struct Person* beforeMin = NULL;
	struct Person* ptr;

	// обходим список до последнего узла
	for (ptr = head; ptr->next != NULL; ptr = ptr->next) {

		// если найден минимум, обновляем 'min' и 'beforeMin' 
		if (ptr->next->id < min->id) {
			min = ptr->next;
			beforeMin = ptr;
		}
	}

	// если 'min' и 'head' не равны, 
	// меняем местами вершину списка и узел 'min' 
	if (min != head)
		swapNodes(&head, head, min, beforeMin);

	// рекрсивно сортируем оставшуюся часть списка 
	head->next = sortId(head->next);

	return head;
}

Person* sortSurname(Person* head)
{
	// если 0 или 1 человек в списке 
	if (head == NULL || head->next == NULL)
		return head;

	// 'min' - указатель на узел, в котором минимальный id 
	struct Person* min = head;

	// 'beforeMin' - указатель на предыдущий до 'min' узел 
	struct Person* beforeMin = NULL;
	struct Person* ptr;

	// обходим список до последнего узла
	for (ptr = head; ptr->next != NULL; ptr = ptr->next) {

		// если найден минимум, обновляем 'min' и 'beforeMin' 
		if (strcmp(ptr->next->surname, min->surname) < 0) {
			min = ptr->next;
			beforeMin = ptr;
		}
	}

	// если 'min' и 'head' не равны, 
	// меняем местами вершину списка и узел 'min' 
	if (min != head)
		swapNodes(&head, head, min, beforeMin);

	// рекрсивно сортируем оставшуюся часть списка 
	head->next = sortSurname(head->next);

	return head;
}

Person* sortAge(Person* head)
{
	// если 0 или 1 человек в списке 
	if (head == NULL || head->next == NULL)
		return head;

	// 'min' - указатель на узел, в котором минимальный id 
	struct Person* min = head;

	// 'beforeMin' - указатель на предыдущий до 'min' узел 
	struct Person* beforeMin = NULL;
	struct Person* ptr;

	// обходим список до последнего узла
	for (ptr = head; ptr->next != NULL; ptr = ptr->next) {

		// если найден минимум, обновляем 'min' и 'beforeMin' 
		if (ptr->next->age < min->age) {
			min = ptr->next;
			beforeMin = ptr;
		}
	}

	// если 'min' и 'head' не равны, 
	// меняем местами вершину списка и узел 'min' 
	if (min != head)
		swapNodes(&head, head, min, beforeMin);

	// рекрсивно сортируем оставшуюся часть списка 
	head->next = sortAge(head->next);

	return head;
}

Person* sortTypeGroup(Person* head)
{
	// если 0 или 1 человек в списке 
	if (head == NULL || head->next == NULL)
		return head;

	// 'min' - указатель на узел, в котором минимальный id 
	struct Person* min = head;

	// 'beforeMin' - указатель на предыдущий до 'min' узел 
	struct Person* beforeMin = NULL;
	struct Person* ptr;

	// обходим список до последнего узла
	for (ptr = head; ptr->next != NULL; ptr = ptr->next) {

		// если найден минимум, обновляем 'min' и 'beforeMin' 
		if (ptr->next->group->type < min->group->type) {
			min = ptr->next;
			beforeMin = ptr;
		}
	}

	// если 'min' и 'head' не равны, 
	// меняем местами вершину списка и узел 'min' 
	if (min != head)
		swapNodes(&head, head, min, beforeMin);

	// рекрсивно сортируем оставшуюся часть списка 
	head->next = sortTypeGroup(head->next);

	return head;
}

void printOnlyTeacherOrStudent(Person* head, bool is_teacher)
{
	struct Person* iter = head;
	printf("=========================================================================================================================\n");
	printf("| Id|      Фамилия       |         Имя        |      Отчество      |Возраст|  Позиция |Класс|Специальность|Время занятий|\n");
	printf("=========================================================================================================================\n");

	while (iter) {
		if (iter->is_teacher != is_teacher) {
			iter = iter->next;
			continue;
		}
		char timeStr[12]; //время занятий
		snprintf(timeStr, sizeof(timeStr), "%d:00-%d:00", iter->group->time[0], iter->group->time[1]);
		char pos[10];
		if (iter->is_teacher)
			strcpy(pos, "Учитель\0");
		else
			strcpy(pos, "Ученик\0");
		printf("|%3d|%20s|%20s|%20s|%7d|%10s|%5d|%13s| %11s |\n",
			iter->id, iter->surname, iter->name, iter->patronymic, iter->age, pos,
			iter->group->classe, groupType(iter->group->type), timeStr);
		iter = iter->next;
	}
	printf("=========================================================================================================================\n");
}
