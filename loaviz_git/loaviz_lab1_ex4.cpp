#include <iostream>
#include <locale>
#include <ctime>
#include <cstdlib>

void clearScreen();

int main()
{
    clearScreen();
    setlocale(LC_ALL, "rus");
    srand(time(NULL));

    int arr[5][5];

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            arr[i][j] = 1 + rand() % 50 - 25;
        }
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            std::cout << arr[i][j] << "\t";
        }
        std::cout << "\n";
    }

    std::cout << "\nСуммы элементов в каждой строке:\n";

    for (int i = 0; i < 5; i++) {
        int sum = 0;
        for (int j = 0; j < 5; j++) {
            sum += arr[i][j];
        }
        std::cout << "Строка " << i + 1 << ": " << sum << "\n";
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