#include <iostream>
#include <queue>
#include <vector>

void printMatrix(const std::vector<std::vector<int>>& matrix,
                 std::vector<std::vector<bool>> visited) {
    for (int i = 0; i < matrix.size() +2; ++i) {
        std::cout << "$";
    }
    std::cout << std::endl;
    for (int i = 0; i < matrix.size(); i++) {
        std::cout << "$";
        for (int j = 0; j < matrix[i].size() ; ++j) {
            if (visited[i][j]) {
                if (matrix[i][j] == 1)
                    std::cout << " ";
            } else {
                std::cout << "@";
            }
        }
        std::cout << "$";
        std::cout << '\n';
    }
     for (int i = 0; i < matrix.size() +2; ++i) {
        std::cout << "$";
    }
    std::cout << '\n';
}

struct Point {
    int x, y;
};

// Функция проверки допустимости хода в заданные координаты
bool isValid(int x, int y, int rows, int cols,
             std::vector<std::vector<int>>& maze) {
    return x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] == 1;
}

// Функция поиска пути в лабиринте с использованием BFS и эвристики
bool findPath(std::vector<std::vector<int>>& maze, Point start, Point end) {
    int rows = maze.size();
    int cols = maze[0].size();

    std::vector<std::vector<bool>> visited(
        rows, std::vector<bool>(
                  cols, false));  // Массив для отслеживания посещенных ячеек
    std::queue<Point> q;  // Очередь для BFS

    q.push(start);  
    visited[start.x][start.y] =
        true; 

    // Направления движения: влево, вправо, вниз, вверх
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!q.empty()) {
        std::cout << "|\n";
        std::cout << "V\n";
        printMatrix(maze, visited);
        Point curr = q.front();
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

int main() {
    std::vector<std::vector<int>> maze = {{1, 0, 1, 0, 1},
                                          {1, 1, 1, 0, 0},
                                          {0, 0, 1, 0, 0},
                                          {1, 1, 1, 1, 1},
                                          {1, 0, 1, 0, 1}};

    Point start = {0, 0};  // Начальная точка
    Point end = {4, 4};    // Конечная точка

    
    if (findPath(maze, start, end)) {
        std::cout << "Путь найден!" << std::endl;
    } else {
        std::cout << "Путь не найден!" << std::endl;
    }

    return 0;
}
