#ifndef FILLEDSHAPE_H
#define FILLEDSHAPE_H

#include "Shape.h"

namespace Draw {

class FilledShape : public Shape
{
public:
     void draw(QPainter &painter) override;
     void update(const QPoint &toPoint) override;
     QRect rect() override;

     FilledShape(QImage* image,
                 const QPoint &eventPoint,
                 int penWidth,
                 const QColor& penColor);
     ~FilledShape();
private:
     std::vector<QPoint> floodFill(const QPoint &pos, const QRgb &newColor);

private:
     std::vector<QPoint> _points;
     QRect _rectangle;
};

} // namespace Draw

#endif // FILLEDSHAPE_H
