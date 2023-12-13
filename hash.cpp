#include <openssl/evp.h>

#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

std::mutex printMutex;
void generateCombinationsHelper(const std::string &characters, int length,
                                std::string current,
                                std::vector<std::string> &combinations) {
    if (length == 0) {
        combinations.push_back(current);
        return;
    }

    for (char c : characters) {
        generateCombinationsHelper(characters, length - 1, current + c,
                                   combinations);
    }
}

std::vector<std::string> generateCombinations(int length) {
    std::string characters = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::string startingDigit = "";
    std::vector<std::string> combinations;
    generateCombinationsHelper(characters, length - 1, startingDigit,
                               combinations);

    return combinations;
}

std::string calculateMD5(const std::string &str) {
    EVP_MD_CTX *mdContext = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_md5();  // Use md5() for MD5

    EVP_DigestInit_ex(mdContext, md, nullptr);
    EVP_DigestUpdate(mdContext, str.c_str(), str.length());

    unsigned char digest[EVP_MD_size(md)];
    EVP_DigestFinal_ex(mdContext, digest, nullptr);
    EVP_MD_CTX_free(mdContext);

    std::stringstream ss;
    for (int i = 0; i < EVP_MD_size(md); i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }

    return ss.str();
}

void parallelHashCrack(const std::string &hashToCrack, int start, int end) {
    for (int i = start; i <= end; i++) {
        // std::vector<std::string> result = generateCombinations(i);
        // for (const auto &combination : result) {
        std::string result = calculateMD5(std::to_string(i));
        if (result == hashToCrack) {
            std::lock_guard<std::mutex> guard(printMutex);
            std::cout << "Найдено совпадение: " << i << '\n';
            return;
        }
        // }
    }
    std::cout << "Совпадение не найдено :(" << '\n';
}

// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         std::cerr << "Использование: " << argv[0] << " <your hash>" << '\n';
//         return 1;
//     }

//     std::string hashToCrack = argv[1];

//     // Автоматически выбираем количество процессов
//     int numThreads = std::thread::hardware_concurrency();

//     // Делим задачу на интервалы для каждого потока
//     int valuesPerThread = 1000000000;
//     std::vector<std::thread> threads;

//     for (int i = 0; i < numThreads; i++) {
//         int start = i * valuesPerThread;
//         int end = (i + 1) * valuesPerThread - 1;

//         threads.emplace_back(parallelHashCrack, std::ref(hashToCrack), start,
//                              end);
//     }

//     // Дожидаемся завершения всех потоков
//     for (auto &thread : threads) {
//         thread.join();
//     }

//     return 0;
// }
