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
using namespace std;

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
string getUserInput() {
    string input;
    cout << ">>> ";
    cin >> input;
    return input;
}

class User {
public:
    User() : username(""), password(""), role("") {}
    User(const string& username, const string& password, const string& role)
        : username(username), password(password), role(role) {}

    const string& getUsername() const { return username; }
    const string& getPassword() const { return password; }
    const string& getRole() const { return role; }

    friend istream& operator>>(istream& in, User& user) {
        in >> user.username >> user.password >> user.role;
        return in;
    }
    void printUserInfo() const {
        cout << "Имя пользователя: " << username << endl;
        cout << "Пароль: " << password << endl;
        cout << "Роль: " << role << endl;
    }
private:
    string username;
    string password;
    string role;
};
class UserManager {
public:
    void registerUser(const string& username, const string& password, const string& role) {
        users.push_back(User(username, password, role));
         cout << "Пользователь зарегистрирован." <<  endl;
    }

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

    void loadUsersFromFile() {
        ifstream file("users.txt");
        if (file.is_open()) {
            User user;
            while (file >> user) { // Здесь используется оператор >> для чтения данных
                users.push_back(user);
            }
            file.close();
        }
        else {
            cout << "Ошибка открытия файла" << endl;
            return;
        }
    }

    void viewAllUsers() {
        for (const User& user : users) {
            user.printUserInfo();
             cout << "-------------------" <<  endl;
        }
    }

    // Метод для добавления учетной записи
    void addUser(const string& username, const string& password, const string& role) {
        users.push_back(User(username, password, role));

    }

    // Метод для редактирования учетной записи по имени пользователя
    bool editUser(const string& username, const string& password, const string& role) {
        for (User& user : users) {
            if (user.getUsername() == username) {
                deleteUser(username);
                addUser(username, password, role);
                return true;
            }
        }
        return false; // Пользователь не найден
    }

    // Метод для удаления учетной записи по имени пользователя
    bool deleteUser(const string& username) {
        for (auto it = users.begin(); it != users.end(); ++it) {
            if (it->getUsername() == username) {
                users.erase(it);
                return true;
            }
        }
        return false; // Пользователь не найден
    }
private:
    vector<User> users;
};

class Production {
public:
    int numberOfWorkbench, numberOfUnits;
    string date, nameOfProduct, responsible;

    Production()
        :date(""), numberOfWorkbench(), nameOfProduct(""), numberOfUnits(), responsible("") {}

    Production(const string& date, int numberOfWorkbench, const string& nameOfProduct, int numberOfUnits, const string& responsible)
        : date(date), numberOfWorkbench(numberOfWorkbench), nameOfProduct(nameOfProduct), numberOfUnits(numberOfUnits), responsible(responsible) {}



    void Display() const {
        int sizeOfTabWorkbench = 20 - to_string(numberOfWorkbench).size();
        int sizeOfTabProduct = 25 - nameOfProduct.size();
        int sizeOfTabUnits = 20 - to_string(numberOfUnits).size();
        int sizeOfTabResponsible = 14 - responsible.size();
        cout <<  left;
        cout <<  setw(10)<< "| " << date
                  <<setw(sizeOfTabWorkbench) << "| " << numberOfWorkbench
                  <<setw(sizeOfTabProduct) << "| " << nameOfProduct
                  <<setw(sizeOfTabUnits) << "| " << numberOfUnits
                  <<setw(sizeOfTabResponsible) << "| " << responsible
                  << " |" <<  endl;
        cout << setfill('-') << setw(101) << "" << setfill(' ') << endl;
    }

