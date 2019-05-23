#ifndef RESIZECOMMAND_H
#define RESIZECOMMAND_H

#include "Shape.h"

#include <QUndoCommand>
#include <QWidget>

namespace Draw
{

class ResizeCommand : public QUndoCommand
{
public:
    virtual void undo() override;
    virtual void redo() override;

    explicit ResizeCommand(QWidget *drawingArea, QImage *image,
                           const QSize &size);
    ~ResizeCommand();

private:
    QWidget *_drawingArea;
    QImage *_image;

    QImage _undoImage;

    QSize _oldSize;
    QSize _newSize;
};

} // namespace Draw

#endif // RESIZECOMMAND_H
