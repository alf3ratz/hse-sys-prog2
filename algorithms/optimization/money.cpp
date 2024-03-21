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
        std::chrono::high_resolution_clock::now();  
    auto res = std::forward<Func>(func)(
        std::forward<Args>(args)...);  
    auto end =
        std::chrono::high_resolution_clock::now();  
    return std::make_pair(
        res, std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                 .count());  
}

// Функция для нахождения наименьшего числа монет при размене суммы
int minCoinsDP(std::vector<int>& coins, int amount) {
    int n = coins.size();
    std::vector<int> dp(amount + 1,
                        INT_MAX);  
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

int main() {
    std::string input;
    int amount = 54323423;
    // std::cout << "Enter string contains from nums and ,: ";
    // std::getline(std::cin, input);
    std::vector<int> coins = {1,2,3,5,25};//getCoinNums(input);

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
