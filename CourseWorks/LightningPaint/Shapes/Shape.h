#ifndef Shape_H
#define Shape_H

#include <QPainter>

namespace Draw
{

class Shape
{
public:
    typedef Shape* ShapePointer;

    virtual void draw(QPainter &painter) = 0;
    virtual void update(const QPoint &toPoint) = 0;
    virtual QRect rect() = 0;

    virtual ~Shape();

protected:
    Shape(QImage* image, int penWidth, const QColor &penColor);

    int getPenWidth();
    QColor getPenColor();

protected:
    QImage* _image;

private:
    int _penWidth;
     QColor _penColor;
};

} // namespace Draw

#endif // Shape_H
