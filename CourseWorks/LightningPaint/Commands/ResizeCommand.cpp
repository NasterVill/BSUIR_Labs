#include "ResizeCommand.h"

namespace Draw
{

ResizeCommand::ResizeCommand(QWidget *drawingArea,
                             QImage *image,
                             const QSize &size) :
    _drawingArea(drawingArea), _image(image),
    _oldSize(image->size()), _newSize(size)
{
}

void ResizeCommand::undo()
{
    *this->_image = this->_undoImage.copy();
    this->_drawingArea->setGeometry(this->_image->rect());
    this->_drawingArea->update();
}

void ResizeCommand::redo()
{
    this->_undoImage = this->_image->copy();

    if (this->_image->size() == this->_newSize)
    {
        this->_drawingArea->setGeometry(this->_image->rect());
        this->_drawingArea->update();
        return;
    }

    QImage newImage(this->_newSize, QImage::Format_ARGB32);
    newImage.fill(Qt::white);
    *this->_image = this->_image->scaled(this->_newSize);
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *this->_image);
    *this->_image = newImage;

    this->_drawingArea->setGeometry(this->_image->rect());
    this->_drawingArea->update();
}

ResizeCommand::~ResizeCommand()
{
}

} // namespace Draw
