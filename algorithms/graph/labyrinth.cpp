#include <iostream>
#include <queue>
#include <vector>

struct Point {
    int x, y;
};
void printMatrix(const std::vector<std::vector<int>>& matrix,
                 std::vector<std::vector<bool>> visited, Point current) {
    for (int i = 0; i < matrix.size() + 2; ++i) {
        std::cout << "$";
    }
    std::cout << std::endl;
    for (int i = 0; i < matrix.size(); i++) {
        std::cout << "$";
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (j == current.y && i == current.x) {
                std::cout << "A";
            } else if (visited[i][j]) {
                if (matrix[i][j] == 1) std::cout << " ";
            } else {
                std::cout << "@";
            }
        }
        std::cout << "$";
        std::cout << '\n';
    }
    for (int i = 0; i < matrix.size() + 2; ++i) {
        std::cout << "$";
    }
    std::cout << '\n';
}
bool isValid(int x, int y, int rows, int cols,
             std::vector<std::vector<int>>& maze) {
    return x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] == 1;
}
// Функция проверки допустимости хода в заданные координаты
bool isValidNode(int x, int y, const std::vector<std::vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    return x >= 0 && x < rows && y >= 0 && y < cols && matrix[x][y] == 1;
}

bool findPath(std::vector<std::vector<int>>& maze, Point start, Point end) {
    int rows = maze.size();
    int cols = maze[0].size();

    std::vector<std::vector<bool>> visited(rows,
                                           std::vector<bool>(cols, false));
    std::queue<Point> q;

    q.push(start);
    visited[start.x][start.y] = true;

    // Направления движения: влево, вправо, вниз, вверх
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!q.empty()) {
        std::cout << "|\n";
        std::cout << "V\n";
        std::queue<Point> tempQ = q;

        // Отображение всех элементов в очереди
        while (!tempQ.empty()) {
            std::cout << "(" << tempQ.front().x << ", " << tempQ.front().y
                      << ")"
                      << " ";  // Выводим элемент в передней
                               // части временной копии очереди
            tempQ.pop();  // Удаляем элемент из передней части временной копии
                          // очереди
        }

        std::cout << std::endl;
        Point curr = q.front();
        printMatrix(maze, visited, curr);
        q.pop();

        if (curr.x == end.x &&
            curr.y == end.y) {  // Если достигли конечной точки, путь найден
            return true;
        }

        // Просматриваем все соседние ячейки
        for (int i = 0; i < 4; ++i) {
            int newX = curr.x + dx[i];
            int newY = curr.y + dy[i];

            if (isValid(newX, newY, rows, cols, maze) && !visited[newX][newY]) {
                q.push({newX, newY});  // Добавляем соседнюю ячейку в очередь
                visited[newX][newY] =
                    true;  // Помечаем соседнюю ячейку как посещенную
            }
        }
    }
    return false;
}

std::vector<std::vector<int>> findShortestPaths(
    const std::vector<std::vector<int>>& matrix, const Point& start) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    std::vector<std::vector<int>> shortestPaths(
        rows, std::vector<int>(cols, -1));  // Инициализируем массив расстояний

    std::queue<Point> q;

    q.push(start);
    shortestPaths[start.x][start.y] =
        0;  // Расстояние от начального узла до него самого равно 0

    while (!q.empty()) {
        Point current = q.front();
       
        // printMatrix(matrix, visited, current);
        q.pop();

        // Перебираем всех соседей текущего узла
        std::vector<Point> neighbors = {{current.x - 1, current.y},
                                        {current.x + 1, current.y},
                                        {current.x, current.y - 1},
                                        {current.x, current.y + 1}};

        for (const auto& neighbor : neighbors) {
            int newX = neighbor.x;
            int newY = neighbor.y;

            // Проверяем, является ли соседный узел допустимым и не посещенным
            if (isValidNode(newX, newY, matrix) &&
                shortestPaths[newX][newY] == -1) {
                q.push(neighbor);
                shortestPaths[newX][newY] =
                    shortestPaths[current.x][current.y] +
                    1;  // Расстояние от начального узла
            }
        }
    }

    return shortestPaths;
}

int main() {
    std::vector<std::vector<int>> maze = {{1, 0, 1, 1, 1},
                                          {1, 1, 1, 0, 0},
                                          {0, 0, 1, 0, 0},
                                          {1, 1, 1, 1, 1},
                                          {1, 0, 1, 0, 0}};

    Point start = {0, 0};  // Начальная точка
    Point end = {4, 0};    // Конечная точка

    std::vector<std::vector<int>> shortestPaths =
        findShortestPaths(maze, start);

    if (findPath(maze, start, end)) {
        std::cout << "Путь найден!" << '\n';
    } else {
        std::cout << "Путь не найден!" << '\n';
    }
    // Вывод кратчайших расстояний до всех достижимых узлов
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Кратчайшие расстояния до всех достижимых узлов:" << std::endl;
    for (int i = 0; i < shortestPaths.size(); ++i) {
        for (int j = 0; j < shortestPaths[i].size(); ++j) {
            if (shortestPaths[i][j] < 0) {
                std::cout << shortestPaths[i][j] << "| ";
            } else {
                std::cout << shortestPaths[i][j] << " | ";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
