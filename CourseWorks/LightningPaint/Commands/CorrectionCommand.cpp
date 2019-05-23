#include "CorrectionCommand.h"

#include <cmath>

namespace Draw
{

CorrectionCommand::CorrectionCommand(QWidget *drawingArea,QImage *image, double gamma)
    : _drawingArea(drawingArea),_image(image), _gamma(gamma)
{
   this->_gamma = (this->_gamma > 0)? this->_gamma : 1;
}

void CorrectionCommand::adjust()
{
    int height = this->_image->height();
    int width = this->_image->width();

    for (int i = 0; i < height - 1; ++i)
    {
        QRgb *line = reinterpret_cast<QRgb*>(this->_image->scanLine(i));
        for (int j = 0; j < width - 1; ++j)
        {
            QRgb pixelColor = line[j];
            int r = qRed(pixelColor);
            int g = qGreen(pixelColor);
            int b = qBlue(pixelColor);
            r = std::pow(double(r)/255, this->_gamma) * 255;
            g = std::pow(double(g)/255, this->_gamma) * 255;
            b = std::pow(double(b)/255, this->_gamma) * 255;
            line[j] = qRgb(r,g,b);
        }
    }
}

void CorrectionCommand::undo()
{
    *this->_image = this->_undoImage.copy();
    this->_drawingArea->update();
}

void CorrectionCommand::redo()
{
    this->_undoImage = this->_image->copy();
    adjust();
    this->_drawingArea->update();
}

CorrectionCommand::~CorrectionCommand()
{
}

} // namespce Draw
