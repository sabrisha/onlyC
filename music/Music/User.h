#include <stdint.h>

const int LOG_IN = 30;
const int PASS_IN = 30;
//const char* USER_FILE = "users.dat";

struct User
{
	char login[LOG_IN]; //�����
	uint8_t pass_hash[16]; 
	bool is_admin; //������� ��������������
	int date; //���� ����������� ������������.  ������������ ��� ���������� ������, ��������� � �������� (00:00:00 UTC) 1 ������ 1970 ����
	struct User* next; //��������� �� ��������� ������� ������
	struct User* prev; //��������� �� ���������� ������� ������
};



void usersWrite(struct User* head); //�������� ������������� � ����
struct User* usersRead(); //��������� ������������� �� �����
//void addUser(User *head, char log[], char pass[], bool isAdmin); //�������� ������������
void addUser(struct User** head, char* log, char* pass, bool isAdmin); //�������� ������������
void deleteUser(struct User** head, int key); //������� ������������ �� ������
void deleteFirstUser(struct User** head); //������� ������� ������������ � ������ 
int printUsersList(struct User* head); //�������� � ������� ������ ������������� 
bool authorize(struct User* head, char login[], char password[], bool isAdmin); //���������� ������� �����������
bool isValidLogin(char* log); //��������� ������������ ������, ��� ������� �������� ������ ���� �������
bool isExists(struct User* head, char* log); // ���������, ���������� �� �����
void do_not_delete_main_admin(struct User** head, int key); //��������� ������� �������� ������ �� ��� ���, ���� � ��������� �� ��������� �� ������ ������������


