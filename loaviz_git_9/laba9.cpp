// для матрицы сформировать матрицу расстояний в обоих методах и сравнить время формирования этих матриц
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <locale>
#include <limits>
#include <iomanip>
#include <queue>

using namespace std;

struct Node {
    int inf;
    Node* next;
};

void clearScreen();
int isInteger(const string& message);

void bfsd(int** G, int numG, int* distance, int s);
void bfsdSpisok(Node** adj, int numG, int* distance, int s);

void dfsd(int** G, int numG, int* distance, int* visited, int v);
void dfsdSpisok(Node** adj, int numG, int* distance, int* visited, int v);

void bfsdD(int** G, int numG, int** GD);
void dfsdHelper(int** G, int numG, int* distance, int* visited, int v);
void dfsdD(int** G, int numG, int** GD);

int main() {
    setlocale(LC_ALL, "Rus");
    clearScreen();
    srand(time(NULL));

    int numG = isInteger("Введите количество вершин графа: ");
    while (numG <= 0) {
        cout << "Ошибка! Количество вершин должно быть положительным\n";
        numG = isInteger("Введите количество вершин графа: ");
    }

    int** G = new int*[numG];
    for (int i = 0; i < numG; i++)
        G[i] = new int[numG];

    for (int i = 0; i < numG; i++) {
        for (int j = i; j < numG; j++) {
            G[i][j] = G[j][i] = (i == j ? 0 : rand() % 2);
        }
    }

    cout << "\nМатрица смежности:\n";
    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++)
            cout << setw(3) << G[i][j];
        cout << "\n";
    }

    Node** adj = new Node*[numG];
    for (int i = 0; i < numG; i++)
        adj[i] = nullptr;

    for (int i = 0; i < numG; i++) {
        for (int j = numG - 1; j >= 0; j--) {
            if (G[i][j] == 1) {
                Node* p = new Node{ j, adj[i] };
                adj[i] = p;
            }
        }
    }

    cout << "\nСписки смежности:\n";
    for (int i = 0; i < numG; i++) {
        cout << i << ": ";
        Node* cur = adj[i];
        while (cur) {
            cout << cur->inf << " ";
            cur = cur->next;
        }
        cout << "\n";
    }

    int** GD_bfs = new int*[numG];
    int** GD_dfs = new int*[numG];
    for (int i = 0; i < numG; i++) {
        GD_bfs[i] = new int[numG];
        GD_dfs[i] = new int[numG];
    }

    clock_t t_bfsD1, t_bfsD2, t_dfsD1, t_dfsD2;

    t_bfsD1 = clock();
    bfsdD(G, numG, GD_bfs);
    t_bfsD2 = clock();
    double timeBfsdD = double(t_bfsD2 - t_bfsD1) / CLOCKS_PER_SEC;

    t_dfsD1 = clock();
    dfsdD(G, numG, GD_dfs);
    t_dfsD2 = clock();
    double timeDfsdD = double(t_dfsD2 - t_dfsD1) / CLOCKS_PER_SEC;

    cout << "\nМатрица расстояний (bfsdD — BFS от всех вершин):\n";
    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++) {
            if (GD_bfs[i][j] == -1)
                cout << setw(4) << "INF";
            else
                cout << setw(4) << GD_bfs[i][j];
        }
        cout << "\n";
    }

    cout << "\nМатрица расстояний (dfsdD — DFS от всех вершин):\n";
    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++) {
            if (GD_dfs[i][j] == -1)
                cout << setw(4) << "INF";
            else
                cout << setw(4) << GD_dfs[i][j];
        }
        cout << "\n";
    }

    cout << "\nВремя построения полной матрицы расстояний:\n";
    cout << "BFS (матрица смежности): " << fixed << setprecision(6) << timeBfsdD << " сек\n";
    cout << "DFS (матрица смежности): " << fixed << setprecision(6) << timeDfsdD << " сек\n";

    int start = isInteger("\nВведите стартовую вершину: ");
    while (start < 0 || start >= numG) {
        cout << "Ошибка! Вершина должна быть в диапазоне [0," << numG-1 << "]\n";
        start = isInteger("Введите вершину: ");
    }

    int* distance = new int[numG];
    int* visited = new int[numG];

    clock_t t1, t2, t3, t4, t5, t6, t7, t8;

    for (int i = 0; i < numG; i++) 
        distance[i] = -1;

    t1 = clock();
    cout << "\nПуть (BFS матрица):\n";
    bfsd(G, numG, distance, start);
    t2 = clock();
    double timeBfsd = double(t2 - t1) / CLOCKS_PER_SEC;

    for (int i = 0; i < numG; i++) 
        distance[i] = -1;
    t3 = clock();
    cout << "\nПуть (BFS списки):\n";
    bfsdSpisok(adj, numG, distance, start);
    t4 = clock();
    double timeBfsdSpisok = double(t4 - t3) / CLOCKS_PER_SEC;

    for (int i = 0; i < numG; i++) { 
        distance[i] = -1; 
        visited[i] = 0; 
    }
    distance[start] = 0;
    t5 = clock();
    cout << "\nПуть (DFS матрица):\n";
    cout << setw(3) << start << "\n";
    dfsd(G, numG, distance, visited, start);
    t6 = clock();
    cout << "Расстояние от вершины " << start << " до вершин:\n";
    for (int i = 0; i < numG; i++){
        if (distance[i] == -1) cout << setw(3) << i << " : изолированная вершина\n";
        else cout << setw(3) << i << " : " << distance[i] << "\n";
    }
    double timeDfsd = double(t6 - t5) / CLOCKS_PER_SEC;

    for (int i = 0; i < numG; i++) {
        distance[i] = -1;
        visited[i] = 0; 
    }
    distance[start] = 0;
    t7 = clock();
    cout << "\nПуть (DFS списки):\n";
    cout << setw(3) << start << "\n";
    dfsdSpisok(adj, numG, distance, visited, start);
    t8 = clock();

    cout << "Расстояние от вершины " << start << " до вершин:\n";
    for (int i = 0; i < numG; i++){
        if (distance[i] == -1) cout << setw(3) << i << " : изолированная вершина\n";
        else cout << setw(3) << i << " : " << distance[i] << "\n";
    }
    double timeDfsdSpisok = double(t8 - t7) / CLOCKS_PER_SEC;

    cout << "\nКоличество вершин графа: " << numG << "\n";
    cout << "Время выполнения bfsd на матрице смежности: " << fixed << setprecision(6) << timeBfsd << " секунд\n";
    cout << "Время выполнения bfsd на списках смежности: " << fixed << setprecision(6) << timeBfsdSpisok << " секунд\n";
    cout << "Время выполнения dfsd на матрице смежности: " << fixed << setprecision(6) << timeDfsd << " секунд\n";
    cout << "Время выполнения dfsd на списках смежности: " << fixed << setprecision(6) << timeDfsdSpisok << " секунд\n";

    delete[] distance;
    delete[] visited;

    for (int i = 0; i < numG; i++) {
        Node* cur = adj[i];
        while (cur) {
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        delete[] G[i];
        delete[] GD_bfs[i];
        delete[] GD_dfs[i];
    }
    delete[] G;
    delete[] adj;
    delete[] GD_bfs;
    delete[] GD_dfs;

    return 0;
}

