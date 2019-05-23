#ifndef CORRECTIONCOMMAND_H
#define CORRECTIONCOMMAND_H

#include <QWidget>
#include <QImage>
#include <QUndoCommand>

namespace Draw
{

class CorrectionCommand : public QUndoCommand
{
public:
    void undo() override;
    void redo() override;

     explicit CorrectionCommand(QWidget *drawingArea, QImage *image, double gamma);
     ~CorrectionCommand();

private:
    void adjust();

private:
    QWidget *_drawingArea;

    QImage *_image;
    QImage _undoImage;

    double _gamma;
};

} // namespace Draw

#endif // CORRECTIONCOMMAND_H
