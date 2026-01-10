#include"buffered_channel.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>

std::vector<std::vector<long long>> A, B, C;

int N;

struct DataForWorker {
    int x1, y1, x2, y2, x3, y3, block_size;

    DataForWorker(int x1, int y1, int x2, int y2, int x3, int y3, int block_size) : x1(x1), x2(x2), x3(x3),
        y1(y1), y2(y2), y3(y3), block_size(block_size) {}

    DataForWorker() : x1(0), x2(0), x3(0),
        y1(0), y2(0), y3(0), block_size(0) {}
};

BufferedChannel<DataForWorker> channel(10);
std::mutex mtx;

void multiply() {
    while (true) {
        auto [data, is_received] = channel.receive();
        if (!is_received) return;
        auto [x1, y1, x2, y2, x3, y3, block_size] = data;
        std::vector<std::vector<long long>> res(block_size, std::vector<long long>(block_size, 0));
        for (int dx = 0; dx < block_size; dx++) {
            for (int dy = 0; dy < block_size; dy++) {
                long long sum = 0;
                for (int k = 0; k < block_size; k++) {
                    if (x1 + dx >= N || y1 + k >= N || x2 + k >= N || y2 + dy >= N) continue;
                    sum += A[x1 + dx][y1 + k] * B[x2 + k][y2 + dy];
                }
                res[dx][dy] = sum;
            }
        }
        mtx.lock();
        for (int dx = 0; dx < block_size; dx++) {
            for (int dy = 0; dy < block_size; dy++) {
                if (x3 + dx >= N || y3 + dy >= N) continue;
                C[x3 + dx][y3 + dy] += res[dx][dy];
            }
        }
        mtx.unlock();
    }
}

std::vector<std::vector<long long>> multiplyMatrices(
    const std::vector<std::vector<long long>>& A,
    const std::vector<std::vector<long long>>& B) {

    int n = A.size();
    std::vector<std::vector<long long>> D(n, std::vector<long long>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            long long sum = 0;
            for (int k = 0; k < n; k++) {
                sum += A[i][k] * B[k][j];
            }
            D[i][j] = sum;
        }
    }

    return D;
}

bool areMatricesEqual(const std::vector<std::vector<long long>>& A,
    const std::vector<std::vector<long long>>& B) {
    int n = A.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (A[i][j] != B[i][j]) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    int block_size, blocks_in_row;
    std::cout << "Введите размер матрицы: ";
    std::cin >> N;
    std::cout << "Введите размер блока (число должно делить размер матрицы): ";
    std::cin >> block_size;

    A.assign(N, std::vector<long long>(N, 0));
    srand((time(nullptr)));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 1000;
        }
    }

    B.assign(N, std::vector<long long>(N, 0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            B[i][j] = rand() % 1000;
        }
    }
    std::cout << '\n';
    std::cout << "Матрица A: \n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << A[i][j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
    std::cout << "Матрица B: \n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << B[i][j] << ' ';
        }
        std::cout << '\n';
    }

    C.assign(N, std::vector<long long>(N, 0));

    blocks_in_row = (N + block_size - 1) / block_size;
    int total_threaads = 10;
    std::vector<std::thread> threads;
    for (int i = 0; i < total_threaads; i++) {
        threads.emplace_back(multiply);
    }
    for (int i = 0; i < blocks_in_row; i++) {
        for (int j = 0; j < blocks_in_row; j++) {
            for (int k = 0; k < blocks_in_row; k++) {
                channel.send(DataForWorker(i * block_size, k * block_size,
                    k * block_size, j * block_size, i * block_size, j * block_size, block_size));
            }
        }
    }


    channel.close();

    for (auto& t : threads) {
        t.join();
    }
    std::cout << '\n';
    std::cout << "Матрица C: \n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << C[i][j] << ' ';
        }
        std::cout << '\n';
    }

    std::vector<std::vector<long long>> D = multiplyMatrices(A, B);

    std::cout << '\n';
    std::cout << "Матрица D: \n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << C[i][j] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << (areMatricesEqual(D, C) ? "Yes" : "No") << '\n';

    return 0;
}
