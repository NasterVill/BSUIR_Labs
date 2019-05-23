#include "Shape.h"

namespace Draw
{

Shape::Shape(QImage* image,int penWidth, const QColor& penColor):
    _image(image),
    _penWidth(penWidth),
    _penColor(penColor)
{
}

int Shape::getPenWidth()
{
    return this->_penWidth;
}

QColor Shape::getPenColor()
{
    return this->_penColor;
}

Shape::~Shape()
{
}

} // namespace Draw
