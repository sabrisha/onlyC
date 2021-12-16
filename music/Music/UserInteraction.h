#include <climits>


//������������ � ������������ ���������� ���� int � ������� � `value`
bool _EntryIntValue(const char* name, int& value, int min = 1, int max = INT_MAX);

//�������� `EntryIntegerValue` ���� �� ����� ��� ������ �����
void EntryIntValue(const char* name, int& value, int min = 1, int max = INT_MAX);

//������������ � ������������ ���������� ���� char[] � ������� � `value`
void EntryCharValue(const char* request, char*& value, int min = 1, int max = 300, bool needUpper = true);

//����������� ���� ����� �������
void PressAnyKey();

//��� �������� �������� � ���.
char* ToRus(wchar_t* str);

bool myIsUpper(char c); //�������� ������� �� ��������� �����

void EntryPass(const char* request, char*& value, int min, int max, bool needUpper);

