#include "ctime"
#include "cstdlib"
#include "locale"
#include "iostream"

int main()
{
    setlocale(LC_ALL, "rus");
    int nums;
    int numnums;

    std::cout << "Введите количество строк квадратной матрицы" << "\n";
    std::cin >> nums;
    numnums = nums;

    int** arr = (int**)malloc(sizeof(int*) * nums);
    for (int i = 0; i < nums; i++) {
        arr[i] = (int*)malloc(sizeof(int) * numnums);
    }

    for (int i = 0; i < nums; i++) {
        for (int j = 0; j <= i; j++) {
            if (i == j) {
                arr[i][j] = rand() % 51 - 10; 
            }
            else {
                int value = rand() % 51 - 10;
                arr[i][j] = value;
                arr[j][i] = value; 
            }
        }
    }

    for (int i = 0; i < nums; i++) {
        for (int j = 0; j < numnums; j++) {
            std::cout << arr[i][j] << "\t";
        }
        std::cout << "\n";
    }

    free(arr);

    return 0;
}