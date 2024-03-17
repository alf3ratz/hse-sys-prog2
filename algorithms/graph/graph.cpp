#include <climits>
#include <iostream>
#include <vector>

using namespace std;


struct Edge {
    int to;
    int weight;
    Edge(int t, int w) : to(t), weight(w) {}
};

// Структура для представления графа
class Graph {
   private:
    int V;  // Количество вершин
    vector<vector<Edge>> adj;  // Список смежности для хранения ребер

   public:
    Graph(int V) : V(V) { adj.resize(V); }

    // Добавление ребра в граф
    void addEdge(int from, int to, int weight) {
        adj[from].emplace_back(to, weight);
        adj[to].emplace_back(
            from,
            weight); 
    }

    // Метод для поиска вершины с минимальным ключом
    int minKey(vector<int>& key, vector<bool>& mstSet) {
        int min = INT_MAX, min_index = -1;
        // cout << "--------------------------------:\n";
        // cout << "----- minKey methond start -----:\n";
        for (int v = 0; v < V; ++v) {
            if (!mstSet[v] && key[v] < min) {
                // cout << "v=" << v << " key[v]=" << key[v] << " min=" << min
                //      << '\n';
                min = key[v];
                min_index = v;
            }
        }
        // cout << "return min_index=" << min_index << '\n';
        // cout << "----- minKey methond end -----:\n";
        // cout << "--------------------------------:\n";
        return min_index;
    }

    // Алгоритм Прима для поиска минимального остова
    void primMST() {
        vector<int> parent(V, -1);  // Вектор для хранения минимального остова
        vector<int> key(V, INT_MAX);  // Вектор для хранения весов ребер
        vector<bool> mstSet(V, false);  // Массив для отметки посещенных вершин

        // Начинаем строить остов с первой вершины
        key[0] = 0;
        parent[0] = -1;  // Начальная вершина не имеет родителя

        for (int count = 0; count < V - 1; ++count) {
            // cout << "key=[";
            // for (auto elem : key) {
            //     cout << elem << ",";
            // }
            // cout << "]\n";
            // cout << "mstSet=[";
            // for (auto elem : mstSet) {
            //     cout << elem << ",";
            // }
            // cout << "]\n";
            int u = minKey(key, mstSet);
            mstSet[u] = true;
            // cout << "Посетили вершину с индексом=" << u << '\n';
            // Обновляем ключи смежных вершин
            for (auto& edge : adj[u]) {
                int v = edge.to;
                int weight = edge.weight;
                if (!mstSet[v] && weight < key[v]) {
                    // cout << "v=" << v << " key[v]=" << key[v] << " parent[v]"
                    //      << u << " weight=" << weight << '\n';
                    parent[v] = u;
                    key[v] = weight;
                } else {
                    cout << "Так как вес от вершины " << u << " к вершине " << v
                         << "=" << weight
                         << " больше, чем уже найденный вес= " << key[v]
                         << " то игнорируем дугу\n";
                }
            }
            // cout << "parent=[";
            // for (auto elem : parent) {
            //     cout << elem << ",";
            // }
            // cout << "]\n";
        }

        // Выводим минимальный остов
        cout << "Веса минимального остова:\n";
        for (int i = 1; i < V; ++i) cout << parent[i] << " - " << i << "\n";
    }
};


int main() {
    
    Graph g(8);
    g.addEdge(0, 1, 2);
    g.addEdge(0, 2, 3);
    g.addEdge(0, 3, 4);
    g.addEdge(1, 5, 11);
    g.addEdge(1, 7, 2);
    g.addEdge(2, 6, 10);
    g.addEdge(3, 4, 13);
    g.addEdge(4, 5, 5);
    g.addEdge(5, 6, 4);
    g.primMST();

    return 0;
}
