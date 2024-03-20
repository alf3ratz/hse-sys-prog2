#include <iostream>
#include <vector>

using namespace std;

// Функция для решения линейной программы методом симплекса
int simplex_method(vector<vector<int>>& A,
                                           vector<int>& power, vector<int>& resources) {
   std::vector<int> dp(3, 0); 

    for (int res = 0; res < 3; ++res) { 
        for (int creature = 0; creature < 3; ++creature) { 
            if (A[res][creature] <= resources[res]) { 
                
                cout<<"dp[creature]="<<dp[creature]<<'\n';
                cout<<"power[creature] + dp[res] - A[res][creature]="<<power[creature] + dp[res] - A[res][creature]<<'\n';
                dp[creature] =power[creature] + dp[res] /*- A[res][creature]*/;
            }
        }
    }
    cout<<dp[0]<<"--"<<dp[1]<<"--"<<dp[2]<<'\n';
    return dp[0];
}
struct Creature {
    int power;
    int costGold;
    int costGems;
    int costCrystals;
};
Creature create_creature() {}

int main() {
    // Ввод количества ресурсов и их стоимостей для каждого существа
    int gold, gems, crystals;
    // cout << "Введите имеющееся кол-во золота: ";
    // cin >> gold;
    // cout << "Введите имеющееся кол-во драгоценностей: ";
    // cin >> gems;
    // cout << "Введите имеющееся кол-во кристаллов: ";
    // cin >> crystals;

    // double cost_mag, cost_knight, cost_dragon;
    // cout << "Введите : ";
    // cin >> cost_mag;
    // cin >> cost_knight;
    // cin >> cost_dragon;

    // Формирование коэффициентов линейной программы
    vector<vector<int>> a = {{1, 3, 5}, {2, 4, 6}, {3, 5, 7}};
    vector<int> c = {1, 2, 3};
    vector<int> b = {10, 10, 10};  

    
    int res = simplex_method(a, c, b);
  
    std::cout << "Максимальная сила: " << res << '\n';
    

    return 0;
}