    static void Save(const vector<Production>& productions) {
         ofstream file("production.txt");
        if (file.is_open()) {
            for (const Production& production : productions) {
                file << production.date << ";" << production.numberOfWorkbench << ";" << production.nameOfProduct << ";" << production.numberOfUnits << ";" << production.responsible <<  endl;
            }
            file.close();
        }
        else {
             cerr << "Не удается открыть файл для записи." <<  endl;
        }
    }
    friend istream& operator>>( istream& in, Production& production) {
        in >> production.date >> production.numberOfWorkbench >> production.nameOfProduct >> production.numberOfUnits >> production.responsible;
        return in;
    }

};
class ProductionDatabase {
private:
    vector<Production> productions;

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
    void LoadData(const string& filename) {
        ifstream fin(filename);
        ifstream file(filename);
        if (file.is_open()) {
            Production production;
            string line;
            string date, workbenchNumber, productName, productCount, responsiblePerson;
                while (getline(file, line)) {
                    istringstream iss(line);
                    getline(iss, date, ';');
                    getline(iss, workbenchNumber, ';');
                    getline(iss, productName, ';');
                    getline(iss, productCount, ';');
                    getline(iss, responsiblePerson);
                    production.date = date;
                    production.numberOfWorkbench = stoi(workbenchNumber);
                    production.nameOfProduct = productName;
                    production.numberOfUnits = stoi(productCount);
                    production.responsible = responsiblePerson;
                    productions.push_back(production);
            }
            file.close();
        }
        else {
            cout << "Ошибка открытия файла" << endl;
            return;
        }
    }

    void DisplayProductions() {
        PrintHeader();
        for (const auto& s : productions) {
            s.Display();
        }
        cout << endl;
    }

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

    void AddProduction(const Production& production) {
        productions.push_back(production);
        cout << "Продукт успешно добавлен." << endl;
    }
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

    void SaveToFile() const {
        Production::Save(productions);
    }
};


class DataFile {
public:
     string filename;

    DataFile(const  string& filename)
        : filename(filename) {}

    // Создать файл
    static void CreateFile(const  string& filename) {
         ofstream file(filename);
        if (file.is_open()) {
            file.close();
             cout << "Файл создан: " << filename <<  endl;
        }
        else {
             cerr << "Невозможно создать файл: " << filename <<  endl;
        }
    }

    // Открыть файл
    static  ifstream OpenFile(const  string& filename) {
         ifstream file(filename);
        if (file.is_open()) {
             cout << "Файл открыт: " << filename <<  endl;
        }
        else {
             cerr << "Невозможно открыть файл: " << filename <<  endl;
        }
        return file;
    }

    // Удалить файл
    static void DeleteFile(const  string& filename) {
        if (remove(filename.c_str()) == 0) {
             cout << "Файл удален: " << filename <<  endl;
        }
        else {
             cerr << "Невозможно удалить файл: " << filename <<  endl;
        }
    }
};

class Menu {
public:
    void menu() {
        UserManager userManager;
        userManager.loadUsersFromFile();
        bool exitMenu = false;
        string username, password, loginUsername, loginPassword;
        string role = "0";

        while (!exitMenu) {
            displayMainMenu();
            int choice;
            choice = stoi(getUserInput());
            switch (choice) {
            case 1:
                system("cls");
                cout << "Введите имя пользователя: ";
                cin >> username;
                cout << "Введите пароль: ";
                password = censorPassword();
                userManager.registerUser(username, password, role);
                userManager.saveUsersToFile();
                break;
            case 2:
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

                break;
            case 3:
                system("cls");
                exitMenu = true;
                break;
            default:
                system("cls");
                cout << "Некорректный выбор. Попробуйте еще раз." <<  endl;
                break;
            }
        }
    }

private:
    void displayMainMenu() {
        cout << setfill('-') << setw(18) << "" << setfill(' ') << endl;
        cout << "Выберите действие: " << endl;
        cout << "1. Регистрация" << endl;
        cout << "2. Вход в аккаунт" << endl;
        cout << "3. Выход" << endl;
        cout << setfill('-') << setw(18) << "" << setfill(' ') << endl;


    }


