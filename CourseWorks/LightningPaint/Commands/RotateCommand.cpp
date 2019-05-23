#include "RotateCommand.h"

namespace Draw
{

RotateCommand::RotateCommand(QWidget *drawingArea, QImage *image, qreal deg) :
    _drawingArea(drawingArea), _image(image), _deg(deg)
{
}

void RotateCommand::rotate(qreal deg)
{
    QMatrix transf;
    *this->_image = this->_image->transformed(transf.rotate(deg));

    this->_drawingArea->setGeometry(this->_image->rect());
    this->_drawingArea->update();
}

void RotateCommand::undo()
{
    rotate(-this->_deg);
}

void RotateCommand::redo()
{
    rotate(this->_deg);
}

RotateCommand::~RotateCommand()
{
}

} // namespace Draw
