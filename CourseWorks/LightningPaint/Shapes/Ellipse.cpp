#include "Ellipse.h"

namespace Draw
{

Ellipse::Ellipse(QImage *image, const QPoint &eventPoint, int penWidth, const QColor &penColor) :
                           Shape(image, penWidth, penColor), _rectangle(eventPoint, eventPoint)
{
}

Ellipse::~Ellipse()
{
}

void Ellipse::draw(QPainter &painter)
{
    if (!this->_rectangle.isNull())
    {
        const QPen prevPen = painter.pen();

        painter.setPen(QPen(this->getPenColor(), this->getPenWidth(),
                       Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        painter.drawEllipse(this->_rectangle);
        painter.setPen(prevPen);
    }
}

void Ellipse::update(const QPoint &toPoint)
{
    this->_rectangle.setBottomRight(toPoint);
}

QRect Ellipse::rect()
{
   const int correction = this->getPenWidth() / 2 + 2;

   QRect shapeRect = this->_rectangle.normalized();
   shapeRect = shapeRect.adjusted(-correction, -correction,
                                  +correction, +correction);

   return shapeRect;
}

} // namespace Draw

