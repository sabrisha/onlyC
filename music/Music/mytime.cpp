#define _CRT_SECURE_NO_WARNINGS
#include "mytime.h"
#include <time.h>
#include <iostream>

int getCurrentTime()
{
	return (int)time(NULL); //получает текущее время в мс
}

char* timeToChar(int time)
{
	time_t timeGMT = (time_t)time; //получаем время в time_t
	struct tm ts;
	char buf[17]; //символьное представление
	ts = *localtime(&timeGMT);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &ts); //формат представления
	return _strdup(buf);
}
