#include <iostream>
#include <cmath>
#include <random>

// Определение функции качества (пример)
double qualityFunction(double x, double y) {
    return x * x + y * y; // Пример: квадрат суммы аргументов
}

// Градиент функции качества
void gradient(double x, double y, double& gradX, double& gradY) {
    // Производные функции (пример)
    gradX = 2 * x;
    gradY = 2 * y;
}

// Метод отжига с использованием градиентного спуска
void simulatedAnnealing(double startX, double startY, double alpha, double epsilon, int maxIterations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    double x = startX;
    double y = startY;

    double gradX, gradY;
    double currentQuality = qualityFunction(x, y);

    for (int i = 0; i < maxIterations; ++i) {
        gradient(x, y, gradX, gradY);

        // Обновление координат
        x -= alpha * gradX + dis(gen) * epsilon;
        y -= alpha * gradY + dis(gen) * epsilon;

        // Обновление текущего значения функции качества
        double newQuality = qualityFunction(x, y);

        // Если новое значение лучше, чем текущее, сохраняем его
        if (newQuality < currentQuality) {
            currentQuality = newQuality;
        }
    }

    std::cout << "Минимум функции: (" << x << ", " << y << "), Значение функции: " << currentQuality << std::endl;
}

int main() {
    double startX = 2.0; // Начальная точка x
    double startY = 2.0; // Начальная точка y
    double alpha = 0.01; // Шаг градиентного спуска
    double epsilon = 0.1; // Параметр метода отжига
    int maxIterations = 10000; // Максимальное количество итераций

    simulatedAnnealing(startX, startY, alpha, epsilon, maxIterations);

    return 0;
}
