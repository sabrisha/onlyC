bool file_exists(char* filename); //�������� ������������� �����


struct Group* EntryGroup(); //���� ������

//���������� ������ � ����
void writeFile(bool entry = false);

//����������� ������
void freeAll(bool only_persons = true);

void createSchool();

//������ ������ �� �����
void readFile(bool entry = false);

//�������� � ������� ���� ������� ������
void _menu1Print();

void addPersonUI();//����������� � ������������ ���������e �������

void changeSchool(); //�������� �����

void editPersonUI(); //�������� ��������

void changeGroup();//�������� ������

void filterMenu();//���� �������� � ����������

void userManagmentMenu();//���� ���������� ��������������

void menuAdmin();//���� ��������������

void menuUser();//���� ������������

void indMenu();//���� � �������������� ��������

void menu1();//���� ������� ������

void ini();//��������� ������������� ������. ���������� �������������, ��������� ����� ������

