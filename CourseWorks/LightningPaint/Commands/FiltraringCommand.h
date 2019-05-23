#ifndef FILTRARINGCOMMAND_H
#define FILTRARINGCOMMAND_H

#include <QWidget>
#include <QImage>
#include <QUndoCommand>

namespace Draw
{

class FiltraringCommand : public QUndoCommand
{
public:
    void undo() override;
    void redo() override;

     explicit FiltraringCommand(QWidget *drawingArea, QImage *image, const int ratio);
     ~FiltraringCommand();

private:
    int findMedian(int* Array, const int size);
    void filtrate();

private:
    QWidget *_drawingArea;

    QImage *_image;
    QImage _undoImage;

    int _ratio;
};

} // namespace Draw

#endif // FILTRARINGCOMMAND_H
