#include <cmath>
#include <iostream>
#include <random>

double qualityFunction(double x, double y) { return x * x + y * y; }

// Градиент функции качества
void gradient(double x, double y, double& gradX, double& gradY) {
    gradX = 2 * x;
    gradY = 2 * y;
}

void annealing(double startX, double startY, double alpha, double epsilon,
               int maxIterations, double coolingRate) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.01, 0.01);

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
        alpha *= coolingRate;

    }

    std::cout << "Минимум функции: (" << x << ", " << y
              << "), Значение функции: " << currentQuality << std::endl;
}
//инимум функции: (-0.144575, -0.265061), Значение функции: 9.8381e-05
//Минимум функции: (4.56425, 5.08988), Значение функции: 3.64294e-05
int main() {
    double startX = 2.0;   // Начальная точка x
    double startY = 2.0;   // Начальная точка y
    double alpha = 0.01;   // Шаг градиентного спуска
    double epsilon = 0.1;  // Параметр метода отжига
    int maxIterations = 10000;  // Максимальное количество итераций
    double coolingRate = 0.999;  // Коэффициент охлаждения
    annealing(startX, startY, alpha, epsilon, maxIterations, coolingRate);

    return 0;
}
