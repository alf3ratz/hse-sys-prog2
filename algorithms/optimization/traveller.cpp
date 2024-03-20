#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

using namespace std;

// Вычисляем итоговую длину маршрута
int calculateRouteLength(const vector<vector<int>>& graph,
                         const vector<int>& path) {
    int length = 0;
    for (int i = 0; i < path.size() - 1; ++i) {
        length += graph[path[i]][path[i + 1]];
    }
    length += graph[path.back()][path[0]];  
                                            
    return length;
}

// Жадный алгоритм для нахождения минимального пути коммивояжера
vector<int> greedyTSP(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> path;
    vector<bool> visited(n, false);

    // Начинаем с вершины 0
    int current = 0;
    visited[current] = true;
    path.push_back(current);

    // Добавляем ближайщую непосещенную вершину
    for (int i = 0; i < n - 1; ++i) {
        int next = -1;
        int minDistance = INT_MAX;
        for (int j = 0; j < n; ++j) {
            if (!visited[j] && graph[current][j] < minDistance) {
                next = j;
                minDistance = graph[current][j];
            }
        }
        current = next;
        visited[current] = true;
        path.push_back(current);
    }

    return path;
}

int main() {
    vector<vector<int>> graph = {{0, 10, 15, 20, 25},
                                 {10, 0, 12, 25, 30},
                                 {15, 12, 0, 30, 17},
                                 {20, 25, 30, 0, 50},
                                 {25, 30, 17, 50, 0}};

    vector<int> path = greedyTSP(graph);

    cout << "Найденный путь: ";
    for (int vertex : path) {
        cout << vertex << " ";
    }
    cout << '\n';
    cout << "Длина пути: " << calculateRouteLength(graph, path) << '\n';

    return 0;
}
