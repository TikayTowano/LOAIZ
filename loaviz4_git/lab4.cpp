/*включить дубликаты как >=, поиск элементов и вывод уровня, где найдены эти элементы (корень - 0)*/
#include <iostream>
#include <limits>

struct Node {
    int data;
    int count;
    Node* left;
    Node* right;
};

int isInteger(const std::string& message);

Node* createTree(Node* root, int data) {
    if (root == nullptr) {
        Node* newNode = new Node;
        newNode->data = data;
        newNode->count = 1;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }

    if (data < root->data) {
        root->left = createTree(root->left, data);
    }
    else if (data > root->data) {
        root->right = createTree(root->right, data);
    }
    else {
        root->count++;
    }
    return root;
}

void printTree(Node* node, int level = 0) {
    if (node == nullptr) return;

    printTree(node->right, level + 1);
    for (int i = 0; i < level; i++) std::cout << "   ";
    std::cout << node->data << "(" << node->count << ")" << std::endl;
    printTree(node->left, level + 1);
}

bool searchTree(Node* root, int value) {
    if (root == nullptr) return false;
    if (root->data == value) return true;
    if (value < root->data)
        return searchTree(root->left, value);
    else
        return searchTree(root->right, value);
}

int countOccurrences(Node* root, int value) {
    if (root == nullptr) return 0;

    if (root->data == value) {
        return root->count;
    }

    if (value < root->data)
        return countOccurrences(root->left, value);
    else
        return countOccurrences(root->right, value);
}

void deleteTree(Node* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}
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
        value = inputInteger("Введите число: ");
        value = isInteger("Введите число: ");
        if (value == -1) {
            std::cout << "Построение дерева окончено\n\n";
            break;
        }
        root = createTree(root, value);
    }

    std::cout << "Дерево (число в скобках - количество вхождений):\n";
    printTree(root);

    std::cout << "\n--- Поиск значений в дереве ---\n";
    while (true) {
        value = isInteger("Введите значение для поиска (-1 для окончания): ");
        if (value == -1) {
            std::cout << "Поиск значений окончен\n";
            break;
        }
        if (searchTree(root, value))
            std::cout << "Значение " << value << " найдено в дереве.\n";
        else
            std::cout << "Значение " << value << " отсутствует в дереве.\n";
        std::cout << "Результаты поиска:\n";
        searchLevel(root, value);
    }

    while (true) {
        value = isInteger("Введите значение для подсчёта вхождений (-1 для окончания): ");
        value = isInteger("\nВведите значение для подсчёта вхождений (-1 для окончания): ");
        if (value == -1) {
            std::cout << "Подсчет вхождений окончен\n";
            std::cout << "Подсчёт вхождений окончен\n";
            break;
        }
        std::cout << "Количество вхождений элемента " << value << ": "
            @@ - 114, 6 + 57, 7 @@ int main() {
            return 0;
        }


        int isInteger(const std::string & message) {
            int value;
            while (true) {
                @@ - 132, 4 + 76, 67 @@ int isInteger(const std::string & message) {
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

        Node* createTree(Node * root, int data) {
            if (root == nullptr) {
                Node* newNode = new Node;
                newNode->data = data;
                newNode->left = nullptr;
                newNode->right = nullptr;
                return newNode;
            }


            if (data < root->data) {
                root->left = createTree(root->left, data);
            }
            else {
                root->right = createTree(root->right, data);
            }

            return root;
        }

        void printTree(Node * node, int level) {
            if (node == nullptr) return;
            printTree(node->right, level + 1);
            for (int i = 0; i < level; i++) std::cout << "   ";
            std::cout << node->data << std::endl;
            printTree(node->left, level + 1);
        }

        void searchLevel(Node * root, int value, int level) {
            if (root == nullptr) return;

            if (root->data == value)
                std::cout << "Значение " << value << " найдено на уровне " << level << std::endl;

            if (value < root->data)
                searchLevel(root->left, value, level + 1);
            else
                searchLevel(root->right, value, level + 1);
        }

        int countOccurrences(Node * root, int value) {
            if (root == nullptr) return 0;
            int count = 0;
            if (root->data == value) count++;
            count += countOccurrences(root->left, value);
            count += countOccurrences(root->right, value);
            return count;
        }

        void deleteTree(Node * root) {
            if (root == nullptr) return;
            deleteTree(root->left);
            deleteTree(root->right);
            delete root;
        }