bool file_exists(char* filename); //проверка существования файла


struct Group* EntryGroup(); //ввод группы

//записывает данные в файл
void writeFile(bool entry = false);

//освобождает память
void freeAll(bool only_persons = true);

void createSchool();

//читает данные из файла
void readFile(bool entry = false);

//печатает в консоль меню первого уровня
void _menu1Print();

void addPersonUI();//запрашивает у пользователя добавлениe человка

void changeSchool(); //поменять школу

void editPersonUI(); //поменять человека

void changeGroup();//поменять группу

void filterMenu();//меню фильтров и сортировки

void userManagmentMenu();//меню управления пользователями

void menuAdmin();//меню администратора

void menuUser();//меню пользователя

void indMenu();//меню с индивидуальным заданием

void menu1();//меню первого уровня

void ini();//первичная инициализация данных. считывание пользователей, установка файла данных

