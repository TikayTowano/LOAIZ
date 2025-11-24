// обход графа в ширину, доп со своей очередью списки смежности в 1 код
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

void enqueue(Node*& head, Node*& tail, int value);
int dequeue(Node*& head, Node*& tail);
bool isEmpty(Node* head);

void bfsOurQueue(int** G, int numG, int* visited, int start);
void bfsOurQueueSpisok(Node** adj, int v, int* visited);

void bfsSTL(int** G, int numG, int* visited, int s);
void bfsSTLSpisok(Node** adj, int v, int* visited);

void clearScreen();
int isInteger(const string& message);

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

    int* visited = new int[numG]{0};

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

    int current = isInteger("\nВведите вершину, с которой хотите начать обход графа: ");
    while (current < 0 || current >= numG) {
        cout << "Ошибка! Вершина должна быть в диапазоне [0," << numG-1 << "]\n";
        current = isInteger("Введите вершину: ");
    }

    clock_t t1 = clock();
    cout << "\nПуть (BFS с queue):\n";
    bfsSTL(G, numG, visited, current);
    clock_t t2 = clock();
    double timeSTL = double(t2 - t1) / CLOCKS_PER_SEC;; 

    for (int i = 0; i < numG; i++)
    visited[i] = 0;

    clock_t t3 = clock();
    cout << "\nПуть (BFS с queue списки смежности):\n";
    bfsOurQueueSpisok(adj, current, visited);
    clock_t t4 = clock();
    double timeSTLSpisok = double(t4 - t3) / CLOCKS_PER_SEC;

    for (int i = 0; i < numG; i++)
        visited[i] = 0;

    clock_t t5 = clock();
    cout << "\nПуть (BFS с queue собственная):\n";
    bfsOurQueue(G, numG, visited, current);
    clock_t t6 = clock();
    double timeOurQueue = double(t6 - t5) / CLOCKS_PER_SEC;

    for (int i = 0; i < numG; i++)
        visited[i] = 0;

    clock_t t7 = clock();
    cout << "\nПуть (BFS с queue собственная списки смежности):\n";
    bfsOurQueueSpisok(adj, current, visited);
    clock_t t8 = clock();
    double timeOurQueueSpisok = double(t8 - t7) / CLOCKS_PER_SEC;

    cout << "\nКоличество вершин графа: " << numG << "\n";
    cout << "Время выполнения queue на матрице смежности: " << fixed << setprecision(6) << timeSTL << " секунд\n";
    cout << "Время выполнения queue на списках смежности: " << fixed << setprecision(6) << timeSTLSpisok << " секунд\n";
    cout << "Время выполнения наша queue на матрице смежности: " << fixed << setprecision(6) << timeOurQueue << " секунд\n";
    cout << "Время выполнения наша queue на списках смежности: " << fixed << setprecision(6) << timeOurQueueSpisok << " секунд\n";

    delete[] visited;
    for (int i = 0; i < numG; i++)
        delete[] G[i];
    delete[] G;

    return 0;
}

void bfsSTL(int** G, int numG, int* visited, int s) {
    queue<int> q;
    visited[s] = 1;
    q.push(s);

    while(!q.empty()) {
        int v = q.front();
        q.pop();
        cout << setw(3) << v << "\n";

        for (int i = 0; i < numG; i++) {
            if (G[v][i] == 1 && visited[i] == 0) {
                q.push(i);
                visited[i] = 1;
            }
        }
    }
}

void bfsSTLSpisok(Node** adj, int start, int* visited) {
    queue<int> q;
    visited[start] = 1;
    q.push(start);

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        cout << setw(3) << v << "\n";

        Node* cur = adj[v];
        while (cur) {
            if (!visited[cur->inf]) {
                visited[cur->inf] = 1;
                q.push(cur->inf);
            }
            cur = cur->next;
        }
    }
}

void bfsOurQueueSpisok(Node** adj, int start, int* visited) {
    Node* head = nullptr;
    Node* tail = nullptr;

    visited[start] = 1;
    enqueue(head, tail, start);

    while (!isEmpty(head)) {
        int curV = dequeue(head, tail);
        cout << setw(3) << curV << "\n";

        Node* cur = adj[curV];
        while (cur) {
            if (!visited[cur->inf]) {
                visited[cur->inf] = 1;
                enqueue(head, tail, cur->inf);
            }
            cur = cur->next;
        }
    }
}

void bfsOurQueue(int** G, int numG, int* visited, int start) {
    Node* head = nullptr;
    Node* tail = nullptr;

    visited[start] = 1;
    enqueue(head, tail, start);

    while (!isEmpty(head)) {
        int v = dequeue(head, tail);
        cout << setw(3) << v << "\n";

        for (int i = 0; i < numG; i++) {
            if (G[v][i] == 1 && visited[i] == 0) {
                visited[i] = 1;
                enqueue(head, tail, i);
            }
        }
    }
}

void enqueue(Node*& head, Node*& tail, int value) {
    Node* p = new Node{value, nullptr};
    if (!head) {
        head = tail = p;
    } else {
        tail->next = p;
        tail = p;
    }
}

int dequeue(Node*& head, Node*& tail) {
    if (!head) return -1;
    Node* temp = head;
    int val = temp->inf;
    head = head->next;
    if (!head) tail = nullptr;
    delete temp;
    return val;
}

bool isEmpty(Node* head) {
    return head == nullptr;
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
