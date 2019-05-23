#ifndef ROTATECOMMAND_H
#define ROTATECOMMAND_H

#include "Shape.h"

#include <QUndoCommand>
#include <QWidget>

namespace Draw
{

class RotateCommand : public QUndoCommand
{
public:
    virtual void undo() override;
    virtual void redo() override;

    explicit RotateCommand(QWidget *drawingArea, QImage *image, qreal deg);
    ~RotateCommand();

private:
    void rotate(qreal deg);
private:
    QWidget *_drawingArea;
    QImage *_image;

    qreal _deg;
};

} // namespace Draw

#endif // ROTATECOMMAND_H
