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


struct User* headUser; //пользователи
char* datafile; //текущий файл с данными
struct Person* headPersons; //список учеников и учителей
struct School* currentSchool; //текущая школа

bool file_exists(char* filename) {
	struct stat   buffer;
	return (stat(filename, &buffer) == 0);
}


struct Group* EntryGroup() {
	struct Group* group = (struct Group*) malloc(sizeof(struct Group)); //выделяем память
	EntryIntValue("Введите класс проведения занятий", group->classe, 1, 100);
	while (true) {
		EntryIntValue("Введите время начала занятий", group->time[0], 8, 18);
		EntryIntValue("Введите время конца занятий", group->time[1], 9, 19);
		if (group->time[0] >= group->time[1])
		{
			printf("\nЗанятия должны длиться час или более!\n");
		}
		else {
			break;
		}
	}
	printGroupTypes();
	EntryIntValue("Введите специальность", group->type, 0, 3);
	group->school = currentSchool;
	return group;
}

//записывает данные в файл
void writeFile(bool entry) { //записывает в файл школу и людей
	if (entry) { //если необходимо - запрашиваем ввод имени файла у пользователя
		EntryCharValue("Введите имя файла", datafile, 1, 300, false);
	}
	if (datafile == NULL || currentSchool == NULL)
	{
		printf("\nФайл не задан или школа отсутствует!\n");
		return;
	}
	writeSchool(*currentSchool, datafile);
	writePerson(headPersons, datafile);
}

//освобождает память
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
	EntryCharValue("Введите название файла", datafile, 1, 300, false);
	freeAll();

	char* name;
	EntryCharValue("Введите название школы", name, 5, 100);
	char* address;
	EntryCharValue("Введите адрес школы", address, 5, 100, false);
	int n;
	EntryIntValue("Введите номер школы", n, 1, 300);

	currentSchool = (struct School*) malloc(sizeof(struct School));

	memcpy(currentSchool->sch_name, name, sizeof(currentSchool->sch_name));
	memcpy(currentSchool->address, address, sizeof(currentSchool->sch_name));
	currentSchool->id = n;

	writeFile();
	printf("\nФайл со школой успешно создан!\n");
}


//читает данные из файла
void readFile(bool entry) {
	if (entry) { //если необходимо - запрашиваем ввод имени файла у пользователя
		char* checker;
		EntryCharValue("Введите имя файла", checker, 1, 300, false);
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
			printf("\nДанный файл уже открыт!\n");
			return;
		}
	}

	if (!file_exists(datafile))
	{
		printf("\nФайл не найден!\n");
		freeAll(true);
		datafile = NULL;
		currentSchool = NULL;
		return;
	}
	if (datafile == NULL)
	{
		printf("\nФайл не задан!\n");
		return;
	}

	currentSchool = readSchool(datafile);
	headPersons = readPerson(datafile, currentSchool);
	printf("\nДанные школы успешно загружены!\n");

}


//печатает в консоль меню первого уровня
void _menu1Print() {
	system("cls"); //очищаем консоль
	printf("1. Вход под администратором\n");
	printf("2. Вход под пользователем\n");
	printf("3. Выход\n\n");
}

//запрашивает у пользователя добавления человка
void addPersonUI() {
	char* sn, * n, * p;
	int age, isT;
	EntryCharValue("Введите фамилию", sn, 2, NAME_LEN);
	EntryCharValue("Введите имя", n, 2, NAME_LEN);
	EntryCharValue("Введите отчество", p, 2, NAME_LEN);
	EntryIntValue("Введите возраст", age, 3, 75);
	EntryIntValue("0 — Ученик. 1 — Учитель.", isT, 0, 1);
	struct Group* g = EntryGroup();
	addPerson(&headPersons, sn, n, p, age, (bool)isT, g, datafile); 
	writeFile();
	printf("\nЗапись успешно добавлена!\n");
	
}

