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
	int classe; //����� ���������� �������
	int time[2]; //������������ ��� �������� ������� ������ ������� � ������. time[0] - ����� ������ �������, time[1]- ����� ����� �������
	int type; //��� ������
	struct School* school; //����������� ����� 
};

struct School
{
	char sch_name[100];
	char address[100];
	int id;
};


void writePerson(struct Person* head, char* datafile); //�������� ����� � ����
struct Person* readPerson(char* datafile, struct School* school); //��������� ����� �� �����
struct Person* findById(struct Person* head, int id); //����� �������� �� id
void addPerson(struct Person** head, char* surname, char* name, char* patronymic, int age, bool is_teacher, struct Group* group, char* datafile); //�������� ������������
void printPersonsList(struct Person* head); //�������� � ������� �����
struct Person* sortId(struct Person* head); //���������� ������� ������ �� id
struct Person* sortSurname(struct Person* head); //���������� ������� ������ �� �������
struct Person* sortAge(struct Person* head); //���������� ������� ������ �� ��������
struct Person* sortTypeGroup(struct Person* head); //���������� ������� ������ �� �������������
void printOnlyTeacherOrStudent(struct Person* head, bool is_teacher); //�������� � ������� ����� � �������� �� �������/������
void deletePerson(struct Person** head, int id); //������� �������� �� id
void deleteFirstPerson(struct Person** head); //������� �������� �� ������� ������
bool appendToFile(struct Person* person, char* datafile);//������������ � ���� ������ ��������
int get_last_id(); //������������ id

//������� ������� ������ ������� ���� 'currX' � 'currY' �� ����� ������� �� ����������
void swapNodes(struct Person** head_ref, struct Person* currX, struct Person* currY, struct Person* prevY);
