#include <stdint.h>

const int LOG_IN = 30;
const int PASS_IN = 30;
//const char* USER_FILE = "users.dat";

struct User
{
	char login[LOG_IN]; //логин
	uint8_t pass_hash[16]; 
	bool is_admin; //признак администратора
	int date; //дата регистрации пользователя.  Определяется как количество секунд, прошедших с полуночи (00:00:00 UTC) 1 января 1970 года
	struct User* next; //указатель на следующий элемент списка
	struct User* prev; //указатель на предыдущий элемент списка
};



void usersWrite(struct User* head); //записать пользователей в файл
struct User* usersRead(); //прочитать пользователей из файла
//void addUser(User *head, char log[], char pass[], bool isAdmin); //добавить пользователя
void addUser(struct User** head, char* log, char* pass, bool isAdmin); //добавить пользователя
void deleteUser(struct User** head, int key); //удаляет пользователя по номеру
void deleteFirstUser(struct User** head); //удаляет первого пользователя в списке 
int printUsersList(struct User* head); //печатает в консоль список пользователей 
bool authorize(struct User* head, char login[], char password[], bool isAdmin); //возвращает признак авторизации
bool isValidLogin(char* log); //проверяет правильность логина, все символы которого должны быть буквами
bool isExists(struct User* head, char* log); // проверяет, существует ли логин
void do_not_delete_main_admin(struct User** head, int key); //запрещает удалить главного админа до тех пор, пока в программе не останется ни одного пользователя