void changeSchool() {
	int id;
	EntryIntValue("Введите id человека, которого необходимо перевести", id,1, get_last_id());
	struct Person* finded = findById(headPersons, id);
	if (finded == NULL) {
		printf("\nЧеловек с указанным id не найден!\n");
		return;
	}
	else
	{
		char pos[10];
		if (finded->is_teacher)
			strcpy(pos, "Учитель\0");
		else
			strcpy(pos, "Ученик\0");
		printf("Выбран %s %s %s,%s\n", finded->surname, finded->name, finded->patronymic, pos);
	}
	char* newSchool;
	EntryCharValue("Введите имя файла новой школы.", newSchool,1,300,false);
	if (!file_exists(newSchool)) {
		printf("Файл не найден!");
		return;
	}
	if (!appendToFile(finded, newSchool))
		printf("\nНевозможно записать человека в новую школу!\n");
	free(newSchool);
	deletePerson(&headPersons, id);
	writeFile();
	printf("\nПеревод в другую школу осуществлен успешно!\n");
}

void editPersonUI() {
	if (headPersons == NULL) {
		printf("\nДанных нет\n");
		return;
	}
	int id;
	EntryIntValue("Введите id человека, которого необходимо редактировать", id, 1, get_last_id());
	struct Person* finded = findById(headPersons, id);
	if (finded == NULL) {
		printf("\nЧеловек с указанным id не найден!\n");
		return;
	}
	else
	{
		char pos[10];
		if (finded->is_teacher)
			strcpy(pos, "Учитель\0");
		else
			strcpy(pos, "Ученик\0");
		printf("Выбран %s %s %s,%s\n", finded->surname, finded->name, finded->patronymic, pos);
	}
	struct Group* group = EntryGroup();
	finded->group = group;

	char* sn, * n, * p;
	int age, isT;
	EntryCharValue("Введите фамилию", sn, 2, NAME_LEN);
	EntryCharValue("Введите имя", n, 2, NAME_LEN);
	EntryCharValue("Введите отчество", p, 2, NAME_LEN);
	EntryIntValue("Введите возраст", age, 3, 75);
	EntryIntValue("0 — Ученик. 1 — Учитель.", isT, 0, 1);

	memcpy(finded->surname, sn, sizeof(finded->surname));
	memcpy(finded->name, n, sizeof(finded->name));
	memcpy(finded->patronymic, p, sizeof(finded->patronymic));
	finded->age = age;
	finded->is_teacher = (bool)isT;
	writeFile();
	printf("\nЗапись успешно отредактирована!\n");
	//free(sn); free(n); free(p);
}

void changeGroup() {
	int id;
	EntryIntValue("Введите id человека, которого необходимо перевести", id,1, get_last_id());
	struct Person* finded = findById(headPersons, id);
	if (finded == NULL) {
		printf("\nЧеловек с указанным id не найден!\n");
		return;
	}
	else
	{
		char pos[10];
		if (finded->is_teacher)
			strcpy(pos, "Учитель\0");
		else
			strcpy(pos, "Ученик\0");
		printf("Выбран %s %s %s,%s\n", finded->surname, finded->name, finded->patronymic, pos);
	}
	struct Group* group = EntryGroup();
	free(finded->group);
	finded->group = group;
	writeFile();
	printf("\nПеревод в другую группу осуществлен успешно!\n");
}

void filterMenu() {
	if (headPersons == NULL) {
		printf("\nДанных нет\n");
		return;
	}
	system("cls"); //очищаем консоль
	printf("1. Сортировка по Id\n");
	printf("2. Сортировка по фамилии\n");
	printf("3. Сортировка по возрасту\n");
	printf("4. Сортировка по специальности\n");
	printf("5. Печатать только учителей\n");
	printf("6. Печатать только учеников\n");
	printf("7. Назад\n");
	int choose;
	EntryIntValue("Сделайте выбор", choose, 1, 7);
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
	system("cls"); //очищаем консоль
	printf("1. Печать всех пользователей\n");
	printf("2. Добавить пользователя\n");
	printf("3. Удалить пользователя по номеру в списке\n");
	int choose;
	EntryIntValue("Сделайте выбор", choose, 1, 3);
	switch (choose) {
	case 1:
		printUsersList(headUser);
		PressAnyKey();
		break;
	case 2:
		char* log, * pass;
		int isAdmin;
		EntryCharValue("Введите логин", log, 3, LOG_IN, false);
		EntryCharValue("Введите пароль", pass, 3, PASS_IN, false);
		EntryIntValue("0 — Пользователь, 1 — Администратор", isAdmin, 0, 1);
		addUser(&headUser, log, pass, isAdmin == 1 ? true : false);
		break;
	case 3:
		int key;
		int max = printUsersList(headUser);
		EntryIntValue("Введите номер в списке", key, 1, max - 1);
		do_not_delete_main_admin(&headUser, key);
		break;
	}
}

