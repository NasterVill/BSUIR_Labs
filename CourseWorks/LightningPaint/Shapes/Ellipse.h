#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "Shape.h"

namespace Draw
{

class Ellipse : public Shape
{
public:
    void draw(QPainter &painter) override;
    void update(const QPoint &toPoint) override;
    QRect rect() override;

    Ellipse(QImage *image,
            const QPoint &eventPoint,
            int penWidth,
            const QColor& penColor);
    ~Ellipse();

private:
    QRect _rectangle;
};

} // namespace Draw

#endif // ELLIPSE_H
