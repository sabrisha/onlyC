#define _CRT_SECURE_NO_WARNINGS
#include "School.h"
#include "Person.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UserInteraction.h"

//������ ����� �� �����
struct School* readSchool(char* datafile)
{
	//struct User* head = (struct User*) malloc(sizeof(struct User));
	//static struct School sch; 
	struct School* sch = (struct School*) malloc(sizeof(struct School)); //��������� ��������� ������������
	
	FILE* fp = fopen(datafile, "rb"); //��������� ����
	if (fp == NULL) //���� �� ������
	{
		fprintf(stderr, "\n������ ������ �����. ���������� �������� �����\n");
		PressAnyKey();
		return nullptr;
	}

	fread(sch->sch_name, sizeof(sch->sch_name), 1, fp);
	fread(sch->address, sizeof(sch->address), 1, fp);
	fread(&sch->id, sizeof(&sch->id), 1, fp);;

	fclose(fp);

	return sch;
}

//���������� ����� � ����
void writeSchool(School school, char* datafile)
{
	if (datafile == NULL)
	{
		fprintf(stderr, "\n������ ������ � ���� �����!\n");
		PressAnyKey();
		return;
	}
	FILE* outfile;

	// ��������� ���� ��� ������
	outfile = fopen(datafile, "wb");
	if (outfile == NULL)
	{
		fprintf(stderr, "\n������ ������ � ���� �����!\n");
		PressAnyKey();
		return;
	}

	// ���������� ��������� � ���� 
	fwrite(school.sch_name, sizeof(school.sch_name), 1, outfile);
	fwrite(school.address, sizeof(school.address), 1, outfile);
	fwrite(&school.id, sizeof(school.id), 1, outfile);
	fwrite("\n", sizeof(char), 1, outfile); //��������� �� ��������� �����

	if (fwrite == 0)
		printf("������ ������ � ���� �������������!\n");

	// ��������� ����
	fclose(outfile);
}
