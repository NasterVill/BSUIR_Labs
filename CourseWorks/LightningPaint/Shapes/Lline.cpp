#include "Line.h"

namespace Draw
{

Line::Line(QImage *image,const QPoint &topLeft, int penWidth, const QColor &penColor) :
           Shape(image, penWidth, penColor), _rectangle(topLeft, topLeft)
{
}

void Line::draw(QPainter &painter)
{
    if (!this->_rectangle.isNull())
    {
        painter.setPen(QPen(this->getPenColor(), this->getPenWidth(),
                       Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        painter.drawLine(this->_rectangle.topLeft(),
                         this->_rectangle.bottomRight());
    }
}

void Line::update(const QPoint &toPoint)
{
    this->_rectangle.setBottomRight(toPoint);
}

QRect Line::rect()
{
   const int correction = this->getPenWidth() / 2 + 2;

   QRect shapeRect = this->_rectangle.normalized();
   shapeRect = shapeRect.adjusted(-correction, -correction,
                                  +correction, +correction);

   return shapeRect;
}

Line::~Line()
{
}

} // namespace Draw
