// обход в ширину (поиск расстояний)
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <locale>
#include <limits>
#include <iomanip>
#include <queue>

using namespace std;

void clearScreen();
int isInteger(const string& message);
void bfsd(int** G, int numG, int* distance, int s);

int main() {
    setlocale(LC_ALL, "Rus");
    clearScreen();
    srand(time(NULL));
    int** G;
    int numG, current;
    int* distance;
    
    numG = isInteger("Введите количество вершин графа: ");
    while (numG <= 0) {
        cout << "Ошибка! Количество вершин должно быть положительным\n";
        numG = isInteger("Введите количество вершин графа: ");
    }

    G = (int**)malloc(sizeof(int*) * numG);
    distance = (int*)malloc(numG * sizeof(int));

    for (int i = 0; i < numG; i++) {
        G[i] = (int*)malloc(numG * sizeof(int));
    }

    for (int i = 0; i < numG; i++) {
        distance[i] = -1;
        for (int j = i; j < numG; j++) {
            G[i][j] = G[j][i] = (i == j ? 0 : rand() % 2);
        }
    }

    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++) {
            cout << std::setw(3) << G[i][j];
        }
        cout << "\n";
    }

    current = isInteger("Введите вершину, с которой хотите начать обход графа: ");
    while (current < 0) {
        cout << "Ошибка! Вершина не может быть отрицательной\n";
        current = isInteger("Введите вершину, с которой хотите начать обход графа: ");
    }
    
    cout << "\nПуть: \n";

    bfsd(G, numG, distance, current);

    free(distance);
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

int isInteger(const string& message) {
    int value;
    while (true) {
        cout << message;
        if (!(cin >> value)) {
            cout << "Ошибка: введено не число.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (cin.peek() != '\n') {
            cout << "Ошибка: введено не целое число.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        return value;
    }
}

void bfsd(int** G, int numG, int* distance, int s) {
    queue<int> q;
    int v;

    distance[s] = 0;
    q.push(s);

    while(!q.empty()) {
        v = q.front();
        q.pop();
        cout << setw(3) << v << "\n";

        for (int i = 0; i < numG; i++) {
            if (G[v][i] == 1 && distance[i] == -1) {
                q.push(i);
                distance[i] = distance[v] + 1;
            }
        }
    }

    cout << "Расстояние от вершины " << s << " до вершины: \n";
    for (int i = 0; i < numG; i++){
       if (distance[i] == -1) {
        cout << setw(3) << i;
        cout << " : изолированная вершина\n";
    }
        else {
            cout << setw(3) << i << " : " << distance[i] << "\n";
        }
    }

}