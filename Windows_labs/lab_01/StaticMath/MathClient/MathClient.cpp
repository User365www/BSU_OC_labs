#include <iostream>
#include "MathLibrary.h"

using namespace std;
using namespace MathLibrary;

int main()
{
    cout << "=== Testing MathLibrary ===" << endl;

    // Тест 1: Глобальные переменные
    cout << "\n1. Testing global variables:" << endl;
    cout << "ZERO = " << ZERO.getValue() << endl;
    cout << "ONE = " << ONE.getValue() << endl;

    // Тест 2: Создание чисел и операции
    cout << "\n2. Testing basic operations:" << endl;
    Number a(20.0);
    Number b(4.0);

    cout << "a = " << a.getValue() << endl;
    cout << "b = " << b.getValue() << endl;

    Number sum = a + b;
    Number diff = a - b;
    Number product = a * b;
    Number quotient = a / b;

    cout << "a + b = " << sum.getValue() << endl;
    cout << "a - b = " << diff.getValue() << endl;
    cout << "a * b = " << product.getValue() << endl;
    cout << "a / b = " << quotient.getValue() << endl;

    // Тест 3: С глобальными переменными
    cout << "\n3. Testing with global variables:" << endl;
    Number test1 = a + ONE;
    Number test2 = b * ZERO;

    cout << "a + ONE = " << test1.getValue() << endl;
    cout << "b * ZERO = " << test2.getValue() << endl;

    // Тест 4: Динамическое создание
    cout << "\n4. Testing dynamic creation:" << endl;
    Number* dynamicNum = createNumber(8.5);
    cout << "Dynamic number = " << dynamicNum->getValue() << endl;

    // Комбинированная операция
    cout << "\n5. Combined operation:" << endl;
    Number result = (a + ONE) * (b - ZERO);
    cout << "(a + ONE) * (b - ZERO) = " << result.getValue() << endl;

    // Очистка
    destroyNumber(dynamicNum);

    cout << "\n=== All tests completed! ===" << endl;

    // Пауза для Windows
    system("pause");
    return 0;
}