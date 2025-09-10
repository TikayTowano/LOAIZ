/* Добавить несколько студентов с похожими именами, переписать 
поиск так, чтобы он происходил по неполному совпадению строки, 
а также осуществить вывод всех верных вариантов вместо одного */

#include <iostream>
#include <string>
#include <locale>

void clearScreen();

struct Student {
    std::string firstName;
    std::string lastName;
    std::string group;
    int age;

    void getInfo() {
        std::cout << "Имя студента: " << firstName << "\n";
        std::cout << "Фамилия студента: " << lastName << "\n";
        std::cout << "Возраст студента: " << age << "\n";
        std::cout << "Номер группы: " << group << "\n";
    }
};

int main() {
    clearScreen();
    setlocale(LC_ALL, "rus");

    const int numOfStudents = 6;
    Student students[numOfStudents];
    
    students[0].firstName = "Ivan";
    students[0].lastName = "Plotnikov";
    students[0].group = "24VVV3";
    students[0].age = 19;

    students[1].firstName = "Bogdan";
    students[1].lastName = "Vinogradov";
    students[1].group = "24VVV3";
    students[1].age = 18;

    students[2].firstName = "Bot";
    students[2].lastName = "Botov";
    students[2].group = "23VP1";
    students[2].age = 20;

    students[3].firstName = "Boto";
    students[3].lastName = "Botov";
    students[3].group = "23VP1";
    students[3].age = 20;

    students[4].firstName = "Botto";
    students[4].lastName = "Botov";
    students[4].group = "23VP1";
    students[4].age = 20;

    students[5].firstName = "Bottto";
    students[5].lastName = "Botov";
    students[5].group = "23VP1";
    students[5].age = 20;

    for (int i = 0; i < numOfStudents; i++) {
        students[i].getInfo();
        std::cout << "\n";
    }

    std::string searchName;
    std::cout << "Введите имя студента которого хотите найти: "<< "\n";
    std::cin >> searchName;
    std::cout << "\n";
    std::cout << "Данные о студенте с таким именем: " << "\n";
    
    bool found = false;
    for (int i = 0; i < numOfStudents; i++) {
        if (students[i].firstName.find(searchName) != std::string::npos) {
            students[i].getInfo();
            std::cout << "\n";
            found = true;
        }
    }
    
    if (!found) {
        std::cout << "Студент с именем " << searchName << " не найден.\n";
    }

    return 0;
}

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}