#ifndef LINE_H
#define LINE_H

#include "Shape.h"

namespace Draw
{

class Line : public Shape
{
public:
    void draw(QPainter &painter) override;
    void update(const QPoint &toPoint) override;
    QRect rect() override;

    Line(QImage *image,
         const QPoint &eventPoint,
         int penWidth,
         const QColor& penColor);
    ~Line();

private:
    QRect _rectangle;
};

} // namespace Draw


#endif // LINE_H
