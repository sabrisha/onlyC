#define _CRT_SECURE_NO_WARNINGS
#include "School.h"
#include "Person.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UserInteraction.h"

//читает школу из файла
struct School* readSchool(char* datafile)
{
	//struct User* head = (struct User*) malloc(sizeof(struct User));
	//static struct School sch; 
	struct School* sch = (struct School*) malloc(sizeof(struct School)); //последний считанный пользователь
	
	FILE* fp = fopen(datafile, "rb"); //открываем файл
	if (fp == NULL) //файл не открыт
	{
		fprintf(stderr, "\nОшибка чтения файла. Невозможно прочесть школу\n");
		PressAnyKey();
		return nullptr;
	}

	fread(sch->sch_name, sizeof(sch->sch_name), 1, fp);
	fread(sch->address, sizeof(sch->address), 1, fp);
	fread(&sch->id, sizeof(&sch->id), 1, fp);;

	fclose(fp);

	return sch;
}

//записывает школу в файл
void writeSchool(School school, char* datafile)
{
	if (datafile == NULL)
	{
		fprintf(stderr, "\nОшибка записи в файл школы!\n");
		PressAnyKey();
		return;
	}
	FILE* outfile;

	// открываем файл для записи
	outfile = fopen(datafile, "wb");
	if (outfile == NULL)
	{
		fprintf(stderr, "\nОшибка записи в файл школы!\n");
		PressAnyKey();
		return;
	}

	// записываем структуру в файл 
	fwrite(school.sch_name, sizeof(school.sch_name), 1, outfile);
	fwrite(school.address, sizeof(school.address), 1, outfile);
	fwrite(&school.id, sizeof(school.id), 1, outfile);
	fwrite("\n", sizeof(char), 1, outfile); //переходим на следующую линию

	if (fwrite == 0)
		printf("Ошибка записи в файл пользователей!\n");

	// закрываем файл
	fclose(outfile);
}
