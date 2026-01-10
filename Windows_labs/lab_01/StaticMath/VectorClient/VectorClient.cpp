#include <iostream>
#include "Vector.h" // Подключаем нашу DLL

// Нам нужно пространство имен MathLibrary, так как Vector использует Number
using namespace MathLibrary;

int main()
{
    std::cout << "--- Vector DLL Test Client ---" << std::endl;

    // 1. Проверка глобальных переменных (0,0) и (1,1)
    std::cout << "VectorZero radius: " << VectorZero.GetRadius().getValue() << std::endl;
    std::cout << "VectorOne x: " << VectorOne.x.getValue() << std::endl;

    // 2. Создание своих векторов
    // Создадим вектор (3, 4). Радиус должен быть 5.
    Vector v1(Number(3.0), Number(4.0));

    std::cout << "Vector (3, 4) created." << std::endl;
    std::cout << "Radius: " << v1.GetRadius().getValue() << std::endl; // Ожидаем 5
    std::cout << "Angle: " << v1.GetAngle().getValue() << std::endl;   // Ожидаем ~0.927

    // 3. Проверка сложения
    // (3, 4) + (1, 1) = (4, 5)
    Vector v2 = v1.Add(VectorOne);
    std::cout << "Addition result (v1 + VectorOne): ("
        << v2.x.getValue() << ", " << v2.y.getValue() << ")" << std::endl;

    return 0;
}