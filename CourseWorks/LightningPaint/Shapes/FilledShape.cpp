#include "FilledShape.h"
#include <queue>
#include <QPoint>
#include <vector>

namespace Draw
{

std::vector<QPoint>
FilledShape::floodFill(const QPoint &pos, const QRgb &newColor) // working version based on BFS algorithm
{
    std::vector<QPoint> modified;
    QImage image = this->_image->copy();

    QRgb oldColor = image.pixel(pos);
    if (oldColor == newColor)
    {
        return modified;
    }

    std::queue<QPoint> nodeQ;
    nodeQ.push(QPoint(pos.x(), pos.y()));

    while(!nodeQ.empty())
    {
        QPoint currNode = nodeQ.front();
        nodeQ.pop();
        if(image.pixel(currNode) == oldColor)
        {
            image.setPixel(currNode.x(),currNode.y(), newColor);
            modified.emplace_back(currNode);

            if(currNode.x() > 0)
                nodeQ.push(QPoint(currNode.x()-1, currNode.y()));
            if(currNode.x() < (image.width() - 1))
                nodeQ.push(QPoint(currNode.x()+1, currNode.y()));
            if(currNode.y() > 0)
                nodeQ.push(QPoint(currNode.x(), currNode.y()-1));
            if(currNode.y() < (image.height() - 1))
                nodeQ.push(QPoint(currNode.x(), currNode.y()+1));
        }
    }
    return modified;
}

FilledShape::FilledShape(QImage* image, const QPoint &topLeft,
                         int penWidth, const QColor &penColor) :
                         Shape(image, 1, penColor),
                         _points(FilledShape::floodFill(topLeft, penColor.rgb())),
                         _rectangle(topLeft, topLeft)
{
    foreach (const QPoint point, this->_points) // Find the containing rectangle
    {
        if (point.x() < this->_rectangle.left())
            this->_rectangle.setLeft(point.x());
        else if (point.x() > this->_rectangle.right())
            this->_rectangle.setRight(point.x());

        if (point.y() < this->_rectangle.top())
            this->_rectangle.setTop(point.y());
        else if (point.y() > this->_rectangle.bottom())
            this->_rectangle.setBottom(point.y());
    }
}

void FilledShape::draw(QPainter &painter)
{

    if (!this->_rectangle.isNull())
    {
        painter.setPen(QPen(this->getPenColor(), this->getPenWidth(),
                       Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        painter.drawPoints(this->_points.data(), this->_points.size());
    }
}

QRect FilledShape::rect()
{

    const int correction = this->getPenWidth() / 2 + 2;

    QRect shapeRect = this->_rectangle.normalized();
    shapeRect = shapeRect.adjusted(-correction, -correction,
                                   +correction, +correction);

    return shapeRect;
}

void FilledShape::update(const QPoint &)
{
    // is ignored because no mouse dragging may occure
}

FilledShape::~FilledShape()
{
}

} // namespace Draw
