#include <vector>
#include <iostream>
#include <fstream>
#include "sieve.h"
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Использование: " << argv[0] << " число\n";
        return 1;
    }

    string text(argv[1]);

   unsigned long long number = 0;

try {
    number = stoull(text);
} catch (const invalid_argument&) {
    cerr << "Это не число\n";
    exit(1);
} catch (const out_of_range&) {
    cerr << "Число слишком большое для unsigned long long\n";
    exit(1);
}

if (text.find('-') != string::npos) {
    cerr << "Число отрицательное\n";
    exit(1);
}

    vector<unsigned long long> result = sieve::SieveOfAtkhin(number);
    ofstream file("primes_chk");

    if (!file.is_open()) {
        cerr << "Не удалось открыть файл\n";
        return 1;
    }

    for (auto num : result) {
        file << num << '\n';
    }

    file.close();

    return 0;
}