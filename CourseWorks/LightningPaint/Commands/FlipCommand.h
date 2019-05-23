#ifndef FLIPCOMMAND_H
#define FLIPCOMMAND_H

#include "Shape.h"

#include <QUndoCommand>
#include <QWidget>

namespace Draw
{

class FlipCommand : public QUndoCommand
{
public:
    virtual void undo() override;
    virtual void redo() override;

    explicit FlipCommand(QWidget *drawingArea, QImage *image,
                         bool horizontal, bool vertical);
    ~FlipCommand();

private:
    QWidget *_drawingArea;
    QImage *_image;

    bool _horizontal;
    bool _vertical;
};

} // namespace Draw

#endif // FLIPCOMMAND_H
