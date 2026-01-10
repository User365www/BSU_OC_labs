#include "pch.h"
#include "Vector.h"
#include <cmath>

// Конструктор с параметрами
Vector::Vector(Number x_val, Number y_val) : x(x_val), y(y_val) {}

// Конструктор по умолчанию
Vector::Vector() {
    // Используем константу ZERO из вашего namespace MathLibrary
    // (она доступна, так как мы написали using namespace MathLibrary в хедере)
    x = ZERO;
    y = ZERO;
}

Number Vector::GetRadius() const {
    // r = sqrt(x^2 + y^2)
    Number sum_squares = (x * x) + (y * y);

    // Используем ваш метод .getValue() для получения double
    double val = std::sqrt(sum_squares.getValue());

    // Возвращаем новый объект Number через конструктор
    return Number(val);
}

Number Vector::GetAngle() const {
    // phi = atan2(y, x)
    double val_y = y.getValue();
    double val_x = x.getValue();

    double angle = std::atan2(val_y, val_x);

    return Number(angle);
}

Vector Vector::Add(const Vector& other) const {
    // Оператор + уже реализован в вашем классе Number
    return Vector(this->x + other.x, this->y + other.y);
}

// Глобальные переменные из задания
// Используем ваши константы ZERO и ONE для инициализации
const Vector VectorZero(ZERO, ZERO);
const Vector VectorOne(ONE, ONE);