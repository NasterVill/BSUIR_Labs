#include "FlipCommand.h"

namespace Draw
{

FlipCommand::FlipCommand(QWidget *drawingArea, QImage *image,
                         bool horizontal, bool vertical) :
    _drawingArea(drawingArea), _image(image),
    _horizontal(horizontal), _vertical(vertical)
{
}

void FlipCommand::undo()
{
    redo();
}

void FlipCommand::redo()
{
    *this->_image = this->_image->mirrored(this->_vertical, this->_horizontal);
    this->_drawingArea->update();
}

FlipCommand::~FlipCommand()
{
}

} // namespace Draw

