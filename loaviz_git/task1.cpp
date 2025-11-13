#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <string>
#include <locale>
#include <vector>
#include <map>
#include <set>

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

// Структура для представления графа через список смежности
struct Graph {
    std::vector<std::set<int>> adjacencyList;
    std::vector<std::string> vertexNames;

    Graph(int n) {
        adjacencyList.resize(n);
        vertexNames.resize(n);
        for (int i = 0; i < n; ++i) {
            vertexNames[i] = std::to_string(i);
        }
    }

    // Добавление ребра (неориентированного)
    void addEdge(int u, int v) {
        if (u >= 0 && u < adjacencyList.size() && v >= 0 && v < adjacencyList.size() && u != v) {
            adjacencyList[u].insert(v);
            adjacencyList[v].insert(u);
        }
    }

    // Удаление ребра
    void removeEdge(int u, int v) {
        if (u >= 0 && u < adjacencyList.size() && v >= 0 && v < adjacencyList.size()) {
            adjacencyList[u].erase(v);
            adjacencyList[v].erase(u);
        }
    }

    // Проверка наличия ребра
    bool hasEdge(int u, int v) const {
        if (u >= 0 && u < adjacencyList.size() && v >= 0 && v < adjacencyList.size()) {
            return adjacencyList[u].count(v) > 0;
        }
        return false;
    }

    // Получение размера графа
    int size() const {
        return adjacencyList.size();
    }

    // Копирование графа
    Graph copy() const {
        Graph newGraph(this->size());
        newGraph.adjacencyList = this->adjacencyList;
        newGraph.vertexNames = this->vertexNames;
        return newGraph;
    }
};

