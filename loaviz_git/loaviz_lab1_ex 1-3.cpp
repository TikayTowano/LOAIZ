#include <iostream>
#include <locale>
#include <ctime>
#include <cstdlib>

int main()
{
    void clearScreen();
    setlocale(LC_ALL, "rus");
    srand(time(NULL));


    int numOfElements;
    std::cout << "Введите количество элементов массива: " << "\n";
    std::cin >> numOfElements;
    std::cout << "Количество элементов массива: " << numOfElements << "\n";


    int* arr = new int[numOfElements];
    std::cout << "Массив из " << numOfElements << " элементов сгенерирован:" << "\n\n";
    for (int i = 0; i < numOfElements; i++) {
        arr[i] = 1 + rand() % 51 - 25;
    }
    for (int i = 0; i < numOfElements; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";


    int max = arr[0], min = arr[0];
    for (int i = 0; i < numOfElements; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    for (int i = 0; i < numOfElements; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }


    std::cout << "\n" << "Максимальный элемент: " << max << "\n";
    std::cout << "Минимальный элемент: " << min << "\n";
    int raz = max- min;
    std::cout << "Разница: " << raz << "\n";

    delete[] arr;

    return 0;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}