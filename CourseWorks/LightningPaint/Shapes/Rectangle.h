#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

namespace Draw
{

class Rectangle : public Shape
{
public:    
    void draw(QPainter &painter) override;
    void update(const QPoint &toPoint) override;
    QRect rect() override;

    Rectangle(QImage *image,
              const QPoint &eventPoint,
              int penWidth,
              const QColor &penColor);
    ~Rectangle();

private:
    QRect _rectangle;
};

} // namespace Draw

#endif // RECTANGLE_H
