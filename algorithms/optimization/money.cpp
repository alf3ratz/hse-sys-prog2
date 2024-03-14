#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Функция для измерения времени выполнения другой функции
template <typename Func, typename... Args>
std::pair<long, int> measureTime(Func&& func, Args&&... args) {
    auto start =
        std::chrono::high_resolution_clock::now();  // Запускаем таймер перед
                                                    // выполнением функции
    auto res = std::forward<Func>(func)(
        std::forward<Args>(args)...);  // Выполняем переданную функцию
    auto end =
        std::chrono::high_resolution_clock::now();  // Останавливаем таймер
                                                    // после выполнения функции
    return std::make_pair(
        res, std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                 .count());  // Возвращаем время выполнения в миллисекундах
}

// Функция для нахождения наименьшего числа монет при размене суммы
int minCoinsDP(std::vector<int>& coins, int amount) {
    int n = coins.size();
    std::vector<int> dp(amount + 1,
                        INT_MAX);  // Инициализируем массив для хранения
                                   // минимального числа монет для каждой суммы
    dp[0] = 0;  // Нулевая сумма требует нуля монет

    // Вычисляем минимальное число монет для каждой суммы от 1 до target
    for (int i = 1; i <= amount; ++i) {
        for (int j = 0; j < n; ++j) {
            if (coins[j] <= i && dp[i - coins[j]] != INT_MAX) {
                dp[i] = std::min(dp[i], dp[i - coins[j]] + 1);
            }
        }
    }

    return dp[amount];  // Возвращаем минимальное число монет для суммы target
}

// Функция для нахождения наименьшего числа монет при размене суммы
int minCoinsGreedy(std::vector<int>& coins, int amount) {
    std::sort(coins.begin(), coins.end(),
              std::greater<long>());  // Сортируем монеты по убыванию номинала
    int count = 0;
    for (int i = 0; i < coins.size(); ++i) {
        while (amount >= coins[i]) {
            amount -= coins[i];
            ++count;
        }
    }
    return count;
}

std::vector<int> getCoinNums(std::string str) {
    std::vector<int> numbers;
    std::stringstream ss(str);
    std::string token;

    // Разделяем строку по запятым и извлекаем числа
    while (std::getline(ss, token, ',')) {
        try {
            int number = std::stoi(token);  // Преобразуем строку в число
            std::cout << number << '\n';
            numbers.push_back(number);  // Добавляем число в список
        } catch (const std::invalid_argument& e) {
            std::cerr << "Ошибка: " << e.what()
                      << " - некорректное число: " << token << "\n";
        } catch (const std::out_of_range& e) {
            std::cerr << "Ошибка: " << e.what()
                      << " - число слишком большое: " << token << "\n";
        }
    }
    return numbers;
}

int main() {
    std::string input;
    int amount = 54323423;
    std::cout << "Enter string contains from nums and ,: ";
    std::getline(std::cin, input);
    std::vector<int> coins = getCoinNums(input);

    std::cout << "Enter sum: ";
    std::cin >> amount;
    std::cout << "Started..."
              << "\n";
    auto resultGreedy = measureTime(minCoinsGreedy, coins, amount);
    std::cout << "Minimum coins (greedy): " << resultGreedy.first << "\n";
    std::cout << "Time taken by greedy algorythm: " << resultGreedy.second
              << " milliseconds"
              << "\n";

    auto resultDP = measureTime(minCoinsDP, coins, amount);
    std::cout << "Minimum coins (DP): " << resultDP.first << "\n";
    std::cout << "Time taken by dynamic programming algorythm: "
              << resultDP.second << " milliseconds"
              << "\n";
    return 0;
}