    void AdminMenu() {
        bool exitAdminMenu = false;
        while (!exitAdminMenu) {
            displayAdminMenu();
            int choice;
            choice = stoi(getUserInput());

            switch (choice) {
            case 1:
                system("cls");
                UserData();
                break;
            case 2:
                system("cls");
                dataFileMenu();
                break;
            case 3:
                system("cls");
                ProductionMenu();
                break;
            case 4:
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
    void displayAdminMenu() {
        cout << setfill('-') << setw(45) << "" << setfill(' ') << endl;
        cout << "Выберите действие" << endl;
        cout << "1. Управление учетными записями пользователей" << endl;
        cout << "2. Работа с файлом данных" << endl;
        cout << "3. Работа с данными" << endl;
        cout << "4. Выход" << endl;
        cout << setfill('-') << setw(45) << "" << setfill(' ') << endl;
    }


    void dataFileMenu() {
        bool c;
        bool exitDataFileMenu = false;
        string filename;
        int choice;
        while (!exitDataFileMenu)
        {
            displayDataFileMenu();
            choice = stoi(getUserInput());


            switch (choice) {
            case 1:
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
                break;
            case 2:
                system("cls");
                cout << setfill('-') << setw(25) << "" << setfill(' ') << endl;
                cout << "Введите название файла: ";
                cin >> filename;
                DataFile::OpenFile(filename);
                cout << setfill('-') << setw(25) << "" << setfill(' ') << endl;
                break;
            case 3:
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
                break;
            case 4:
                system("cls");
                exitDataFileMenu = true;
                break;
            default:
                system("cls");
                cout << "Некорректный выбор. Попробуйте еще раз." << endl;
                break;
            }
        }
    }
    void displayDataFileMenu() {
        cout << setfill('-') << setw(18) << "" << setfill(' ') << endl;
        cout << "Выберите действие:" << endl;
        cout << "1. Создать файл" << endl;
        cout << "2. Открыть файл" << endl;
        cout << "3. Удалить файл" << endl;
        cout << "4. Вернуться назад" << endl;
        cout << setfill('-') << setw(18) << "" << setfill(' ') << endl;
    }


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
            choice = stoi(getUserInput());


            switch (choice) {
            case 1:
                system("cls");
                userManager.viewAllUsers();
                break;
            case 2:
                c = confirmChanges();
                if (c == true) {
                    system("cls");
                    cout << "Введите нового пользователя,пароль,роль" << endl;
                    username = getUserInput();
                    password = getUserInput();
                    role = getUserInput();

                    userManager.addUser(username, password, role);
                    userManager.saveUsersToFile();
                }
                else
                    cout << "Откат изменений" << endl;
                break;
            case 3:
                c = confirmChanges();
                if (c == true) {
                    system("cls");
                    cout << setfill('-') << setw(33) << "" << setfill(' ') << endl;
                    cout << "Введите имя пользователя: " << endl;
                    username = getUserInput();
                    cout << "Введите пароль:";
                    password = getUserInput();
                    role = getUserInput();
                    if (userManager.editUser(username, password, role))
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
                else
                    cout << "Откат изменений" << endl;
                break;
            case 4:
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
                system("cls");
                exitUserChangeMenu = true;
                break;
            default:
                system("cls");
                cout << "Некорректный выбор. Попробуйте еще раз." << endl;
                break;
            }
        }
    }
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
            choice = stoi(getUserInput());


            switch (choice) {
            case 1:
                system("cls");
                Productiondatabase.DisplayProductions();
                break;
            case 2:
                c = confirmChanges();
                if (c == true) {
                    system("cls");
                    cout << setfill('-') << setw(29) << "" << setfill(' ') << endl;
                    cout << "Введите дату: " << endl;
                    date = getUserInput();
                    cout << "Введите номер цеха: " << endl;
                    numberOfWorkbench = stoi(getUserInput());
                    cout << "Введите название изделия: " << endl;
                    nameOfProduct = getUserInput();
                    cout << "Введите кол-во изделий: " << endl;
                    numberOfUnits = stoi(getUserInput());
                    cout << "Введите ФИО ответственного: " << endl;
                    responsible = getUserInput();
                    cout << setfill('-') << setw(29) << "" << setfill(' ') << endl;
                    production = Production(date, numberOfWorkbench, nameOfProduct, numberOfUnits, responsible);
                    Productiondatabase.AddProduction(production);
                    Productiondatabase.SaveToFile();
                }
                else
                    cout << "Откат изменений" << endl;
                break;
            case 3:
                c = confirmChanges();
                if (c == true) {
                    system("cls");
                    cout << setfill('-') << setw(27) << "" << setfill(' ') << endl;
                    cout << "Введите номер для удаления" << endl;
                    i = stoi(getUserInput());
                    Productiondatabase.DeleteProduction(i);
                    cout << setfill('-') << setw(27) << "" << setfill(' ') << endl;
                }
                else
                    cout << "Откат изменений" << endl;
                break;
            case 4:
                c = confirmChanges();
                if (c == true) {
                    system("cls");
                    cout << setfill('-') << setw(29) << "" << setfill(' ') << endl;
                    cout << "Введите номер для изменения" << endl;
                    i = stoi(getUserInput());
                    Productiondatabase.DeleteProduction(i);
                    cout << "Введите дату: " << endl;
                    date = getUserInput();
                    cout << "Введите номер цеха: " << endl;
                    numberOfWorkbench = stoi(getUserInput());
                    cout << "Введите название изделия: " << endl;
                    nameOfProduct = getUserInput();
                    cout << "Введите кол-во изделий: " << endl;
                    numberOfUnits = stoi(getUserInput());
                    cout << "Введите ФИО ответственного: " << endl;
                    responsible = getUserInput();
                    cout << setfill('-') << setw(29) << "" << setfill(' ') << endl;
                    production = Production(date, numberOfWorkbench, nameOfProduct, numberOfUnits, responsible);
                    Productiondatabase.AddProduction(production);
                    Productiondatabase.SaveToFile();
                }
                else
                    cout << "Откат изменений" << endl;
                break;
            case 5:
                system("cls");
                exitProductionMenu = true;
                break;
            default:
                system("cls");
                cout << "Некорректный выбор. Попробуйте еще раз." << endl;
                break;
            }
        }
    }
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
            choice = stoi(getUserInput());
            switch (choice)
            {
            case 1:
                system("cls");
                Productiondatabase.DisplayProductions();
                break;
            case 2:
                system("cls");
                {
                    cout << setfill('-') << setw(46) << "" << setfill(' ') << endl;
                    int numberOfWorkbench;
                    string startDate, endDate;
                    cout << "Введите начало периода в формате ГГГГ-ММ-ДД: " << endl;
                    startDate = getUserInput();
                    cout << "Введите конец периода в формате ГГГГ-ММ-ДД: " << endl;
                    endDate = getUserInput();
                    cout << "Введите номер цеха" << endl;
                    numberOfWorkbench = stoi(getUserInput());
                    cout << setfill('-') << setw(46) << "" << setfill(' ') << endl;
                    Productiondatabase.FilterAndSortByDate(numberOfWorkbench, true, startDate, endDate);
                    break;
                }
                break;
            case 3:
                system("cls");
                {
                    cout << setfill('-') << setw(46) << "" << setfill(' ') << endl;
                    int numberOfWorkbench;
                    string startDate, endDate;
                    cout << "Введите начало периода в формате ГГГГ-ММ-ДД: " << endl;
                    startDate = getUserInput();
                    cout << "Введите конец периода в формате ГГГГ-ММ-ДД: " << endl;
                    endDate = getUserInput();
                    cout << "Введите номер цеха";
                    numberOfWorkbench = stoi(getUserInput());
                    cout << setfill('-') << setw(46) << "" << setfill(' ') << endl;
                    Productiondatabase.FilterAndSortByDate(numberOfWorkbench, false, startDate, endDate);
                    break;
                }
                break;
            case 4:
                system("cls");
                {
                    Productiondatabase.SaveToFile();
                    exitUserDataProduction = true;
                    break;
                }
                break;
            default:
                system("cls");
                cout << "Некорректный выбор. Попробуйте еще раз." << endl;
                break;
            }
        }
    }
    void displayUserDataProduction() {
        cout << setfill('-') << setw(102) << "" << setfill(' ') << endl;
        cout << "Выберите действие:" << endl;
        cout << "1. Вывести информацию о продуктах" << endl;
        cout << "2. Вывести список товаров произведенных в одном цеху за определенный промежуток времени по убыванию" << endl;
        cout << "3. Вывести список товаров произведенных в одном цеху за определенный промежуток времени по возрастанию" << endl;
        cout << "4. Выход" << endl;
        cout << setfill('-') << setw(102) << "" << setfill(' ') << endl;
    }

};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    Menu menu;
    menu.menu();
    return 0;
}