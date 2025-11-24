// обход в глубину, стек от руки, списки, в 1 код
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <locale>
#include <limits>
#include <iomanip>

struct StackNode {
    int value;
    StackNode* next;
};

StackNode* stackHead = nullptr;

struct Node {
    int inf;
    Node* next;
};

void pushStack(int v);
bool isEmptyStack();
int popStackC();

void dfs(int** G, int numG, int* visited, int v);
void dfsStack(int** G, int numG, int* visited, int v);
void dfsList(Node** adj, int v, int* visited);

void clearScreen();
int isInteger(const std::string& message);

int main() {
    setlocale(LC_ALL, "Rus");
    clearScreen();
    srand(time(NULL));

    int numG = isInteger("Введите количество вершин графа: ");
    while (numG <= 0) {
        std::cout << "Ошибка! Положительное число!\n";
        numG = isInteger("Введите количество вершин графа: ");
    }

    int** G = new int*[numG];
    for (int i = 0; i < numG; i++)
        G[i] = new int[numG];

    int* visited = new int[numG];

    for (int i = 0; i < numG; i++) {
        visited[i] = 0;
        for (int j = 0; j < numG; j++)
            G[i][j] = (i == j ? 0 : rand() % 2);
    }

    std::cout << "\nМатрица смежности:\n";
    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++)
            std::cout << std::setw(3) << G[i][j];
        std::cout << "\n";
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

    std::cout << "\nСписки смежности:\n";
    for (int i = 0; i < numG; i++) {
        std::cout << i << ": ";
        Node* cur = adj[i];
        while (cur) {
            std::cout << cur->inf << " ";
            cur = cur->next;
        }
        std::cout << "\n";
    }

    int start = isInteger("\nВведите вершину для начала обхода: ");

    for (int i = 0; i < numG; i++) visited[i] = 0;
    std::cout << "\nПуть (обычный DFS):\n";
    dfs(G, numG, visited, start);

    for (int i = 0; i < numG; i++) visited[i] = 0;
    stackHead = nullptr;
    std::cout << "\nПуть (DFS со стеком):\n";
    dfsStack(G, numG, visited, start);

    for (int i = 0; i < numG; i++) visited[i] = 0;
    std::cout << "\nПуть (DFS по спискам смежности):\n";
    dfsList(adj, start, visited);

    for (int i = 0; i < numG; i++) {
        Node* cur = adj[i];
        while (cur) {
            Node* t = cur;
            cur = cur->next;
            delete t;
        }
    }
    delete[] adj;
    delete[] visited;
    for (int i = 0; i < numG; i++)
        delete[] G[i];
    delete[] G;

    return 0;
}

void pushStack(int v) {
    StackNode* p = new StackNode{v, stackHead};
    stackHead = p;
}

bool isEmptyStack() {
    return stackHead == nullptr;
}

int popStack() {
    if (isEmptyStack()) return -1;
    StackNode* t = stackHead;
    int v = t->value;
    stackHead = stackHead->next;
    delete t;
    return v;
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
        return value;
    }
}

void dfs(int** G, int numG, int* visited, int v) {
    visited[v] = 1;
    std::cout << " " << v << "\n";
    for (int i = 0; i < numG; i++) {
        if (G[v][i] == 1 && visited[i] == 0)
            dfs(G, numG, visited, i);
    }
}

void dfsStack(int** G, int numG, int* visited, int v) {
    pushStack(v);
    while (!isEmptyStack()) {
        int cur = popStack();
        if (!visited[cur]) {
            visited[cur] = 1;
            std::cout << " " << cur << "\n";
            for (int i = numG - 1; i >= 0; i--) {
                if (G[cur][i] == 1 && visited[i] == 0)
                    pushStack(i);
            }
        }
    }
}

void dfsList(Node** adj, int v, int* visited) {
    visited[v] = 1;
    std::cout << " " << v << "\n";
    Node* cur = adj[v];
    while (cur) {
        if (!visited[cur->inf])
            dfsList(adj, cur->inf, visited);
        cur = cur->next;
    }
}