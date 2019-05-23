#include "DrawingArea.h"
#include "CreateFunctions.h"

#include "ImageSizeException.h"

#include "DrawCommand.h"
#include "ResizeCommand.h"
#include "FlipCommand.h"
#include "RotateCommand.h"
#include "CorrectionCommand.h"
#include "FiltraringCommand.h"

#include <qevent.h>
#include <QPainter>

namespace Draw
{

const QSize DrawingArea::_startSize(1280,720);
const QSize DrawingArea::_maxSize(5000,5000);

DrawingArea::DrawingArea(QUndoStack *undoStack, QWidget *parent) :
                         QWidget(parent),
                         _undoStack(undoStack),
                         _penWidth(1),
                         _penColor(Qt::black),
                         _currentShape(nullptr)
{
    ResizeCommand(this, &this->_image, _startSize).redo();
}

DrawingArea::~DrawingArea()
{
}

bool DrawingArea::isModified()
{
    return !(this->_undoStack->isClean());
}

void DrawingArea::resizeIamge(const QSize newSize)
{
    this->_undoStack->push(new ResizeCommand(this, &this->_image,
                                             newSize));
}

void DrawingArea::flip(bool horizontal, bool vertical)
{
    this->_undoStack->push(new FlipCommand(this, &this->_image,
                                           horizontal, vertical));
}

void DrawingArea::rotate(qreal deg)
{
    this->_undoStack->push(new RotateCommand(this, &this->_image,
                                             deg));
}

void DrawingArea::medianFilter(const int ratio)
{
    this->_undoStack->push(new FiltraringCommand(this, &this->_image,
                                                 ratio));
}

void DrawingArea::gammaCorrection(const double gamma)
{
    this->_undoStack->push(new CorrectionCommand(this, &this->_image,
                                                 gamma));
}

void DrawingArea::createNewImage()
{
    ResizeCommand(this, &this->_image, _startSize).redo();
    this->_image.fill(Qt::white);

    this->_undoStack->clear();
}

bool DrawingArea::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    const QSize newSize = loadedImage.size();

    if(newSize.width() > this->_maxSize.width() ||
       newSize.height() > this->_maxSize.height())
        throw ImageSizeException("Bad image resolution!");

    this->_image = loadedImage.convertToFormat(QImage::Format_ARGB32);

    ResizeCommand(this, &this->_image, newSize).redo();
    this->_undoStack->clear();

    return true;
}

bool DrawingArea::saveImage(const QString &fileName, const char *fileFormat)
{
    if (this->_image.save(fileName, fileFormat))
    {
        this->_undoStack->clear();
        return true;
    }
    else
        return false;
}

void DrawingArea::setPenColor(const QColor &newColor)
{
    this->_penColor = newColor;
}

void DrawingArea::setPenWidth(int newWidth)
{
    this->_penWidth = newWidth;
}

void DrawingArea::setCreateEllipse()
{
    this->_createShape = &createEllipse;
}

void DrawingArea::setCreateRectangle()
{
    this->_createShape = &createRectangle;
}

void DrawingArea::setCreateLine()
{
    this->_createShape = &createLine;
}

void DrawingArea::setCreateCurve()
{
    this->_createShape = &createCurve;
}

void DrawingArea::setCreateEraser()
{
    this->_createShape = &createEraser;
}

void DrawingArea::setCreateFilledShape()
{
    this->_createShape = &createFilledShape;
}

QColor DrawingArea::getPenColor()
{
    return this->_penColor;
}

int DrawingArea::getPenWidth()
{
     return this->_penWidth;
}

QSize DrawingArea::getImageSize()
{
 return this->_image.size();
}

void DrawingArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->_currentShape = _createShape(&this->_image,
                                           event->pos(),
                                           this->_penWidth,
                                           this->_penColor);
        update(this->_currentShape->rect());
    }
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && this->_currentShape)
    {

        const QRect prevRect = this->_currentShape->rect();
        this->_currentShape->update(event->pos());

        update(this->_currentShape->rect().united(prevRect));
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && this->_currentShape)
    {
        this->_undoStack->push(new DrawCommand(this, &this->_image,
                                                this->_currentShape));
        this->_currentShape = nullptr;
    }
}

void DrawingArea::paintEvent(QPaintEvent *event)
{
    const QRect paintRect = event->rect();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.drawImage(paintRect, this->_image, paintRect);

    if (this->_currentShape)
        this->_currentShape->draw(painter);
}

} // namespace Draw
