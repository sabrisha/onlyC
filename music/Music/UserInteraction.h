#include <climits>


//заправшивает у пользовател€ переменную типа int и заносит в `value`
bool _EntryIntValue(const char* name, int& value, int min = 1, int max = INT_MAX);

//¬ызывает `EntryIntegerValue` пока не будет дан верный ответ
void EntryIntValue(const char* name, int& value, int min = 1, int max = INT_MAX);

//заправшивает у пользовател€ переменную типа char[] и заносит в `value`
void EntryCharValue(const char* request, char*& value, int min = 1, int max = 300, bool needUpper = true);

//«апрашивает ввод любой клавиши
void PressAnyKey();

//дл€ перевода символов в рус.
char* ToRus(wchar_t* str);

bool myIsUpper(char c); //ѕроверка символа на заглавную букву

void EntryPass(const char* request, char*& value, int min, int max, bool needUpper);

