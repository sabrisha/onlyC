#define _CRT_SECURE_NO_WARNINGS
#include "mytime.h"
#include <time.h>
#include <iostream>

int getCurrentTime()
{
	return (int)time(NULL); //�������� ������� ����� � ��
}

char* timeToChar(int time)
{
	time_t timeGMT = (time_t)time; //�������� ����� � time_t
	struct tm ts;
	char buf[17]; //���������� �������������
	ts = *localtime(&timeGMT);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &ts); //������ �������������
	return _strdup(buf);
}
