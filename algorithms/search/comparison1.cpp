#include <chrono>
#include <cmath>
#include <iostream>
#include <list>
#include <vector>

using namespace std;
using namespace chrono;

// Пример простой хэш-функции для целых чисел
size_t hashFunc(int key, size_t tableSize) { return key % tableSize; }

class HashSet {
   private:
    vector<vector<int>> table;  // Хэш-таблица в виде вектора списков
    size_t size;  // Текущий размер множества

    size_t getBucketIndex(int key) const { return hashFunc(key, table.size()); }

   public:
    HashSet(size_t initialSize = 10) : size(0) { table.resize(initialSize); }

    // Вставка элемента в множество
    void insert(int key) {
        size_t bucketIndex = getBucketIndex(key);
        if (!contains(key)) {
            table[bucketIndex].push_back(key);
            ++size;
        }
    }

    // Проверка наличия элемента в множестве
    bool contains(int key) const {
        size_t bucketIndex = getBucketIndex(key);
        const auto& bucket = table[bucketIndex];
        for (int element : bucket) {
            if (element == key) {
                return true;
            }
        }
        return false;
    }

    // Поиск элемента в множестве
    vector<int>::const_iterator find(int key) const {
        size_t bucketIndex = getBucketIndex(key);
        auto& bucket = table[bucketIndex];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (*it == key) {
                return it;
            }
        }
        return bucket.end();
    }

    // Получение текущего размера множества
    size_t getSize() const { return size; }
};

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class BST {
   private:
    TreeNode* root;

    TreeNode* findRecursive(TreeNode* node, int key) {
        if (node == nullptr || node->val == key) {
            return node;
        }

        if (key < node->val) {
            return findRecursive(node->left, key);
        } else {
            return findRecursive(node->right, key);
        }
    }

   public:
    BST() : root(nullptr) {}

    void insert(int key) { root = insertRecursive(root, key); }

    TreeNode* insertRecursive(TreeNode* node, int key) {
        if (node == nullptr) {
            return new TreeNode(key);
        }

        if (key < node->val) {
            node->left = insertRecursive(node->left, key);
        } else if (key > node->val) {
            node->right = insertRecursive(node->right, key);
        }

        return node;
    }

    TreeNode* find(int key) { return findRecursive(root, key); }
};

void testHashSet(int dataSize) {
    HashSet hashSet;

    for (int i = 0; i < dataSize; ++i) {
        hashSet.insert(i);
    }

    auto start = high_resolution_clock::now();
    hashSet.find(dataSize - 1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Время hashSet для" << dataSize
         << " элементов: " << duration.count() << " микросекунд" << endl;
}

void testBinarySearchTree(int dataSize) {
    BST binaryTree;

    for (int i = 0; i < dataSize; ++i) {
        binaryTree.insert(i);
    }

    auto start = high_resolution_clock::now();
    binaryTree.find(dataSize - 1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Время binary tree для" << dataSize
         << " элементов: " << duration.count() << " микросекунд" << endl;
}

int main() {
    // Разные объемы данных для тестирования
    vector<int> dataSizes = {100, 1000, 10000, 50000};

    // Тестирование HashSet
    cout << "Testing HashSet:" << endl;
    for (int size : dataSizes) {
        testHashSet(size);
    }

    // Тестирование бинарного дерева
    cout << "\nTesting Binary Tree:" << endl;
    for (int size : dataSizes) {
        testBinarySearchTree(size);
    }

    return 0;
}
