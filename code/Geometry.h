#pragma once

#include <ostream>
#include <ios>

class Point
{
public:
    int abscissa;
    int ordinate;
    explicit Point (int abscissa = 0, int ordinate = 0) noexcept;
    bool operator< (const Point&) const noexcept;
    bool is_in_circle (const Point &center = Point (), int radius = 1) const noexcept;
    friend std::ostream& operator<< (std::ostream &out, const Point &point) noexcept;
};

class Size
{
public:
    int length;
    int width;
    explicit Size (int length = 50, int width = 50) noexcept;
    int area () const noexcept;
    friend std::ostream& operator<< (std::ostream &out, const Size &size) noexcept;
};

int max (int op1, int op2) noexcept;
int min (int op1, int op2) noexcept;