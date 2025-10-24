/*включить дубликаты как >=, поиск элементов и вывод уровня, где найдены эти элементы (корень - 0)*/
#include <iostream>
#include <limits>

struct Node {
    int data;
    Node* left;
    Node* right;
};

int isInteger(const std::string& message);
void clearScreen();
Node* createTree(Node* root, int data);
void printTree(Node* node, int level = 0);
void searchLevel(Node* root, int value, int level = 0);
int countOccurrences(Node* root, int value);
void deleteTree(Node* root);

int main() {
    clearScreen();
    Node* root = nullptr;
    int value;
    std::cout << "-1 — окончание построения дерева\n";

    while (true) {
        value = isInteger("Введите число: ");
        if (value == -1) {
            std::cout << "Построение дерева окончено\n\n";
            break;
        }
        root = createTree(root, value);
    }

    printTree(root);

    while (true) {
        value = isInteger("Введите значение для поиска (-1 для окончания): ");
        if (value == -1) {
            std::cout << "Поиск значений окончен\n";
            break;
        }
        std::cout << "Результаты поиска:\n";
        searchLevel(root, value);
    }

    while (true) {
        value = isInteger("\nВведите значение для подсчёта вхождений (-1 для окончания): ");
        if (value == -1) {
            std::cout << "Подсчёт вхождений окончен\n";
            break;
        }
        std::cout << "Количество вхождений элемента " << value << ": " 
                  << countOccurrences(root, value) << std::endl;
    }

    deleteTree(root);
    return 0;
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

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

Node* createTree(Node* root, int data) {
    if (root == nullptr) {
        Node* newNode = new Node;
        newNode->data = data;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }


    if (data < root->data) {
        root->left = createTree(root->left, data);
    } else {
        root->right = createTree(root->right, data);
    }

    return root;
}

void printTree(Node* node, int level) {
    if (node == nullptr) return;
    printTree(node->right, level + 1);
    for (int i = 0; i < level; i++) std::cout << "   ";
    std::cout << node->data << std::endl;
    printTree(node->left, level + 1);
}

void searchLevel(Node* root, int value, int level) {
    if (root == nullptr) return;

    if (root->data == value)
        std::cout << "Значение " << value << " найдено на уровне " << level << std::endl;

    if (value < root->data)
        searchLevel(root->left, value, level + 1);
    else
        searchLevel(root->right, value, level + 1);
}

int countOccurrences(Node* root, int value) {
    if (root == nullptr) return 0;
    int count = 0;
    if (root->data == value) count++;
    count += countOccurrences(root->left, value);
    count += countOccurrences(root->right, value);
    return count;
}

void deleteTree(Node* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}