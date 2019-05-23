#ifndef Curve_H
#define Curve_H

#include "Shape.h"

namespace Draw
{

class Curve : public Shape
{
public:
    void draw(QPainter &painter) override;
    void update(const QPoint &toPoint) override;
    QRect rect() override;

    Curve(QImage *image,
          const QPoint &eventPoint,
          int penWidth,
          const QColor& penColor);
    ~Curve();

private:
    QPolygon _polyLine;
    bool _mouseMove;
};

} // namespace Draw
#endif // Curve_H