void menuAdmin() { //меню администратора
	while (true) {
		system("cls"); //очищаем консоль
		if (currentSchool != NULL)
			printf("ID-%d:%s, %s\n\n", currentSchool->id, currentSchool->sch_name, currentSchool->address);
		else
			printf("Школа не выбрана\n\n");
		printf("1. Создать файл с данными школы\n");
		printf("2. Открыть файл с данными школы\n");
		printf("3. Добавить запись\n");
		printf("4. Редактирование записи\n");
		printf("5. Удаление записи\n");
		printf("6. Просмотр всех данных в табличной форме\n");
		printf("7. Процедуры сортировки и фильтрации данных\n");
		printf("8. Управление пользователями\n");
		printf("9. Выход в меню первого уровня\n");
		int choose;
		EntryIntValue("Сделайте выбор", choose, 1, 9);
		if (datafile == NULL && (choose != 1 && choose != 2 && choose != 9 && choose != 8))
		{
			printf("\nВыберите школу\n");
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
				printf("\nДанных нет\n");
				PressAnyKey();
				break;
			}
			int id;
			EntryIntValue("Введите id человека", id, 1, get_last_id());
			deletePerson(&headPersons, id);
			writeFile();
			printf("\nЗапись успешно удалена!\n");
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
		system("cls"); //очищаем консоль
		if (currentSchool != NULL)
			printf("№%d:%s, %s\n\n", currentSchool->id,currentSchool->sch_name,currentSchool->address);
		else
			printf("Школа не выбрана\n\n");
		printf("1. Открыть файл с данными школы\n");
		printf("2. Просмотр всех данных в табличной форме\n");
		printf("3. Индивидуальное задание\n");
		printf("4. Процедуры сортировки и фильтрации данных\n");
		printf("5. Выход в меню первого уровня\n");
		int choose;
		EntryIntValue("Сделайте выбор", choose, 1, 5);
		if (datafile == NULL && (choose != 1 && choose != 5))
		{
			printf("Выберите школу\n");
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
	printf("1. Перевод в другую школу\n");
	printf("2. Перевод в другую группу\n");
	int choose;
	EntryIntValue("Сделайте выбор", choose, 1, 2);
	if (choose == 1) {
		changeSchool();
		PressAnyKey();
	}
	else {
		changeGroup();
		PressAnyKey();
	}
}

void menu1() //меню первого уровня
{
	while (true) {
		_menu1Print();
		int choose;
		EntryIntValue("Сделайте выбор", choose, 1, 3);
		char* login;
		char* password;
		switch (choose) {
		case 1:
			EntryCharValue("Введите логин", login, 3, LOG_IN, false); // ввод логина
			EntryPass("Введите пароль", password, 3, PASS_IN, false); // ввод пароля
			if (!authorize(headUser, login, password, true)) { // прошла ли авторизация успешно
				//if(false){
				PressAnyKey(); // нажатие клавиши для продолжения
			}
			else {
				printf("\n\nВы вошли под администратором\n");
				PressAnyKey();
				menuAdmin(); // переход к меню админа
			}
			break;
		case 2:
			EntryCharValue("Введите логин", login, 3, LOG_IN, false); // ввод логина
			EntryPass("Введите пароль", password, 3, PASS_IN, false); // ввод пароля
			if (!authorize(headUser, login, password, false)) { // прошла ли авторизация успешно
				//if(false){
				PressAnyKey(); // нажатие клавиши для продолжения
			}
			else {
				printf("\n\nВы вошли под пользователем\n");
				PressAnyKey();
				menuUser(); // переход к меню пользователя
			}
			break;
		case 3: 
			freeAll(); // освобождение памяти
			exit(0); // выход с 0
			break;
		}

	}
}

void ini()
{
	datafile = NULL; // инициализация указателя на файл с данными
	headUser = usersRead(); // чтение и загрузка пользователей
}

