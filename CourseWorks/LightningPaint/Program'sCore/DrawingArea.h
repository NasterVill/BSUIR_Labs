#ifndef DRAW_H
#define DRAW_H

#include "Shape.h"

#include <QUndoStack>
#include <QWidget>

namespace Draw
{

class DrawingArea : public QWidget
{
    Q_OBJECT
public:    
    bool isModified();

    void createNewImage();

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);

    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    QColor getPenColor();
    int getPenWidth();
    QSize getImageSize();

    void resizeIamge(const QSize newSize);

    void flip(bool horizontal, bool vertical);
    void rotate(qreal deg);

    void medianFilter(const int ratio);
    void gammaCorrection(const double gamma);

    DrawingArea(QUndoStack *undoStack, QWidget *parent = 0);
    ~DrawingArea();

public slots:
    void setCreateEllipse();
    void setCreateRectangle();
    void setCreateLine();
    void setCreateCurve();
    void setCreateEraser();
    void setCreateFilledShape();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    Shape::ShapePointer(*_createShape)(QImage*, const QPoint&,
                                       int, const QColor&);
private:
    QUndoStack *_undoStack;
    QImage _image;
    int _penWidth;
    QColor _penColor;
    Shape::ShapePointer _currentShape;

    static const QSize _startSize;
    static const QSize _maxSize;
};

} // namespace Draw

#endif //DRAW_H

