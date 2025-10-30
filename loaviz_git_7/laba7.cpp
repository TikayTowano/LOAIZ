// обход в глубину
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <locale>
#include <limits>
#include <iomanip>

void clearScreen();
int isInteger(const std::string& message);
void dfs(int** G, int numG, int* visited, int s);

int main() {
    setlocale(LC_ALL, "Rus");
    clearScreen();
    srand(time(NULL));
    int** G;
    int numG, current;
    int* visited;
    
    numG = isInteger("Введите количество вершин графа: ");
    while (numG <= 0) {
        std::cout << "Ошибка! Количество вершин должно быть положительным\n";
        numG = isInteger("Введите количество вершин графа: ");
    }

    G = (int**)malloc(sizeof(int*) * numG);
    visited = (int*)malloc(numG * sizeof(int));

    for (int i = 0; i < numG; i++) {
        G[i] = (int*)malloc(numG * sizeof(int));
    }

    for (int i = 0; i < numG; i++) {
        visited[i] = 0;
        for (int j = 0; j < numG; j++) {
            G[i][j] = G[j][i] = (i == j ? 0 : rand() % 2);
        }
    }

    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++) {
            std::cout << std::setw(3) << G[i][j];
        }
        std::cout << "\n";
    }

    current = isInteger("Введите вершину, с которой хотите начать обход графа: ");
    while (current < 0) {
        std::cout << "Ошибка! Вершина не может быть отрицательной\n";
        current = isInteger("Введите вершину, с которой хотите начать обход графа: ");
    }
    
    std::cout << "Путь: \n";

    dfs(G, numG, visited, current);

    free(visited);
    for (int i = 0; i < numG; i++){
        free(G[i]);
    }
    free(G);

    return 0;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int isInteger(const std::string& message) {
    int value;
    while (true) {
        std::cout << message;
        if (!(std::cin >> value)) {
            std::cout << "Ошибка: введено не число.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (std::cin.peek() != '\n') {
            std::cout << "Ошибка: введено не целое число.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        return value;
    }
}

void dfs(int** G, int numG, int* visited, int s) {
    visited[s] = 1;
    std::cout << std::setw(3) << s << "\n";

    for (int i = 0; i < numG; i++) {
        if (G[s][i] == 1 && visited[i] == 0) {
            dfs(G, numG, visited, i);
        }
    }
}
