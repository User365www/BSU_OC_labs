#pragma once

#ifdef VECTOR_EXPORTS
#define VECTOR_API __declspec(dllexport)
#else
#define VECTOR_API __declspec(dllimport)
#endif

// Подключаем заголовок статической библиотеки
#include "MathLibrary.h"
#include <cmath> 

// !!! ИСПРАВЛЕНИЕ: Подключаем пространство имен, чтобы "Number" стал виден
using namespace MathLibrary;

class VECTOR_API Vector {
public:
    Number x;
    Number y;

    // Конструктор по умолчанию
    Vector();
    // Конструктор с параметрами
    Vector(Number x_val, Number y_val);

    Number GetRadius() const;
    Number GetAngle() const;
    Vector Add(const Vector& other) const;
};

extern VECTOR_API const Vector VectorZero;
extern VECTOR_API const Vector VectorOne;