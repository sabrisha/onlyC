const int NAME_LEN = 20;

struct Person
{
	char surname[NAME_LEN];
	char name[NAME_LEN];
	char patronymic[NAME_LEN];
	int id;
	int age;
	bool is_teacher;
	struct Group* group;
	struct Person* next;
	struct Person* prev;
};

struct Group
{
	int classe; //класс проведения занятий
	int time[2]; //используется для хранения времени начала занятий у группы. time[0] - время начала занятий, time[1]- время конца занятий
	int type; //тип группы
	struct School* school; //музыкальная школа 
};

struct School
{
	char sch_name[100];
	char address[100];
	int id;
};


void writePerson(struct Person* head, char* datafile); //записать людей в файл
struct Person* readPerson(char* datafile, struct School* school); //прочитать людей из файла
struct Person* findById(struct Person* head, int id); //найти человека по id
void addPerson(struct Person** head, char* surname, char* name, char* patronymic, int age, bool is_teacher, struct Group* group, char* datafile); //добавить пользователя
void printPersonsList(struct Person* head); //печатает в консоль людей
struct Person* sortId(struct Person* head); //сортировка методом выбора по id
struct Person* sortSurname(struct Person* head); //сортировка методом выбора по фамилии
struct Person* sortAge(struct Person* head); //сортировка методом выбора по возрасту
struct Person* sortTypeGroup(struct Person* head); //сортировка методом выбора по специальности
void printOnlyTeacherOrStudent(struct Person* head, bool is_teacher); //печатает в консоль людей с фильтром по учитель/ученик
void deletePerson(struct Person** head, int id); //удалить человека по id
void deleteFirstPerson(struct Person** head); //удалить человека на вершине списка
bool appendToFile(struct Person* person, char* datafile);//дозаписывает в файл одного человека
int get_last_id(); //максимальный id

//функция которая меняет местами узлы 'currX' и 'currY' не меняя местами их содержимое
void swapNodes(struct Person** head_ref, struct Person* currX, struct Person* currY, struct Person* prevY);
