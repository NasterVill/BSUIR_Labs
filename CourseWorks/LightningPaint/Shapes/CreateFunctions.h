#ifndef CREATEFUNCTIONS_H
#define CREATEFUNCTIONS_H

#include "Shape.h"
#include "Ellipse.h"
#include "Rectangle.h"
#include "Curve.h"
#include "Line.h"
#include "FilledShape.h"

namespace Draw
{

Shape::ShapePointer createEllipse(QImage* image,
                                    const QPoint &topLeft,
                                    int penWidth,
                                    const QColor& penColor);
Shape::ShapePointer createRectangle(QImage* image,
                                    const QPoint &topLeft,
                                    int penWidth,
                                    const QColor& penColor);
Shape::ShapePointer createLine(QImage* image,
                                    const QPoint &topLeft,
                                    int penWidth,
                                    const QColor& penColor);
Shape::ShapePointer createCurve(QImage* image,
                                    const QPoint &topLeft,
                                    int penWidth,
                                    const QColor& penColor);
Shape::ShapePointer createEraser(QImage* image,
                                    const QPoint &topLeft,
                                    int penWidth,
                                    const QColor& penColor);
Shape::ShapePointer createFilledShape(QImage* image,
                                    const QPoint &topLeft,
                                    int penWidth,
                                    const QColor& penColor);

} // namespace Draw

#endif // CREATEFUNCTIONS_H
