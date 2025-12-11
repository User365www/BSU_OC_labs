#pragma once


namespace MathLibrary
{
    class Number
    {
    private:
        double value;

    public:
        // Конструкторы
        Number();
        Number(double val);

        // Арифметические операции
        Number operator+(const Number& other) const;
        Number operator-(const Number& other) const;
        Number operator*(const Number& other) const;
        Number operator/(const Number& other) const;

        // Методы доступа
        double getValue() const;
        void setValue(double val);
    };

    // Глобальные константы
    extern Number ZERO;
    extern Number ONE;

    // Функции для работы с числами
    extern "C" Number* createNumber(double value);
    extern "C" void destroyNumber(Number* number);
}