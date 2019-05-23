#include "DrawCommand.h"

namespace Draw
{

DrawCommand::DrawCommand(QWidget *drawingArea, QImage *image,
                           Shape::ShapePointer shape) :
    _drawingArea(drawingArea),
    _image(image),
    _undoImage(image->copy(shape->rect())),
    _shape(shape)
{
}

void DrawCommand::undo()
{
    const QRect rect = this->_shape->rect();

    QPainter painter(this->_image);
    painter.drawImage(rect, this->_undoImage);

    this->_drawingArea->update(rect);
}

void DrawCommand::redo()
{
    QPainter painter(this->_image);
    this->_shape->draw(painter);

    this->_drawingArea->update(this->_shape->rect());
}

DrawCommand::~DrawCommand()
{
    delete this->_shape;
}

} // namespace Draw
