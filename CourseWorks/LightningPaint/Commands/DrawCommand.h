#ifndef DRAWCOMMAND_H
#define DRAWCOMMAND_H

#include "Shape.h"

#include <QUndoCommand>
#include <QWidget>

namespace Draw
{

class DrawCommand : public QUndoCommand
{
public:
    virtual void undo() override;
    virtual void redo() override;

    explicit DrawCommand(QWidget *drawingArea, QImage *image,
                         Shape::ShapePointer shape);
    ~DrawCommand();

private:
    QWidget *_drawingArea;
    QImage *_image;

    QImage _undoImage;
    Shape::ShapePointer _shape;
};

} // namespace Draw

#endif // DRAWCOMMAND_H
