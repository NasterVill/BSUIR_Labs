#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DrawingArea.h"

#include <QActionGroup>
#include <QMainWindow>
#include <QScrollArea>
#include <QUndoStack>
#include <QList>

namespace Draw
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void open();
    void save();
    void createNew();

    void penColor();
    void penWidth();

    void flipHorizontal();
    void flipVertical();
    void rotateLeft();
    void rotateRight();

    void changeSize();
    void filtrate();
    void correction();

    void about();

private:
    void createDrawActionGruop();
    void createEffectsActionGruop();
    void createActions();
    void createMenus();
    QToolBar* createToolBar();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);

    QMenu *_saveAsMenu;
    QMenu *_fileMenu;
    QMenu *_doRedoMenu;
    QMenu *_brushMenu;
    QMenu *_effectsMenu;
    QMenu *_addOptionsMenu;
    QMenu *_helpMenu;

    QToolBar* _drawToolBar;

    QAction *_openAct;
    QList<QAction *> _saveAsActs;
    QAction *_exitAct;
    QAction *_newAct;
    QAction *_undoAct;
    QAction *_redoAct;
    QAction *_penColorAct;
    QAction *_penWidthAct;
    QAction *_resizeAct;
    QAction *_correctionAct;
    QAction *_filtrateAct;
    QAction *_aboutAct;

    QActionGroup _drawActionGroup;
    QActionGroup _effectsActionGroup;

    QUndoStack _undoStack;

    DrawingArea *_workingSpace;
    QScrollArea *_scrollingArea;
};

} // namespace Draw

#endif // MAINWINDOW_H
