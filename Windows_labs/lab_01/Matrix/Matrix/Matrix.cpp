#include <iostream>
#include <vector>
#include <windows.h> // Основная библиотека для WinAPI
#include <ctime>

int N;

// Замена std::mutex на CRITICAL_SECTION (быстрее для Windows внутри одного процесса)
CRITICAL_SECTION cs;

std::vector<std::vector<long long>> A, B, C;

// Так как CreateThread принимает только один аргумент (LPVOID),
// создадим структуру для передачи всех параметров блока
struct ThreadData {
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int block_size;
};

// Функция потока должна иметь сигнатуру DWORD WINAPI FunctionName(LPVOID lpParam)
DWORD WINAPI MultiplyBlock(LPVOID lpParam) {
    // Распаковываем данные
    ThreadData* data = (ThreadData*)lpParam;
    int x1 = data->x1; int y1 = data->y1;
    int x2 = data->x2; int y2 = data->y2;
    int x3 = data->x3; int y3 = data->y3;
    int block_size = data->block_size;

    // Локальный буфер для вычислений (чтобы не блокировать мьютекс слишком часто)
    std::vector<std::vector<long long>> res(block_size, std::vector<long long>(block_size, 0));

    // Вычисления (логика осталась прежней)
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

    // Критическая секция (аналог mtx.lock)
    EnterCriticalSection(&cs);
    for (int dx = 0; dx < block_size; dx++) {
        for (int dy = 0; dy < block_size; dy++) {
            if (x3 + dx >= N || y3 + dy >= N) continue;
            C[x3 + dx][y3 + dy] += res[dx][dy];
        }
    }
    // Выход из критической секции (аналог mtx.unlock)
    LeaveCriticalSection(&cs);

    // Удаляем структуру, которую создали в main (new ThreadData)
    delete data;
    return 0;
}

int main() {
    // Настройка локали для корректного вывода русского текста
    setlocale(LC_ALL, "Russian");

    int block_size, blocks_in_row;
    std::cout << "Введите размер матрицы: ";
    std::cin >> N;
    std::cout << "Введите размер блока (число должно делить размер матрицы): ";
    std::cin >> block_size;

    // Инициализация критической секции
    InitializeCriticalSection(&cs);

    // Инициализация матриц
    A.assign(N, std::vector<long long>(N, 0));
    srand((unsigned int)time(nullptr));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10; // Уменьшил диапазон для наглядности, можно вернуть 1000
        }
    }

    B.assign(N, std::vector<long long>(N, 0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            B[i][j] = rand() % 10;
        }
    }

    C.assign(N, std::vector<long long>(N, 0));

    blocks_in_row = (N + block_size - 1) / block_size; // Округление вверх, если N не делится нацело

    // Вектор для хранения дескрипторов (Handles) потоков
    std::vector<HANDLE> hThreads;

    for (int i = 0; i < blocks_in_row; i++) {
        for (int j = 0; j < blocks_in_row; j++) {
            for (int k = 0; k < blocks_in_row; k++) {
                // Выделяем память под данные для потока в куче
                ThreadData* data = new ThreadData;
                data->x1 = i * block_size;
                data->y1 = k * block_size;
                data->x2 = k * block_size;
                data->y2 = j * block_size;
                data->x3 = i * block_size;
                data->y3 = j * block_size;
                data->block_size = block_size;

                // Создаем поток Windows
                // Параметры: (Security, StackSize, FuncPtr, ArgPtr, Flags, ThreadId)
                HANDLE hThread = CreateThread(NULL, 0, MultiplyBlock, data, 0, NULL);

                if (hThread == NULL) {
                    std::cerr << "Ошибка создания потока!" << std::endl;
                    delete data; // Если поток не создан, чистим память сами
                }
                else {
                    hThreads.push_back(hThread);
                }
            }
        }
    }

    // Ожидание завершения потоков
    // WaitForMultipleObjects имеет лимит 64 объекта, поэтому надежнее ждать в цикле
    for (auto& h : hThreads) {
        WaitForSingleObject(h, INFINITE); // Ждем каждый поток до бесконечности
        CloseHandle(h); // Обязательно закрываем дескриптор, чтобы не было утечки ресурсов
    }

    // Удаление критической секции
    DeleteCriticalSection(&cs);

    std::cout << "\nМатрица A:\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << A[i][j] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "\nМатрица B:\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << B[i][j] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "\nРезультат (Матрица C):\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << C[i][j] << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}