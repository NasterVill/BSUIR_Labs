#include "Curve.h"

namespace Draw
{

Curve::Curve(QImage *image, const QPoint &eventPoint, int penWidth, const QColor &penColor) :
                           Shape(image, penWidth, penColor)
{
    this->_mouseMove = false;
    this->_polyLine << eventPoint;
}

Curve::~Curve()
{
}

void Curve::draw(QPainter &painter)
{
    if(!this->_polyLine.isEmpty())
    {
        const QPen prevPen = painter.pen();

        painter.setPen(QPen(this->getPenColor(), this->getPenWidth(),
                       Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        if(!this->_mouseMove)
        {
            painter.drawPoint(this->_polyLine[0]);
        }
        else
        {
            painter.drawPolyline(this->_polyLine);
        }

        painter.setPen(prevPen);
    }
}

void Curve::update(const QPoint &toPoint)
{
    this->_mouseMove = true;
    this->_polyLine << toPoint;
}

QRect Curve::rect()
{
   const int correction = this->getPenWidth() / 2 + 2;

   QRect shapeRect = this->_polyLine.boundingRect();
   shapeRect = shapeRect.adjusted(-correction, -correction,
                                  +correction, +correction);

   return shapeRect;
}

} // namespace Draw
