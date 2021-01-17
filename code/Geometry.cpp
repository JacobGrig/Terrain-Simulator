#include "geometry.h"

Point::Point (
    int abscissa,
    int ordinate
) noexcept
{
    this->abscissa = abscissa;
    this->ordinate = ordinate;
}

bool Point::operator< (
    const Point& point
) const noexcept
{
    return (this->ordinate < point.ordinate ||
            this->ordinate == point.ordinate &&
            this->abscissa < point.abscissa);
}

bool Point::is_in_circle (
    const Point &center,
    int radius
) const noexcept
{
    int x_dif = this->abscissa - center.abscissa;
    int y_dif = this->ordinate - center.ordinate;
    return (x_dif * x_dif + y_dif * y_dif < radius * radius);
}

std::ostream& operator<< (
    std::ostream &out,
    const Point &point
) noexcept
{
    try
    {
        return out << "(" << point.abscissa << ", " << point.ordinate << ")";
    }
    catch (std::ios_base::failure &)
    {
        return out;
    }
}

Size::Size (
    int length,
    int width
) noexcept
{
    this->length = length;
    this->width = width;
}

int Size::area () const noexcept
{
    return this->length * this->width;
}

std::ostream& operator<< (
    std::ostream &out,
    const Size &size
) noexcept
{
    try
    {
        return out << "length = " << size.length << ", width = " << size.width;
    }
    catch (std::ios_base::failure &)
    {
        return out;
    }
}

int max (
    int op1,
    int op2
) noexcept
{
    return (op1 > op2) ? op1 : op2;
}

int min (
    int op1,
    int op2
) noexcept
{
    return - max (- op1, - op2);
}