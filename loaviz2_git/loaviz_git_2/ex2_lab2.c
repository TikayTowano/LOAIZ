#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void clearScreen();

void shell(int* items, int count) {
    int i, j, gap, k;
    int x, a[5];
    a[0] = 9; a[1] = 5; a[2] = 3; a[3] = 2; a[4] = 1;

    for (k = 0; k < 5; k++) {
        gap = a[k];
        for (i = gap; i < count; ++i) {
            x = items[i];
            for (j = i - gap; (x < items[j]) && (j >= 0); j = j - gap)
                items[j + gap] = items[j];
            items[j + gap] = x;
        }
    }
}

void qs(int* items, int left, int right) {
    
    if (left >= right) {
        return;
    }

    
    while (left < right) {
        int i = left;
        int j = right;
        int pivot = items[(left + right) / 2]; 

        
        while (i <= j) {
            while (items[i] < pivot) i++;
            while (items[j] > pivot) j--;
            if (i <= j) {
                int temp = items[i];
                items[i] = items[j];
                items[j] = temp;
                i++;
                j--;
            }
        }

       
        if (j - left < right - i) {
            qs(items, left, j);
            left = i;
        }
        else {
            qs(items, i, right);
            right = j;
        }
    }
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void generateRandom(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }
}

void generateAscending(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
}

void generateDescending(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}

void generateMixed(int* arr, int n) {
    for (int i = 0; i < n / 2; i++) {
        arr[i] = i;
    }
    for (int i = n / 2; i < n; i++) {
        arr[i] = n - i;
    }
}

int* copyArray(const int* source, int n) {
    int* copy = malloc(n * sizeof(int));
    if (copy == NULL) {
        printf("Ошибка выделения памяти!\n");
        exit(1);
    }
    memcpy(copy, source, n * sizeof(int));
    return copy;
}

double measureShellTime(int* arr, int n) {
    int* temp = copyArray(arr, n);
    clock_t start = clock();
    shell(temp, n);
    clock_t end = clock();
    free(temp);
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

double measureQsTime(int* arr, int n) {
    int* temp = copyArray(arr, n);
    clock_t start = clock();
    qs(temp, 0, n - 1);
    clock_t end = clock();
    free(temp);
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

double measureStdQsortTime(int* arr, int n) {
    int* temp = copyArray(arr, n);
    clock_t start = clock();
    qsort(temp, n, sizeof(int), compare);
    clock_t end = clock();
    free(temp);
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main() {
    
    clearScreen();

    const int n = 100000; 
    int* originalArray = malloc(n * sizeof(int));

    if (originalArray == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

  
    const char* arrayTypes[] = {
        "Случайный",
        "Возрастающий",
        "Убывающий",
        "Смешанный"
    };

  
    void (*generators[])(int*, int) = {
        generateRandom,
        generateAscending,
        generateDescending,
        generateMixed
    };

  
    FILE* file = fopen("rez2.txt", "w");
    if (file == NULL) {
        printf("Ошибка открытия файла!\n");
        free(originalArray);
        return 1;
    }

 
    fprintf(file, "Сортировка Шелла     Быстрая сортировка     Стандартная qsort\n");

    
    srand(time(NULL));

  
    for (int i = 0; i < 4; i++) {
        printf("Тестирование на массиве: %s\n", arrayTypes[i]);

  
        generators[i](originalArray, n);

        
        double timeShell = measureShellTime(originalArray, n);

       
        double timeQs = measureQsTime(originalArray, n);


        double timeStdQsort = measureStdQsortTime(originalArray, n);

   
        fprintf(file, "   %.6f\t\t%.6f\t\t%.6f\n",
            timeShell, timeQs, timeStdQsort);

        printf("Результаты: Shell: %.6f, QS: %.6f, Std: %.6f\n\n",
            timeShell, timeQs, timeStdQsort);
    }

  
    fclose(file);
    free(originalArray);

    printf("Результаты записаны в файл rez2.txt\n");

    return 0;
}

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}