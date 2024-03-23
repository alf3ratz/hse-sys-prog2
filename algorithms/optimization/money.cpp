#include <algorithm>
#include <chrono>
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Функция для измерения времени выполнения другой функции
template <typename Func, typename... Args>
std::pair<long, int> measureTime(Func&& func, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    auto res = std::forward<Func>(func)(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    return std::make_pair(
        res, std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                 .count());
}

// Функция для нахождения наименьшего числа монет при размене суммы
int minCoinsDP(std::vector<int>& coins, int amount) {
    int n = coins.size();
    std::vector<int> dp(amount + 1, INT_MAX);
    dp[0] = 0;

    for (int i = 1; i <= amount; ++i) {
        for (int j = 0; j < n; ++j) {
            if (coins[j] <= i && dp[i - coins[j]] != INT_MAX) {
                dp[i] = std::min(dp[i], dp[i - coins[j]] + 1);
            }
        }
    }

    return dp[amount];
}

int minCoinsGreedy(std::vector<int>& coins, int amount) {
    std::sort(coins.begin(), coins.end(),
              std::greater<long>());  // Сортируем монеты по убыванию
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

    while (std::getline(ss, token, ',')) {
        int number = std::stoi(token);
        std::cout << number << '\n';
        numbers.push_back(number);
    }
    return numbers;
}

void saveMatrixToCSV(const std::vector<std::vector<long>>& matrix,
                     const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла " << filename << std::endl;
        return;
    }

    for (const auto& row : matrix) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i != row.size() - 1) {
                file << ",";  // Разделитель между элементами в строке
            }
        }
        file << std::endl;  // Переход на новую строку
    }

    file.close();
    std::cout << "Матрица успешно сохранена в файле " << filename << std::endl;
}

int main() {
    std::string input;
    int amount = 54323423;

    // std::cout << "Enter string contains from nums and ,: ";
    // std::getline(std::cin, input);
    std::vector<int> coins = {1, 2, 3, 5, 25};
    std::vector<int> coins2 = {7, 13, 21, 188, 94};
    std::vector<int> amounts = {119,   7240,   10324,    15967,
                                32429, 324324, 43234234, 54323423,743234234};

    // std::cout << "Enter sum: ";
    // std::cin >> amount;
    // std::cout << "Started..."
    //           << "\n";
    std::vector<std::vector<long>> times = {{}, {}};

    for (auto num : amounts) {
        coins = {1, 2, 3, 5, 25};
        std::cout << "Кол-во монет для размена: " << num << "\n";
        auto resultGreedy = measureTime(minCoinsGreedy, coins, num);
        std::cout << "Минимальный размен (greedy): " << resultGreedy.first
                  << "\n";
        std::cout << "Время выполнения: " << resultGreedy.second
                  << " миллисекунд"
                  << "\n";
        times[0].push_back(resultGreedy.second);
        coins = {1, 2, 3, 5, 25};
        auto resultDP = measureTime(minCoinsDP, coins, num);
        std::cout << "Минимальный размен (DP): " << resultDP.first << "\n";
        std::cout << "Время выполнения: " << resultDP.second << " миллисекунд"
                  << "\n";
        times[1].push_back(resultDP.second);
        std::cout << "-------------\n";
    }
    saveMatrixToCSV(times, "test.csv");
    times = {{}, {}};
    for (auto num : amounts) {
        coins2 = {7, 13, 21, 188, 94};
        std::cout << "Кол-во монет для размена: " << num << "\n";
        auto resultGreedy = measureTime(minCoinsGreedy, coins2, num);
        std::cout << "Минимальный размен (greedy): " << resultGreedy.first
                  << "\n";
        std::cout << "Время выполнения: " << resultGreedy.second
                  << " миллисекунд"
                  << "\n";
        times[0].push_back(resultGreedy.second);
        coins2 = {7, 13, 21, 188, 94};
        auto resultDP = measureTime(minCoinsDP, coins2, num);
        std::cout << "Минимальный размен (DP): " << resultDP.first << "\n";
        std::cout << "Время выполнения: " << resultDP.second << " миллисекунд"
                  << "\n";
        times[1].push_back(resultDP.second);
        std::cout << "-------------\n";
    }
    saveMatrixToCSV(times, "test2.csv");
    return 0;
}
