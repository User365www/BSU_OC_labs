#include "MathLibrary.h"
#include <stdexcept>

namespace MathLibrary
{
    // Реализация конструкторов
    Number::Number() : value(0.0) {}
    Number::Number(double val) : value(val) {}

    // Реализация операций
    Number Number::operator+(const Number& other) const {
        return Number(value + other.value);
    }

    Number Number::operator-(const Number& other) const {
        return Number(value - other.value);
    }

    Number Number::operator*(const Number& other) const {
        return Number(value * other.value);
    }

    Number Number::operator/(const Number& other) const {
        if (other.value == 0.0) {
            throw std::runtime_error("Division by zero!");
        }
        return Number(value / other.value);
    }

    // Реализация методов доступа
    double Number::getValue() const {
        return value;
    }

    void Number::setValue(double val) {
        value = val;
    }

    // Инициализация глобальных переменных
    Number ZERO(0.0);
    Number ONE(1.0);

    // Реализация функций создания/удаления
    Number* createNumber(double value) {
        return new Number(value);
    }

    void destroyNumber(Number* number) {
        delete number;
    }
}