#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void clearScreen();
int** createArr(int n);
void freeArr(int** arr, int n);
void fillArr(int** arr, int n);
double multiplyArr(int n);
void runCalculations();

int main() {
    clearScreen();
    runCalculations();
    return 0;
}

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

int** createArr(int n) {
    int** arr = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        arr[i] = (int*)malloc(n * sizeof(int));
    }
    return arr;
}

void freeArr(int** arr, int n) {
    for (int i = 0; i < n; i++) {
        free(arr[i]);
    }
    free(arr);
}

void fillArr(int** arr, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            arr[i][j] = rand() % 100 + 1;
        }
    }
}

double multiplyArr(int n) {
    int** a = createArr(n);
    int** b = createArr(n);
    int** c = createArr(n);

    fillArr(a, n);
    fillArr(b, n);

    clock_t start = clock();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = 0;
            for (int k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    clock_t end = clock();
    double timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC;

    freeArr(a, n);
    freeArr(b, n);
    freeArr(c, n);

    return timeTaken;
}

void runCalculations() {
    int sizes[] = { 100, 200, 400, 1000, 2000, 4000, 10000 };
    int numSize = sizeof(sizes) / sizeof(sizes[0]);

    FILE* file = fopen("rez.txt", "w");
    if (file == NULL) {
        printf("Ошибка открытия файла!\n");
        return;
    }

    fprintf(file, "Размер матрицы\tВремя (сек)\n");

    srand(time(NULL));

    for (int i = 0; i < numSize; i++) {
        int n = sizes[i];
        printf("Вычисление для матрицы %dx%d...\n", n, n);

        double timeTaken = multiplyArr(n);

        fprintf(file, "%d\t%.6f\n", n, timeTaken);

        printf("Завершено: %dx%d - время: %.6f сек.\n", n, n, timeTaken);
    }

    fclose(file);
    printf("Результаты записаны в файл rez.txt\n");
}