#include <iostream>
#include <cstring>
#include <queue>
#include <climits>
#include <locale>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace std;

void clearScreen();
int isInteger(const string& message);
bool bfs(int** rGraph, int numG, int s, int t, int parent[]);
int fordFulkerson(int** graph, int numG, int s, int t);
void printMatrix(int** Matrix, int numG);
string intToString(int value);
bool askToRestart();
char safeCharInput(const string& message);
bool saveGraphToFile(int** graph, int numG, bool isOriented, const string& filename);
bool loadGraphFromFile(int**& graph, int& numG, bool& isOriented, const string& filename);
bool saveResultToFile(int source, int sink, int max_flow, int numG, bool isOriented, const string& filename);

int main() {
    setlocale(LC_ALL, "Rus");

    bool restartProgram = true;

    while (restartProgram) {
        clearScreen();
        srand(static_cast<unsigned int>(time(NULL)));

        cout << "==============================================||\n";
        cout << "||   РЕАЛИЗАЦИЯ АЛГОРИТМА ФОРДА-ФАЛКЕРСОНА    ||\n";
        cout << "||   Для поиска максимального потока в сети   ||\n";
        cout << "||==============================================\n\n";

        int** graph = nullptr;
        int numG = 0;
        bool isOriented = false;
        bool loadedFromFile = false;

        
        cout << "=== РЕЖИМ РАБОТЫ С ГРАФОМ ===\n";
        cout << "1 - Сгенерировать случайный граф\n";
        cout << "2 - Загрузить граф из файла\n";
        int modeChoice = isInteger("Выберите режим (1 или 2): ");
        while (modeChoice != 1 && modeChoice != 2) {
            cout << "Ошибка! Введите 1 или 2\n";
            modeChoice = isInteger("Выберите режим (1 или 2): ");
        }

        if (modeChoice == 2) {
            
            string filename;
            cout << "Введите имя файла для загрузки: ";
            cin >> filename;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            graph = nullptr;
            if (loadGraphFromFile(graph, numG, isOriented, filename)) {
                cout << "Граф успешно загружен из файла '" << filename << "'\n";
                loadedFromFile = true;
                cout << "Количество вершин: " << numG << "\n";
                cout << "Тип графа: " << (isOriented ? "ориентированный" : "неориентированный") << "\n\n";
            }
            else {
                cout << "Ошибка загрузки файла. Будет сгенерирован случайный граф.\n";
                loadedFromFile = false;
                modeChoice = 1;
            }
        }

        if (modeChoice == 1) {
            
            numG = isInteger("Введите количество вершин графа (рекомендуется 4-10): ");

            while (numG <= 0) {
                cout << "Ошибка! Количество вершин должно быть положительным\n";
                numG = isInteger("Введите количество вершин графа: ");
            }
            while (numG == 1) {
                cout << "Ошибка! Для построения сети необходимо более 1 вершины\n";
                numG = isInteger("Введите количество вершин графа: ");
            }

            if (!loadedFromFile) {
                cout << "\nТип графа:\n";
                cout << "1 — Ориентированный\n";
                cout << "2 — Неориентированный\n";
                int orientChoice = isInteger("Выберите тип (1 или 2): ");
                while (orientChoice != 1 && orientChoice != 2) {
                    cout << "Ошибка! Введите 1 или 2\n";
                    orientChoice = isInteger("Выберите тип (1 или 2): ");
                }
                isOriented = (orientChoice == 1);
            }

            
            graph = new int* [numG];
            for (int i = 0; i < numG; i++) {
                graph[i] = new int[numG];
            }

            cout << "\n=== ГЕНЕРАЦИЯ СЛУЧАЙНОЙ СЕТИ ===\n\n";
            cout << "Параметры генерации:\n";
            cout << "- Вероятность наличия ребра: 70%\n";
            cout << "- Пропускная способность: 1-100\n";
            cout << "- Петли отсутствуют\n\n";

            
            for (int i = 0; i < numG; i++) {
                for (int j = 0; j < numG; j++) {
                    if (i == j) {
                        graph[i][j] = 0;
                    }
                    else {
                        int hasEdge = (rand() % 100) < 70;
                        if (hasEdge) {
                            graph[i][j] = rand() % 100 + 1;
                        }
                        else {
                            graph[i][j] = 0;
                        }

                        if (!isOriented && i < j) {
                            graph[j][i] = graph[i][j];
                        }
                    }
                }
            }

            if (isOriented) {
                for (int i = 0; i < numG; i++) {
                    for (int j = i + 1; j < numG; j++) {
                        if (graph[i][j] > 0 && graph[j][i] > 0 && (rand() % 100) < 50) {
                            if (rand() % 2 == 0) {
                                graph[i][j] = 0;
                            }
                            else {
                                graph[j][i] = 0;
                            }
                        }
                    }
                }
            }
        }

        cout << "Матрица пропускных способностей:\n";
        printMatrix(graph, numG);

        
        if (modeChoice == 1) {
            cout << "\nХотите сохранить граф в файл перед расчетом? (y/n): ";
            char saveBeforeChoice;
            cin >> saveBeforeChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (saveBeforeChoice == 'y' || saveBeforeChoice == 'Y') {
                string filename;
                cout << "Введите имя файла для сохранения: ";
                getline(cin, filename);
                if (saveGraphToFile(graph, numG, isOriented, filename)) {
                    cout << "Граф успешно сохранен в файл '" << filename << "'\n";
                }
                else {
                    cout << "Ошибка сохранения файла.\n";
                }
            }
        }

        int source, sink;

        cout << "\n=== ВЫБОР ИСТОКА И СТОКА ===\n";

        
        source = 0;
        sink = numG - 1;

        bool sourceHasOutgoing = false;
        bool sinkHasIncoming = false;

        for (int j = 0; j < numG; j++) {
            if (graph[source][j] > 0) sourceHasOutgoing = true;
            if (graph[j][sink] > 0) sinkHasIncoming = true;
        }

        
        if (!sourceHasOutgoing || !sinkHasIncoming) {
            for (int i = 0; i < numG; i++) {
                for (int j = 0; j < numG; j++) {
                    if (graph[i][j] > 0) {
                        source = i;
                        break;
                    }
                }
                if (source != 0) break;
            }

            for (int i = numG - 1; i >= 0; i--) {
                for (int j = 0; j < numG; j++) {
                    if (graph[j][i] > 0) {
                        sink = static_cast<int>(i);
                        break;
                    }
                }
                if (sink != numG - 1) break;
            }
        }

        cout << "Автоматически выбрано:\n";
        cout << "Исток (source): вершина " << source << endl;
        cout << "Сток (sink): вершина " << sink << endl;

        
        cout << "\nХотите изменить выбор? (y/n): ";
        char changeChoice = safeCharInput("");

        if (changeChoice == 'y' || changeChoice == 'Y') {
            string prompt = "\nВведите номер вершины-истока (0-" + intToString(numG - 1) + "): ";
            source = isInteger(prompt);
            while (source < 0 || source >= numG) {
                cout << "Ошибка! Вершина должна быть от 0 до " << numG - 1 << "\n";
                source = isInteger("Введите номер вершины-истока: ");
            }

            sink = isInteger("Введите номер вершины-стока (0-" + intToString(numG - 1) + "): ");
            while (sink < 0 || sink >= numG || sink == source) {
                if (sink == source) {
                    cout << "Ошибка! Сток не может совпадать с истоком.\n";
                }
                else {
                    cout << "Ошибка! Вершина должна быть от 0 до " << numG - 1 << "\n";
                }
                sink = isInteger("Введите номер вершины-стока: ");
            }
        }

        cout << "\n=== ВЫЧИСЛЕНИЕ МАКСИМАЛЬНОГО ПОТОКА ===\n";
        cout << "Исток: " << source << ", Сток: " << sink << "\n";
        cout << "Количество вершин: " << numG << "\n";
        cout << "Тип графа: " << (isOriented ? "ориентированный" : "неориентированный") << "\n\n";

        int max_flow = fordFulkerson(graph, numG, source, sink);

        cout << "\n=================================================\n";
        cout << "Максимальный поток из вершины " << source
            << " в вершину " << sink << " равен: " << max_flow << endl;
        cout << "=================================================\n";

        
        cout << "\n=== АНАЛИЗ ГРАФА ===\n";

        int edgeCount = 0;
        int totalCapacity = 0;
        for (int i = 0; i < numG; i++) {
            for (int j = 0; j < numG; j++) {
                if (graph[i][j] > 0) {
                    edgeCount++;
                    totalCapacity += graph[i][j];
                }
            }
        }

        cout << "Количество вершин: " << numG << endl;
        cout << "Количество рёбер: " << edgeCount << endl;
        cout << "Суммарная пропускная способность всех рёбер: " << totalCapacity << endl;
        if (edgeCount > 0) {
            double avgCapacity = static_cast<double>(totalCapacity) / static_cast<double>(edgeCount);
            cout << "Средняя пропускная способность ребра: " << avgCapacity << endl;
        }

        
        cout << "\n=== СОХРАНЕНИЕ РЕЗУЛЬТАТОВ ===\n";

        
        cout << "Хотите сохранить граф в файл? (y/n): ";
        char saveGraphChoice;
        cin >> saveGraphChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (saveGraphChoice == 'y' || saveGraphChoice == 'Y') {
            string filename;
            cout << "Введите имя файла для сохранения графа: ";
            getline(cin, filename);
            if (saveGraphToFile(graph, numG, isOriented, filename)) {
                cout << "Граф успешно сохранен в файл '" << filename << "'\n";
            }
            else {
                cout << "Ошибка сохранения графа.\n";
            }
        }

        
        cout << "\nХотите сохранить результаты расчета? (y/n): ";
        char saveResultChoice;
        cin >> saveResultChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (saveResultChoice == 'y' || saveResultChoice == 'Y') {
            string filename;
            cout << "Введите имя файла для сохранения результатов: ";
            getline(cin, filename);
            if (saveResultToFile(source, sink, max_flow, numG, isOriented, filename)) {
                cout << "Результаты успешно сохранены в файл '" << filename << "'\n";
            }
            else {
                cout << "Ошибка сохранения результатов.\n";
            }
        }

        
        for (int i = 0; i < numG; i++) {
            delete[] graph[i];
        }
        delete[] graph;

        restartProgram = askToRestart();
    }

    cout << "\nПрограмма завершена. До свидания!\n";
    return 0;
}

