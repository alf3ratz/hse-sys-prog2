#include <cctype>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

std::map<int, std::string> m;
enum TokenType {
    VARIABLE,
    NUMBER,
    OPERATION,
    KEYWORD,
    ASSIGNMENT,
    INVALID,
    EOL
};

struct Token {
    TokenType type;
    std::string value;
};

bool isValidRoman(std::string s) {
    // Создаем словарь для соответствия римских цифр и их значений
    std::unordered_map<char, int> romanValues = {
        {'I', 1},   {'V', 5},   {'X', 10},  {'L', 50},
        {'C', 100}, {'D', 500}, {'M', 1000}};

    int prevValue = 0;  // Предыдущее значение
    int repeatCount = 0;  // Количество повторений символа
    for (char c : s) {
        // Проверяем, что символ c есть в словаре
        if (romanValues.find(c) == romanValues.end()) {
            return false;  // Если символ не найден, строка не является римским
                           // числом
        }

        int value = romanValues[c];

        // Если значение текущей римской цифры равно предыдущему, увеличиваем
        // счетчик повторений
        if (value == prevValue) {
            repeatCount++;
            if (c == 'D' || c == 'L' || c == 'V') {
                return false;  // D, L и V не могут повторяться
            }
            if (repeatCount > 3) {
                return false;  // Не более трех повторений символов
            }
        } else {
            repeatCount = 1;  // Сбрасываем счетчик повторений
        }

        // Если значение текущей римской цифры меньше предыдущего и не является
        // допустимой комбинацией, строка не является римским числом
        if (value < prevValue &&
            !(prevValue / value == 5 || prevValue / value == 10)) {
            return false;
        }

        prevValue = value;  // Обновляем предыдущее значение
    }

    return true;
}

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::istringstream iss(input);
    std::string token;

    while (iss >> token) {
        Token t;
        if (token == "As" || token == "Anagnosi" || token == "Grafo") {
            t.type = KEYWORD;
        } else if (islower(token[0])) {
            t.type = VARIABLE;
        } else if (isValidRoman(token)) {
            t.type = NUMBER;
        } else if (token == "+" || token == "-" || token == "*" ||
                   token == "/") {
            t.type = OPERATION;
        } else if (token == "=") {
            t.type = ASSIGNMENT;
        } else if (token == "\n") {
            t.type = EOL;
        } else {
            t.type = INVALID;
        }
        t.value = token;
        tokens.push_back(t);
    }

    return tokens;
}

void printTokens(const std::vector<Token>& tokens, std::string input) {
    std::cout << input << '\n';
    for (const auto& token : tokens) {
        std::cout << "<" << m[token.type] << ">";
    }
    std::cout << '\n';
}

int main() {
    m[TokenType::VARIABLE] = "VARIABLE";
    m[TokenType::NUMBER] = "NUMBER";
    m[TokenType::OPERATION] = "OPERATION";
    m[TokenType::KEYWORD] = "KEYWORD";
    m[TokenType::ASSIGNMENT] = "ASSIGNMENT";
    m[TokenType::INVALID] = "INVALID";
    m[TokenType::EOL] = "EOL";

    std::string input = "As uno = XI + C\nAs de = Anagnosi";
    std::vector<Token> tokens = tokenize(input);
    printTokens(tokens, input);
    return 0;
}