// Вывод списка смежности
void printAdjacencyList(const Graph& graph) {
    std::cout << "\nСписки смежности:\n";
    for (int i = 0; i < graph.size(); ++i) {
        std::cout << graph.vertexNames[i] << ": ";
        bool first = true;
        for (int neighbor : graph.adjacencyList[i]) {
            if (!first) std::cout << ", ";
            std::cout << graph.vertexNames[neighbor];
            first = false;
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

// Генерация случайного неориентированного графа
Graph generateGraph(int n) {
    Graph graph(n);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (rand() % 2 == 1) {
                graph.addEdge(i, j);
            }
        }
    }
    return graph;
}

// Отождествление вершин u и v
Graph mergeVertices(const Graph& graph, int u, int v) {
    if (u == v || u < 0 || v < 0 || u >= graph.size() || v >= graph.size()) {
        return graph.copy();
    }

    int newSize = graph.size() - 1;
    Graph newGraph(newSize);

    // Создаем отображение старых индексов на новые
    std::vector<int> indexMap(graph.size());
    int newIndex = 0;
    for (int i = 0; i < graph.size(); ++i) {
        if (i != v) {
            indexMap[i] = newIndex;
            newGraph.vertexNames[newIndex] = (i == u) ?
                graph.vertexNames[u] + "+" + graph.vertexNames[v] :
                graph.vertexNames[i];
            newIndex++;
        }
    }

    // Заполняем новый список смежности
    for (int i = 0; i < graph.size(); ++i) {
        if (i == v) continue; // Пропускаем вершину v

        int currentNewIndex = indexMap[i];

        // Объединяем соседей вершин u и v для новой вершины u
        if (i == u) {
            std::set<int> mergedNeighbors;
            // Добавляем всех соседей u и v (кроме самих u и v)
            for (int neighbor : graph.adjacencyList[u]) {
                if (neighbor != u && neighbor != v) {
                    mergedNeighbors.insert(indexMap[neighbor]);
                }
            }
            for (int neighbor : graph.adjacencyList[v]) {
                if (neighbor != u && neighbor != v) {
                    mergedNeighbors.insert(indexMap[neighbor]);
                }
            }
            newGraph.adjacencyList[currentNewIndex] = mergedNeighbors;
        }
        else {
            // Для остальных вершин просто переносим соседей с учетом нового индекса
            std::set<int> newNeighbors;
            for (int neighbor : graph.adjacencyList[i]) {
                if (neighbor == v) {
                    // Заменяем v на новую объединенную вершину
                    newNeighbors.insert(indexMap[u]);
                }
                else if (neighbor != u && neighbor != v) {
                    newNeighbors.insert(indexMap[neighbor]);
                }
            }
            newGraph.adjacencyList[currentNewIndex] = newNeighbors;
        }
    }

    return newGraph;
}

// Стягивание ребра u-v
Graph contractEdge(const Graph& graph, int u, int v) {
    if (!graph.hasEdge(u, v)) {
        return graph.copy();
    }
    return mergeVertices(graph, u, v);
}

// Расщепление вершины v
Graph splitVertex(const Graph& graph, int v) {
    if (v < 0 || v >= graph.size()) {
        return graph.copy();
    }

    int newSize = graph.size() + 1;
    Graph newGraph(newSize);

    // Копируем имена вершин
    for (int i = 0; i < graph.size(); ++i) {
        newGraph.vertexNames[i] = graph.vertexNames[i];
    }
    newGraph.vertexNames[newSize - 1] = graph.vertexNames[v] + "_split";

    // Копируем список смежности для всех вершин кроме v
    for (int i = 0; i < graph.size(); ++i) {
        if (i != v) {
            newGraph.adjacencyList[i] = graph.adjacencyList[i];
        }
    }

    // Распределяем связи вершины v между старой и новой вершиной
    std::set<int> oldVNeighbors;
    std::set<int> newVNeighbors;

    for (int neighbor : graph.adjacencyList[v]) {
        if (neighbor != v) { // Игнорируем петли
            if (neighbor % 2 == 0) {
                newVNeighbors.insert(neighbor);
            }
            else {
                oldVNeighbors.insert(neighbor);
            }
        }
    }

    // Обновляем соседей для старой вершины v
    newGraph.adjacencyList[v] = oldVNeighbors;

    // Устанавливаем соседей для новой вершины
    newGraph.adjacencyList[newSize - 1] = newVNeighbors;

    // Соединяем старую и новую вершины
    newGraph.addEdge(v, newSize - 1);

    // Обновляем соседей других вершин
    for (int neighbor : newVNeighbors) {
        newGraph.adjacencyList[neighbor].erase(v);
        newGraph.adjacencyList[neighbor].insert(newSize - 1);
    }

    return newGraph;
}

int main() {
    setlocale(LC_ALL, "RUS");
    srand(time(NULL));
    clearScreen();

    int n = isInteger("Введите количество вершин графа: ");
    while (n < 3) n = isInteger("Минимум 3. Повторите: ");

    Graph graph = generateGraph(n);
    std::cout << "Исходный граф:";
    printAdjacencyList(graph);

    Graph result = graph.copy();

    // === Отождествление вершин ===
    std::cout << "\n=== Отождествление вершин ===\n";
    int u, v;
    while (true) {
        u = isInteger("Введите вершину u для отождествления: ");
        v = isInteger("Введите вершину v для отождествления: ");
        if (u >= 0 && u < n && v >= 0 && v < n && u != v) break;
        std::cout << "Ошибка: неверные вершины, повторите.\n";
    }

    result = mergeVertices(result, u, v);
    std::cout << "После отождествления вершин " << u << " и " << v << ":";
    printAdjacencyList(result);

    // === Стягивание ребра ===
    std::cout << "\n=== Стягивание ребра ===\n";
    while (true) {
        u = isInteger("Введите вершину u: ");
        v = isInteger("Введите вершину v: ");
        if (u >= 0 && u < result.size() && v >= 0 && v < result.size() && u != v && result.hasEdge(u, v)) break;
        std::cout << "Ошибка: вершины не смежны или неверны.\n";
    }

    result = contractEdge(result, u, v);
    std::cout << "После стягивания ребра " << u << "-" << v << ":";
    printAdjacencyList(result);

    // === Расщепление вершины ===
    std::cout << "\n=== Расщепление вершины ===\n";
    while (true) {
        int w = isInteger("Введите вершину для расщепления: ");
        if (w >= 0 && w < result.size()) {
            result = splitVertex(result, w);
            std::cout << "После расщепления вершины " << w << ":";
            printAdjacencyList(result);
            break;
        }
        else {
            std::cout << "Ошибка: неверная вершина.\n";
        }
    }

    std::cout << "\nРабота завершена.\n";
    return 0;
}