void bfsd(int** G, int numG, int* distance, int s) {
    queue<int> q;
    distance[s] = 0;
    q.push(s);

    while(!q.empty()) {
        int v = q.front();
        q.pop();
        cout << setw(3) << v << "\n";

        for (int i = 0; i < numG; i++) {
            if (G[v][i] == 1 && distance[i] == -1) {
                q.push(i);
                distance[i] = distance[v] + 1;
            }
        }
    }

    cout << "Расстояние от вершины " << s << " до вершины:\n";
    for (int i = 0; i < numG; i++) {
        if (distance[i] == -1) cout << setw(3) << i << " : изолированная вершина\n";
        else cout << setw(3) << i << " : " << distance[i] << "\n";
    }
}

void bfsdSpisok(Node** adj, int numG, int* distance, int s) {
    queue<int> q;
    distance[s] = 0;
    q.push(s);

    while(!q.empty()) {
        int v = q.front();
        q.pop();
        cout << setw(3) << v << "\n";

        Node* cur = adj[v];
        while(cur) {
            if (distance[cur->inf] == -1) {
                distance[cur->inf] = distance[v] + 1;
                q.push(cur->inf);
            }
            cur = cur->next;
        }
    }

    cout << "Расстояние от вершины " << s << " до вершины:\n";
    for (int i = 0; i < numG; i++) {
        if (distance[i] == -1) cout << setw(3) << i << " : изолированная вершина\n";
        else cout << setw(3) << i << " : " << distance[i] << "\n";
    }
}

void dfsd(int** G, int numG, int* distance, int* visited, int v) {
    visited[v] = 1;
    for (int i = 0; i < numG; i++) {
        if (G[v][i] == 1 && !visited[i]) {
            distance[i] = distance[v] + 1;
            cout << setw(3) << i << "\n";
            dfsd(G, numG, distance, visited, i);
        }
    }
}

void dfsdSpisok(Node** adj, int numG, int* distance, int* visited, int v) {
    visited[v] = 1;
    Node* cur = adj[v];
    while(cur) {
        int to = cur->inf;
        if (!visited[to]) {
            distance[to] = distance[v] + 1;
            cout << setw(3) << to << "\n";
            dfsdSpisok(adj, numG, distance, visited, to);
        }
        cur = cur->next;
    }
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

void bfsdD(int** G, int numG, int** GD) {
    int* distance = new int[numG];
    queue<int> q;

    for (int s = 0; s < numG; s++) {
        for (int i = 0; i < numG; i++) {
            distance[i] = -1;
        }

        distance[s] = 0;
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int i = 0; i < numG; i++) {
                if (G[v][i] == 1 && distance[i] == -1) {
                    distance[i] = distance[v] + 1;
                    q.push(i);
                }
            }
        }

        for (int i = 0; i < numG; i++) {
            GD[s][i] = distance[i];
        }
    }

    delete[] distance;
}

void dfsdHelper(int** G, int numG, int* distance, int* visited, int v) {
    for (int i = 0; i < numG; i++) {
        if (G[v][i] == 1 && !visited[i]) {
            visited[i] = 1;
            distance[i] = distance[v] + 1;
            dfsdHelper(G, numG, distance, visited, i);
        }
    }
}

void dfsdD(int** G, int numG, int** GD) {
    int* distance = new int[numG];
    int* visited = new int[numG];

    for (int s = 0; s < numG; s++) {
        for (int i = 0; i < numG; i++) {
            distance[i] = -1;
            visited[i] = 0;
        }

        distance[s] = 0;
        visited[s] = 1;
        dfsdHelper(G, numG, distance, visited, s);

        for (int i = 0; i < numG; i++) {
            GD[s][i] = distance[i];
        }
    }

    delete[] distance;
    delete[] visited;
}