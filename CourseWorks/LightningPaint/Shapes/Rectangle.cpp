#include "Rectangle.h"

namespace Draw
{

Rectangle::Rectangle(QImage *image,const QPoint &topLeft,
                     int penWidth, const QColor &penColor) :
                     Shape(image ,penWidth, penColor), _rectangle(topLeft, topLeft)
{
}

Rectangle::~Rectangle()
{
}

void Rectangle::draw(QPainter &painter)
{
    if (!this->_rectangle.isNull())
    {
        painter.setPen(QPen(this->getPenColor(), this->getPenWidth(),
                       Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        painter.drawRect(this->_rectangle);
    }
}

void Rectangle::update(const QPoint &toPoint)
{
    this->_rectangle.setBottomRight(toPoint);
}

QRect Rectangle::rect()
{
   const int correction = this->getPenWidth() / 2 + 2;

   QRect shapeRect = this->_rectangle.normalized();
   shapeRect = shapeRect.adjusted(-correction, -correction,
                                  +correction, +correction);

   return shapeRect;
}

} // namespace Draw
