#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

const int numThreads = 2;

// Генератор случайных символов
char generateRandomChar() {
	static random_device rd;
	static mt19937 gen(rd());
	static uniform_int_distribution<> dis('a', 'z');
	return static_cast<char>(dis(gen));
}

// Функция для генерации случайной строки
string generateRandomString(int length) {
	string randomString;
	randomString.reserve(length);
	for (int i = 0; i < length; ++i) {
		randomString.push_back(generateRandomChar());
	}
	return randomString;
}

// Функция для подсчета числа вхождений "a" в строке
int countAInString(const string& str) {
	return count(str.begin(), str.end(), 'a');
}

int main() {
	const int totalChar = 1000000;
	const string randomString = generateRandomString(totalChar);
    vector<thread> threads;
    vector<int> results(numThreads);

	for (int i = 0; i < numThreads; ++i) {
		int chunkSize = totalChar / numThreads;
		int start = i * chunkSize;
		int end = (i == numThreads - 1) ? totalChar : (i + 1) * chunkSize;
		threads.emplace_back([&, start, end, i] {
			results[i] = countAInString(randomString.substr(start, end - start));
		});
	}
	
	// Дожидаемся завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }

	// Итоговый подсчет (редукция)
    int totalA = 0;
    for (int result : results) {
        totalA += result;
    }
    
    cout << "Total 'a' count: " << totalA;
}