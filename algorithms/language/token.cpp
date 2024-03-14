#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>

enum TokenType {
    VARIABLE,
    CONSTANT,
    OPERATION,
    INPUT,
    OUTPUT,
    ASSIGNMENT,
    INVALID
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::istringstream iss(input);
    std::string token;

    while (iss >> token) {
        Token t;
        if (token == "As") {
            t.type = ASSIGNMENT;
        } else if (token == "Anagnosi") {
            t.type = INPUT;
        } else if (token == "Grafo") {
            t.type = OUTPUT;
        } else if (islower(token[0])) {
            t.type = VARIABLE;
        } else if (std::isdigit(token[0])) {
            t.type = CONSTANT;
        } else if (token == "+" || token == "-" || token == "*" || token == "/") {
            t.type = OPERATION;
        } else {
            t.type = INVALID;
        }
        t.value = token;
        tokens.push_back(t);
    }

    return tokens;
}

void printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << "Type: " << token.type << ", Value: " << token.value << std::endl;
    }
}

int main() {
    std::string input = "As uno = XI + C";
    std::vector<Token> tokens = tokenize(input);
    printTokens(tokens);
    return 0;
}