bool saveGraphToFile(int** graph, int numG, bool isOriented, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << "# Формат файла графа для алгоритма Форда-Фалкерсона\n";
    file << "# Количество вершин: " << numG << endl;
    file << "# Тип графа: " << (isOriented ? "ориентированный" : "неориентированный") << endl;
    file << "# Матрица пропускных способностей:\n";

    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++) {
            file << graph[i][j];
            if (j < numG - 1) file << " ";
        }
        file << endl;
    }

    file.close();
    return true;
}

bool loadGraphFromFile(int**& graph, int& numG, bool& isOriented, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    string line;
    vector<vector<int>> matrix;
    numG = 0;
    isOriented = true;

    
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            if (line.find("Тип графа:") != string::npos) {
                if (line.find("неориентированный") != string::npos) {
                    isOriented = false;
                }
            }
            continue;
        }

        stringstream ss(line);
        vector<int> row;
        int value;

        while (ss >> value) {
            row.push_back(value);
        }

        if (!row.empty()) {
            matrix.push_back(row);
            if (numG == 0) {
                numG = static_cast<int>(row.size());
            }
        }
    }

    file.close();

    if (static_cast<int>(matrix.size()) != numG) {
        return false;
    }

    graph = new int* [numG];
    for (int i = 0; i < numG; i++) {
        graph[i] = new int[numG];
        for (int j = 0; j < numG; j++) {
            graph[i][j] = matrix[i][j];
        }
    }

    return true;
}

