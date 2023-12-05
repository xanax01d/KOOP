#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <locale>
#include <Windows.h>
#include <chrono>
#include <conio.h>
#include <functional>
#include <regex>
using namespace std;
//Функция для замены пароля звездами
string censorPassword() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { 
        if (ch == 8) {
            if (!password.empty()) {
                cout << "\b \b"; 
                password.pop_back();
            }
        }
        else {
            cout << "*"; 
            password.push_back(ch); 
        }
    }
    cout << endl;
    return password;
}
//Функция для подтверждения изменений
bool confirmChanges() {
    string choice;
    cout << "Вы действительно хотите внести изменения? (Д/Н)?" << endl;
    cout << ">>> ";
    cin >> choice;
    if (choice == "Д" || choice == "д") {
        return true;
    }
    else if (choice == "Н" || choice == "н") {
        return false;
    }
    else {
        return false;
    }
}
//Функция для получения целочисленного ввода пользователя
int getUserInput() {
    int input;
    cout <<  ">>> ";
    if (!(cin >> input)) {
        // Обработка случая, когда ввод не является целым числом
        cin.clear();  // Сброс флага ошибки
        while (cin.get() != '\n') continue;  // Очистка буфера от некорректного ввода
        return 0;
    }
    return input;
}
//Функция для получения строкового ввода пользователя
string getStringInput() {
    string input;
    cout << ">>> ";
    cin.ignore();
    getline(cin, input);
    return input;
}
//Функции для проверки введенной даты
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
bool isValidDate(const string& dateStr) {
    if (dateStr.length() != 10)
        return false;

    int year = stoi(dateStr.substr(0, 4));
    int month = stoi(dateStr.substr(5, 2));
    int day = stoi(dateStr.substr(8, 2));
    if (year < 0 || year > 9999) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;

    if (month == 2) {
        if (isLeapYear(year)) {
            if (day > 29) return false;
        }
        else {
            if (day > 28) return false;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    }
    else {
        if (day > 31) return false;
    }

    return true;
}
//Класс, представляющий пользователя
class User {
public:
    // Конструктор по умолчанию
    User() : username(""), password(""), role("") {}
    // Конструктор с параметрами для инициализации пользователя
    User(const string& username, const string& password, const string& role)
        : username(username), password(password), role(role) {}
    // Геттер для получения имени пользователя
    const string& getUsername() const { return username; }
    // Геттер для получения пароля пользователя
    const string& getPassword() const { return password; }
    // Геттер для получения роли пользователя
    const string& getRole() const { return role; }
    // Перегруженный оператор для получения данных из файла
    friend istream& operator>>(istream& in, User& user) {
        in >> user.username >> user.password >> user.role;
        return in;
    }
    // Метод для вывода данных о пользователе
    void printUserInfo() const {
        cout << "Имя пользователя: " << username << endl;
        cout << "Пароль: " << password << endl;
        cout << "Роль: " << role << endl;
    }
    void setPassword(string password) {
        this->password = password;
    };
    void setRole(string role) {
        this->role = role;
    }
private:
    string username; //Имя пользователя
    string password; //Пароль
    string role; //Роль
};
//Класс для управления пользователями
class UserManager {
public:
    // Метод для регистрации пользователя
    void registerUser(const string& username, const string& password, const string& role) {
        users.push_back(User(username, password, role));
         cout << "Пользователь зарегистрирован." <<  endl;
    }
    // Метод для авторизации пользователя
    int loginUser(const string& username, const string& password, const string& role) {

        system("cls");

        for (const User& user : users) {
            if (user.getUsername() == username && user.getPassword() == password && user.getRole() == role) {
                cout << "Вы успешно вошли в систему." << endl;
                return 0;
            }
            else if (user.getUsername() == username && user.getPassword() == password && user.getRole() != role) {
                cout << "Вы успешно вошли в систему как администратор" << endl;
                return 1;
            }

        }
        cout << "Неправильное имя пользователя или пароль." << endl;

    }
    // Метод для сохранения информации о пользователях в файл
    void saveUsersToFile() {
         ofstream file("users.txt");
        if (file.is_open()) {
            for (const User& user : users) {
                file << user.getUsername() << " " << user.getPassword() << " " << user.getRole() <<  endl;
            }
            file.close();
        }
        else {
             cerr << "Не удается открыть файл для записи." <<  endl;
        }
    }
    // Метод для загрузки информации о пользователях из файла
    void loadUsersFromFile() {
        ifstream file("users.txt");
        if (file.is_open()) {
            User user;
            while (file >> user) {
                users.push_back(user);
            }
            file.close();
        }
        else {
            cout << "Ошибка открытия файла" << endl;
            return;
        }
    }
    // Метод для вывода информации о всех пользователях
    void viewAllUsers() {
        for (const User& user : users) {
            user.printUserInfo();
             cout << "-------------------" <<  endl;
        }
    }
    // Метод для добавления пользователя
    void addUser(const string& username, const string& password, const string& role) {
        users.push_back(User(username, password, role));

    }
    // Метод для редактирования пользователя
    bool editUser(const string& username, const string& password, const string& role) {
        for (User& user : users) {
            if (user.getUsername() == username) {
                // Найден пользователь, обновляем информацию
                user.setPassword(password);
                user.setRole(role);
                return true;
            }
        }
        return false; // Пользователь не найден
    }
    // Метод для удаления пользователя
    bool deleteUser(const string& username) {
        for (auto it = users.begin(); it != users.end(); ++it) {
            if (it->getUsername() == username) {
                users.erase(it);
                return true;
            }
        }
        return false; 
    }
private:
    vector<User> users;
};
// Класс, представляющий запись о производстве
class Production {
public:
    int numberOfWorkbench, numberOfUnits;
    string date, nameOfProduct, responsible;
    // Конструктор по умолчанию для инициализации полей
    Production()
        :date(""), numberOfWorkbench(), nameOfProduct(""), numberOfUnits(), responsible("") {}
    // Конструктор с параметрами для установки значений полей
    Production(const string& date, int numberOfWorkbench, const string& nameOfProduct, int numberOfUnits, const string& responsible)
        : date(date), numberOfWorkbench(numberOfWorkbench), nameOfProduct(nameOfProduct), numberOfUnits(numberOfUnits), responsible(responsible) {}
    // Метод для вывода информации о производстве
    void Display() const {
        // Определение размеров отступов для выравнивания данных в таблице
        int sizeOfTabWorkbench = 20 - to_string(numberOfWorkbench).size();
        int sizeOfTabProduct = 25 - nameOfProduct.size();
        int sizeOfTabUnits = 20 - to_string(numberOfUnits).size();
        int sizeOfTabResponsible = 14 - responsible.size();
        // Вывод данных о производстве в виде таблицы
        cout <<  left;
        cout <<  setw(10)<< "| " << date
                  <<setw(sizeOfTabWorkbench) << "| " << numberOfWorkbench
                  <<setw(sizeOfTabProduct) << "| " << nameOfProduct
                  <<setw(sizeOfTabUnits) << "| " << numberOfUnits
                  <<setw(sizeOfTabResponsible) << "| " << responsible
                  << " |" <<  endl;
        cout << setfill('-') << setw(101) << "" << setfill(' ') << endl;
    }
    // Статический метод для сохранения данных в файл
    static void Save(const vector<Production>& productions) {
         ofstream file("production.txt");
        if (file.is_open()) {
            for (const Production& production : productions) {
                // Сохранение данных о производстве в файл в формате CSV
                file << production.date << ";" << production.numberOfWorkbench << ";" << production.nameOfProduct << ";" << production.numberOfUnits << ";" << production.responsible <<  endl;
            }
            file.close();
        }
        else {
             cerr << "Не удается открыть файл для записи." <<  endl;
        }
    }
};
// Класс, представляющий базу данных производства
class ProductionDatabase {
private:
    vector<Production> productions;
    // Метод для вывода заголовка таблицы
    void PrintHeader() const {
        cout <<  left;
        cout <<  setw(20) << "| Дата"
            <<  setw(20) << "| Номер цеха"
            <<  setw(25) << "| Название продукта"
            <<  setw(20) << "| Количество"
            <<  setw(20) << "| Ответсвенный |" <<  endl;

         cout <<  setfill('-') <<  setw(101) << "" <<  setfill(' ') <<  endl;
    }

public:
    int returnSize() {
        return(productions.size());
    }
    // Метод для загрузки данных из файла
    void LoadData(const string& filename) {
        ifstream fin(filename);
        ifstream file(filename);
        if (file.is_open()) {
            Production production;
            string line;
            string workbenchNumber, productCount;
                while (getline(file, line)) {
                    istringstream iss(line);
                    getline(iss, production.date, ';');
                    getline(iss, workbenchNumber, ';');
                    getline(iss, production.nameOfProduct, ';');
                    getline(iss, productCount, ';');
                    getline(iss, production.responsible);
                    production.numberOfWorkbench = stoi(workbenchNumber);
                    production.numberOfUnits = stoi(productCount);
                    productions.push_back(production);
            }
            file.close();
        }
        else {
            cout << "Ошибка открытия файла" << endl;
            return;
        }
    }
    // Метод для редактирования записи о производстве
    bool EditProduction(int index, const Production& updatedProduction) {
        if (index >= 0 && index < productions.size()) {
            productions[index] = updatedProduction;
            return true;
        }
    }
    // Метод для отображения выбранной записи о производстве
    void DisplayProduction(int i) {
        cout << "Выбранная запись для редактирования:" << endl;
        if (i >= 0 && i < productions.size()) {
            cout << "Дата: " << productions[i].date << endl;
            cout << "Номер цеха: " << productions[i].numberOfWorkbench << endl;
            cout << "Название изделия: " << productions[i].nameOfProduct << endl;
            cout << "Количество изделий: " << productions[i].numberOfUnits << endl;
            cout << "Ответственный: " << productions[i].responsible << endl;
            cout << setfill('-') << setw(29) << "" << setfill(' ') << endl;
        }
        else {
            cout << "Неверный индекс" << endl;
        }
    }
    // Метод для отображения всех записей о производстве
    void DisplayProductions() {
        PrintHeader();
        for (const auto& s : productions) {
            s.Display();
        }
        cout << endl;
    }
    // Метод для удаления записи о производстве
    void DeleteProduction(int index) {
        if (index >= 0 && index < productions.size()) {
            productions.erase(productions.begin() + index);
            cout << "Продукт успешно удален." << endl;
        }
        else {
            cout << "Неверный номер продукта." << endl;
        }
        SaveToFile();
    }
    //Метод для добавления продукта
    void AddProduction(const Production& production) {
        productions.push_back(production);
        cout << "Продукт успешно добавлен." << endl;
    }
    // Метод сортировки записей о производстве по количеству изделий
    void SortProduction(bool descending) {
        if (descending) {
            sort(productions.begin(), productions.end(), [](const Production& p1, const Production& p2) {
                return p1.numberOfUnits > p2.numberOfUnits;
                });
        }
        else {
            sort(productions.begin(), productions.end(), [](const Production& p1, const Production& p2) {
                return p1.numberOfUnits < p2.numberOfUnits;
                });
        }
    }
    // Метод для фильтрации и сортировки записей о производстве по дате
    void FilterAndSortByDate(int numberOfWorkbench, bool descending, const  string& startDate, const  string& endDate) {
         vector<Production> filteredProduction;
        auto startIt =  find_if(productions.begin(), productions.end(), [&startDate](const Production& p) {
            return p.date >= startDate;
            });

        auto endIt =  find_if(productions.begin(), productions.end(), [&endDate](const Production& p) {
            return p.date > endDate;
            });

        for (auto it = startIt; it != endIt; ++it) {
            if (numberOfWorkbench == it->numberOfWorkbench) {
                filteredProduction.push_back(*it);
            }
        }

        if (filteredProduction.empty()) {
             cout << "Нет информации по данному цеху/промежутку времени" <<  endl;
            return;
        }

        SortProduction(descending);
        PrintHeader();
        for (const auto& s : filteredProduction) {
            s.Display();
        }
         cout <<  endl;
    }
    // Метод для сохранения данных о производстве в файл
    void SaveToFile() const {
        Production::Save(productions);
    }
};

// Класс для работы с файлами
class DataFile {
public:
     string filename; // имя файла
    // Конструктор с параметром filename для инициализации имени файла
    DataFile(const  string& filename)
        : filename(filename) {}
    // Статический метод для создания файла с заданным именем
    static void CreateFile(const  string& filename) {
         ofstream file(filename);
        if (file.is_open()) {
            file.close();
             cout << "Файл создан: " << filename <<  endl;
        }
        else {
             cout << "Невозможно создать файл: " << filename <<  endl;
        }
    }

    // Статический метод для открытия файла с заданным именем для чтения
    static  ifstream OpenFile(const  string& filename) {
         ifstream file(filename);
        if (file.is_open()) {
             cout << "Файл открыт: " << filename <<  endl;
        }
        else {
             cout << "Невозможно открыть файл: " << filename <<  endl;
        }
        return file;
    }
    // Статический метод для удаления файла с заданным именем
    static void DeleteFile(const  string& filename) {
        if (remove(filename.c_str()) == 0) {
             cout << "Файл удален: " << filename <<  endl;
        }
        else {
             cerr << "Невозможно удалить файл: " << filename <<  endl;
        }
    }
};
// Главный класс, управляющий всей программой
class Menu {
public:
    // Метод для отображения меню
    void menu() {
        cout << setfill('-') << setw(66) << "" << setfill(' ') << endl;
        cout << left << "Белорусская государственная академия связи\n"
            << "Курсовая работа\n" << "По дисциплине\n"
            << "ОБЪЕКТНО-ОРИЕНТИРОВАННОЕ ПРОГРАММИРОВАНИЕ\n" << "Вариант 8\n"
            << "Разработка программы учета выпускаемой предприятием продукции\n"
            << "Разработал студент гр.ИТ-291\n" << "Климкович З.А" << endl;
        cout << setfill('-') << setw(66) << "" << setfill(' ') << endl;
        system("pause");
        system("cls");
        UserManager userManager;
        userManager.loadUsersFromFile();
        bool exitMenu = false;
        string username, password, loginUsername, loginPassword;
        string role = "0";

        while (!exitMenu) {
            displayMainMenu();
            int choice;
            choice = getUserInput();
            switch (choice) {
            case 1:
                // Регистрация пользователя
                system("cls");
                cout << setfill('-') << setw(27) << "" << setfill(' ') << endl;
                cout << "Введите имя пользователя: ";
                cin >> username;
                cout << "Введите пароль: ";
                password = censorPassword();
                cout << setfill('-') << setw(30) << "" << setfill(' ') << endl;
                userManager.registerUser(username, password, role);
                userManager.saveUsersToFile();
                system("pause");
                break;
            case 2:
                // Авторизация пользователя
                system("cls");
                cout << setfill('-') << setw(30) << "" << setfill(' ') << endl;
                cout << "Введите имя пользователя: ";
                cin >> loginUsername;
                cout << "Введите пароль: ";
                loginPassword = censorPassword();
                cout << setfill('-') << setw(30) << "" << setfill(' ') << endl;
                if (userManager.loginUser(loginUsername, loginPassword, role) == 0)
                    UserDataProduction();
                else if (userManager.loginUser(loginUsername, loginPassword, role) == 1) {
                    AdminMenu();
                }
                else if (userManager.loginUser(loginUsername, loginPassword, role) == 2) {
                    cout << "Неверные данные" << endl;
                }
                system("pause");
                break;
            case 3:
                // Выход
                system("cls");
                exitMenu = true;
                break;
            default:
                //Отработка некорректного выбора
                system("cls");
                cout << "Некорректный выбор. Попробуйте еще раз." <<  endl;
                break;
            }
        }
    }

private:
    //Метод для вывода главного меню
    void displayMainMenu() {
        cout << setfill('-') << setw(18) << "" << setfill(' ') << endl;
        cout << "Выберите действие: " << endl;
        cout << "1. Регистрация" << endl;
        cout << "2. Вход в аккаунт" << endl;
        cout << "3. Выход" << endl;
        cout << setfill('-') << setw(18) << "" << setfill(' ') << endl;


    }

    // Метод для меню администратора
    void AdminMenu() {
        bool exitAdminMenu = false;
        while (!exitAdminMenu) {
            displayAdminMenu();
            int choice;
            choice = getUserInput();

            switch (choice) {
            case 1:
                // Вывод информации о пользователях
                system("cls");
                UserData();
                break;
            case 2:
                // Управление файлами
                system("cls");
                dataFileMenu();
                break;
            case 3:
                // Управление базой данных продукции производства
                system("cls");
                ProductionMenu();
                break;
            case 4:
                // Выход
                system("cls");
                exitAdminMenu = true;
                break;
            default:
                system("cls");
                cout << "Некорректный выбор. Попробуйте еще раз." << endl;
                break;
            }
        }
    }
    //Метод для отображения меню администратора
    void displayAdminMenu() {
        cout << setfill('-') << setw(45) << "" << setfill(' ') << endl;
        cout << "Выберите действие" << endl;
        cout << "1. Управление учетными записями пользователей" << endl;
        cout << "2. Работа с файлом данных" << endl;
        cout << "3. Работа с данными" << endl;
        cout << "4. Выход" << endl;
        cout << setfill('-') << setw(45) << "" << setfill(' ') << endl;
    }

    // Метод для меню управления файлами
    void dataFileMenu() {
        bool c;
        bool exitDataFileMenu = false;
        string filename;
        int choice;
        while (!exitDataFileMenu)
        {
            displayDataFileMenu();
            choice = getUserInput();
            switch (choice) {
            case 1:
                // Создать файл
                c = confirmChanges();
                if (c == true) {
                    system("cls");
                    cout << setfill('-') << setw(25) << "" << setfill(' ') << endl;
                    cout << "Введите название файла: ";
                    cin >> filename;
                    DataFile::CreateFile(filename);
                    cout << setfill('-') << setw(25) << "" << setfill(' ') << endl;
                }
                else{
                    cout << "Откат изменений" << endl;
                    cout << setfill('-') << setw(25) << "" << setfill(' ') << endl;
                }
                system("pause");
                break;
            case 2:
                // Открыть файл
                system("cls");
                cout << setfill('-') << setw(25) << "" << setfill(' ') << endl;
                cout << "Введите название файла: ";
                cin >> filename;
                DataFile::OpenFile(filename);
                cout << setfill('-') << setw(25) << "" << setfill(' ') << endl;
                system("pause");
                break;
            case 3:
                // Удалить файл
                c = confirmChanges();
                if (c == true) {
                    system("cls");
                    cout << setfill('-') << setw(25) << "" << setfill(' ') << endl;
                    cout << "Введите название файла: ";
                    cin >> filename;
                    DataFile::DeleteFile(filename);
                    cout << setfill('-') << setw(25) << "" << setfill(' ') << endl;
                }
                else{
                    cout << "Откат изменений" << endl;
                    cout << setfill('-') << setw(25) << "" << setfill(' ') << endl;
                }
                system("pause");
                break;
            case 4:
                // Выход
                system("cls");
                exitDataFileMenu = true;
                break;
            default:
                // Некорректный выбор
                system("cls");
                cout << "Некорректный выбор. Попробуйте еще раз." << endl;
                break;
            }
        }
    }
    //Метод для вывода меню управления файлами
    void displayDataFileMenu() {
        cout << setfill('-') << setw(18) << "" << setfill(' ') << endl;
        cout << "Выберите действие:" << endl;
        cout << "1. Создать файл" << endl;
        cout << "2. Открыть файл" << endl;
        cout << "3. Удалить файл" << endl;
        cout << "4. Вернуться назад" << endl;
        cout << setfill('-') << setw(18) << "" << setfill(' ') << endl;
    }

    //Метод для меню управления пользователями
    void UserData()
    {
        UserManager userManager;
        userManager.loadUsersFromFile();
        string username, password, role;
        User user(username, password, role);
        bool exitUserChangeMenu = false;

        while (!exitUserChangeMenu)
        {
            bool c;
            displayUserData();
            int choice;
            choice = getUserInput();


            switch (choice) {
            case 1:
                // Вывод всех пользователей
                system("cls");
                userManager.viewAllUsers();
                break;
            case 2:
                // Создание пользователя
                c = confirmChanges();
                if (c == true) {
                    system("cls");
                    cout << "Введите нового пользователя,пароль,роль" << endl;
                    username = getStringInput();
                    password = getStringInput();
                    role = getStringInput();

                    userManager.addUser(username, password, role);
                    userManager.saveUsersToFile();
                    system("pause");
                }
                else{
                    cout << "Откат изменений" << endl;
                    system("pause");
                }
                break;
            case 3:
                // Изменение пользователя
                c = confirmChanges();
                if (c == true) {
                    system("cls");
                    cout << setfill('-') << setw(33) << "" << setfill(' ') << endl;
                    cout << "Введите имя пользователя: " << endl;
                    username = getStringInput();
                    cout << "Введите новый пароль:" << endl;
                    password = getStringInput();
                    cout << "Введите роль пользователя(0/1): " << endl;
                    role = getStringInput();
                    if (userManager.editUser(username, password,role))
                    {
                        cout << "Пользователь изменен" << endl;
                        cout << setfill('-') << setw(33) << "" << setfill(' ') << endl;
                        userManager.saveUsersToFile();
                    }
                    else{
                        cout << "Пользователь не найден" << endl;
                        cout << setfill('-') << setw(33) << "" << setfill(' ') << endl;
                    }
             
                }
                else{
                    cout << "Откат изменений" << endl;
                }
                system("pause");
                system("cls");
                break;
            case 4:
                // Удаление пользователя
                c = confirmChanges();
                if (c == true) {
                    system("cls");
                    cout << setfill('-') << setw(34) << "" << setfill(' ') << endl;
                    cout << "Введите пользователя для удаления" << endl;
                    cin >> username;
                    if (userManager.deleteUser(username))
                    {
                        userManager.saveUsersToFile();
                        cout << "Пользователь удален" << endl;
                        cout << setfill('-') << setw(33) << "" << setfill(' ') << endl;
                    }
                    else{
                        cout << "Пользователь не найден" << endl;
                        cout << setfill('-') << setw(33) << "" << setfill(' ') << endl;
                    }
                }
                else{
                    cout << "Откат изменений" << endl;
                    cout << setfill('-') << setw(33) << "" << setfill(' ') << endl;
                }
                break;
            case 5:
                // Выход
                system("cls");
                exitUserChangeMenu = true;
                break;
            default:
                // Некорректный выбор
                system("cls");
                cout << "Некорректный выбор. Попробуйте еще раз." << endl;
                break;
            }
        }
    }
    // Метод для отображения подменю управления учетными записями пользователей
    void displayUserData() {
        cout << setfill('-') << setw(33) << "" << setfill(' ') << endl;
        cout << "Выберите действие:" << endl;
        cout << "1. Просмотреть все учетные записи" << endl;
        cout << "2. Добавить учетную запись" << endl;
        cout << "3. Отредактировать учетную запись" << endl;
        cout << "4. Удалить учетную запись" << endl;
        cout << "5. Вернуться назад" << endl;
        cout << setfill('-') << setw(33) << "" << setfill(' ') << endl;
    }

    // Меню для управления продукцией
    void ProductionMenu()
    {
        string date, nameOfProduct, responsible;
        bool c;
        int numberOfWorkbench = 1;
        int numberOfUnits = 1;
        Production production(date, numberOfWorkbench, nameOfProduct, numberOfUnits, responsible);
        bool exitProductionMenu = false;
        int i = 0;
        ProductionDatabase Productiondatabase;
        string filename = "production.txt";

        Productiondatabase.LoadData(filename);
        while (!exitProductionMenu) {
            displayProductionMenu();
            int choice;
            choice = getUserInput();

            switch (choice) {
            case 1:
                // Вывод продукции
                system("cls");
                Productiondatabase.DisplayProductions();
                break;
            case 2:
                c = confirmChanges();
                // Добавление записи
                if (c == true) {
                    system("cls");
                    cout << setfill('-') << setw(29) << "" << setfill(' ') << endl;
                    do{
                        cout << "Введите дату: " << endl;
                        cout << ">>>";
                        cin >> date;
                    } while (!isValidDate(date));
                    cout << "Введите номер цеха: " << endl;
                    numberOfWorkbench = getUserInput();
                    cout << "Введите название изделия: " << endl;
                    nameOfProduct = getStringInput();
                    cout << "Введите кол-во изделий: " << endl;
                    numberOfUnits = getUserInput();
                    cout << "Введите ФИО ответственного: " << endl;
                    responsible = getStringInput();
                    cout << setfill('-') << setw(29) << "" << setfill(' ') << endl;
                    production = Production(date, numberOfWorkbench, nameOfProduct, numberOfUnits, responsible);
                    Productiondatabase.AddProduction(production);
                    Productiondatabase.SaveToFile();
                    system("pause");
                }
                else
                    cout << "Откат изменений" << endl;
                break;
            case 3:
                // Удаление записи
                c = confirmChanges();
                if (c == true) {
                    system("cls");
                    cout << setfill('-') << setw(27) << "" << setfill(' ') << endl;
                    cout << "Введите номер для удаления" << endl;
                    i = getUserInput();
                    Productiondatabase.DeleteProduction(i);
                    cout << setfill('-') << setw(27) << "" << setfill(' ') << endl;
                    system("pause");
                }
                else
                    cout << "Откат изменений" << endl;
                break;
            case 4:
                // Редактирование записи
                c = confirmChanges();
                if (c == true) {
                    system("cls");
                    cout << setfill('-') << setw(29) << "" << setfill(' ') << endl;
                    cout << "Введите номер для изменения" << endl;
                    i = getUserInput();
                    if (i > Productiondatabase.returnSize()) {
                        cout << "Неверный индекс";
                        break;
                    }
                    cout << setfill('-') << setw(29) << "" << setfill(' ') << endl;
                    Productiondatabase.DisplayProduction(i);
                    do {
                        cout << "Введите новую дату: " << endl;
                        cout << ">>> ";
                        cin >> date;
                    } while (!isValidDate(date));
                    cout << "Введите новый номер цеха: " << endl;
                    numberOfWorkbench = getUserInput();
                    cout << "Введите новое название изделия: " << endl;
                    nameOfProduct = getStringInput();
                    cout << "Введите новое количество изделий: " << endl;
                    numberOfUnits = getUserInput();
                    cout << "Введите новое ФИО ответственного: " << endl;
                    responsible = getStringInput();
                    cout << setfill('-') << setw(29) << "" << setfill(' ') << endl;
                    Production updatedProduction(date, numberOfWorkbench, nameOfProduct, numberOfUnits, responsible);
                    if(Productiondatabase.EditProduction(i, updatedProduction)){
                        Productiondatabase.SaveToFile();
                        cout << "Запись успешно отредактированна.\n";
                    }
                    else {
                        cout << "Ошибка при редактировании записи.\n";
                    }
                }
                else{
                    cout << "Откат изменений" << endl;
                }
                system("pause");
                system("cls");
                break;
            case 5:
                // Выход
                system("cls");
                exitProductionMenu = true;
                break;
            default:
                // Некорректный выбор
                system("cls");
                cout << "Некорректный выбор. Попробуйте еще раз." << endl;
                break;
            }
        }
    }
    // Метод для отображения подменю работы с данными о производстве
    void displayProductionMenu() {
         cout << setfill('-') << setw(25) << "" << setfill(' ') << endl;
         cout << "Выберите действие:" <<  endl;
         cout << "1. Просмотреть все данные" <<  endl;
         cout << "2. Добавить запись" <<  endl;
         cout << "3. Удалить запись" <<  endl;
         cout << "4. Редактировать запись" <<  endl;
         cout << "5. Вернуться назад" <<  endl;
         cout << setfill('-') << setw(25) << "" << setfill(' ') << endl;
    }

    // Меню управления продукцией в рамках пользователя
    void UserDataProduction()
    {
        bool exitUserDataProduction = false;
        string filename = "production.txt";
        ProductionDatabase Productiondatabase;
        int choice = 0;
        Productiondatabase.LoadData(filename);
        while (!exitUserDataProduction)
        {
            displayUserDataProduction();
            choice = getUserInput();
            switch (choice)
            {
            case 1:
                system("cls");
                // Вывод продукции
                Productiondatabase.DisplayProductions();
                break;
            case 2:
                // Сортировка и фильтрация по дате по убыванию
                system("cls");
                {
                    cout << setfill('-') << setw(46) << "" << setfill(' ') << endl;
                    int numberOfWorkbench;
                    string startDate, endDate;
                    do{
                        cout << "Введите начало периода в формате ГГГГ-ММ-ДД: " << endl;
                        cout << ">>>";
                        cin >> startDate;
                    } while (!isValidDate(startDate));
                    do{
                        cout << "Введите конец периода в формате ГГГГ-ММ-ДД: " << endl;
                        cout << ">>>";
                        cin >> endDate;
                    } while (!isValidDate(endDate));
                    cout << "Введите номер цеха" << endl;
                    numberOfWorkbench = getUserInput();
                    cout << setfill('-') << setw(46) << "" << setfill(' ') << endl;
                    Productiondatabase.FilterAndSortByDate(numberOfWorkbench, true, startDate, endDate);
                    break;
                }
                break;
            case 3:
                // Сортировка и фильтрация по дате по возрастанию
                system("cls");
                {
                    cout << setfill('-') << setw(46) << "" << setfill(' ') << endl;
                    int numberOfWorkbench;
                    string startDate, endDate;
                    cout << "Введите начало периода в формате ГГГГ-ММ-ДД: " << endl;
                    startDate = getStringInput();
                    cout << "Введите конец периода в формате ГГГГ-ММ-ДД: " << endl;
                    endDate = getStringInput();
                    cout << "Введите номер цеха";
                    numberOfWorkbench = getUserInput();
                    cout << setfill('-') << setw(46) << "" << setfill(' ') << endl;
                    Productiondatabase.FilterAndSortByDate(numberOfWorkbench, false, startDate, endDate);
                    break;
                }
                break;
            case 4:
                system("cls");
                {
                    // Выход
                    Productiondatabase.SaveToFile();
                    exitUserDataProduction = true;
                    break;
                }
                break;
            default:
                // Некорректный выбор
                system("cls");
                cout << "Некорректный выбор. Попробуйте еще раз." << endl;
                break;
            }
        }
    }
    // Метод для отображения подменю данных о продукции в рамках обычного пользователя пользователя
    void displayUserDataProduction() {
        cout << setfill('-') << setw(127) << "" << setfill(' ') << endl;
        cout << "Выберите действие:" << endl;
        cout << "1. Вывести информацию о продукции." << endl;
        cout << "2. Вывести список продукции произведенной в одном цеху за определенный промежуток времени по убыванию количества продукции." << endl;
        cout << "3. Вывести список продукции произведенной в одном цеху за определенный промежуток времени по возрастанию количества продукции." << endl;
        cout << "4. Выход" << endl;
        cout << setfill('-') << setw(127) << "" << setfill(' ') << endl;
    }

};
// Главная функция
int main() {
    // Установка русского вывода в консоли
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // Установка русской локали
    setlocale(LC_ALL, "rus");
    // Инициализация главного меню
    Menu menu;
    //Вызов главного меню
    menu.menu();
    return 0;
}