bool saveResultToFile(int source, int sink, int max_flow, int numG, bool isOriented, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << "==============================================\n";
    file << "РЕЗУЛЬТАТЫ РАСЧЕТА МАКСИМАЛЬНОГО ПОТОКА\n";
    file << "Алгоритм Форда-Фалкерсона\n";
    file << "==============================================\n\n";

    file << "ПАРАМЕТРЫ ГРАФА:\n";
    file << "Количество вершин: " << numG << endl;
    file << "Тип графа: " << (isOriented ? "ориентированный" : "неориентированный") << endl;
    file << "Исток (source): вершина " << source << endl;
    file << "Сток (sink): вершина " << sink << endl;

    file << "\nРЕЗУЛЬТАТ РАСЧЕТА:\n";
    file << "Максимальный поток: " << max_flow << endl;

    file << "\n==============================================\n";
    file << "Дата расчета: " << __DATE__ << " " << __TIME__ << endl;

    file.close();
    return true;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

char safeCharInput(const string& message) {
    char choice;
    cout << message;

    cin >> choice;

    
    while (cin.get() != '\n') {
        continue;
    }

    return choice;
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

string intToString(int value) {
    stringstream ss;
    ss << value;
    return ss.str();
}

bool bfs(int** rGraph, int numG, int s, int t, int parent[]) {
    bool* visited = new bool[numG];
    memset(visited, 0, sizeof(bool) * static_cast<size_t>(numG));

    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < numG; v++) {
            if (!visited[v] && rGraph[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    bool result = visited[t];
    delete[] visited;
    return result;
}

int fordFulkerson(int** graph, int numG, int s, int t) {
    int u, v;

    
    int** rGraph = new int* [numG];
    for (int i = 0; i < numG; i++) {
        rGraph[i] = new int[numG];
        for (int j = 0; j < numG; j++) {
            rGraph[i][j] = graph[i][j];
        }
    }

    int* parent = new int[numG];
    int max_flow = 0;
    int path_flow;
    int iteration = 1;

    cout << "Поиск максимального потока...\n";
    cout << "----------------------------------------\n";

    
    while (bfs(rGraph, numG, s, t, parent)) {
        
        path_flow = INT_MAX;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = (path_flow < rGraph[u][v]) ? path_flow : rGraph[u][v];
        }

        
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }

        
        max_flow += path_flow;

        
        cout << "Итерация " << iteration++ << ":\n";
        cout << "  Путь: ";
        vector<int> path;
        for (v = t; v != -1; v = parent[v]) {
            path.push_back(v);
        }
        for (int i = static_cast<int>(path.size()) - 1; i >= 0; i--) {
            cout << path[i];
            if (i > 0) cout << " -> ";
        }
        cout << "\n  Поток на пути: " << path_flow << endl;
        cout << "  Текущий поток: " << max_flow << endl;
        cout << "----------------------------------------\n";
    }

    cout << "\nАлгоритм завершен. Увеличивающих путей больше нет.\n";

    
    for (int i = 0; i < numG; i++) {
        delete[] rGraph[i];
    }
    delete[] rGraph;
    delete[] parent;

    return max_flow;
}

void printMatrix(int** Matrix, int numG) {
    
    cout << "    ";
    for (int j = 0; j < numG; j++) {
        cout << setw(3) << "[" << j << "]";
    }
    cout << "\n";

    
    for (int i = 0; i < numG; i++) {
        cout << "[" << setw(1) << i << "] ";
        for (int j = 0; j < numG; j++) {
            if (Matrix[i][j] == 0) {
                cout << setw(5) << " 0 ";
            }
            else {
                cout << setw(5) << Matrix[i][j];
            }
        }
        cout << "\n";
    }
}

bool askToRestart() {
    cout << "\n==================================================\n";
    cout << "Хотите начать заново? (1 - да, 0 - нет): ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return (choice == 1);
}