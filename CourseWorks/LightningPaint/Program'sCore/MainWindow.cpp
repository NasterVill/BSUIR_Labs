#include "mainwindow.h"
#include "CreateFunctions.h"
#include "ImageSizeException.h"

#include "DrawingArea.h"

#include <QtWidgets>

namespace Draw
{

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _drawActionGroup(this), _effectsActionGroup(this)
{
    this->_workingSpace = new DrawingArea(&this->_undoStack);
    this->_workingSpace->setMinimumSize(100,100);

    createActions();
    createMenus();
    this->_drawToolBar = createToolBar();
    addToolBar(Qt::TopToolBarArea, this->_drawToolBar);

    setWindowIcon(QIcon(":/icons/MainIcon.ico"));

    this->_scrollingArea = new QScrollArea;
    this->_scrollingArea->setWidget(this->_workingSpace);
    setCentralWidget(this->_scrollingArea);

    setWindowTitle("Lightning Paint");
    resize(500, 500);

   this->_drawActionGroup.actions().first()->trigger();
}

MainWindow::~MainWindow()
{
    delete this->_drawToolBar;

    delete this->_newAct;
    delete this->_openAct;
    delete this->_exitAct;
    delete this->_undoAct;
    delete this->_redoAct;
    delete this->_penColorAct;
    delete this->_penWidthAct;
    delete this->_resizeAct;
    delete this->_correctionAct;
    delete this->_filtrateAct;
    delete this->_aboutAct;

    foreach (QAction *action, _saveAsActs)
        delete action;

    foreach (QAction *action, _drawActionGroup.actions())
        delete action;

    foreach (QAction *action, _effectsActionGroup.actions())
        delete action;

    delete this->_saveAsMenu;
    delete this->_fileMenu;
    delete this->_doRedoMenu;
    delete this->_brushMenu;
    delete this->_effectsMenu;
    delete this->_addOptionsMenu;
    delete this->_helpMenu;

    delete this->_workingSpace;
    delete this->_scrollingArea;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();
}

void MainWindow::createNew()
{
    if (maybeSave())
        this->_workingSpace->createNewImage();
}

void MainWindow::open()
{
    if (maybeSave())
    {
        try
        {
            const QString fileName =
                QFileDialog::getOpenFileName(this, "Open File",
                                             QDir::currentPath());
            if (!fileName.isEmpty())
                this->_workingSpace->openImage(fileName);
        }
        catch (ImageSizeException &excepetion)
        {
            QString message;
            message.insert(message.size(), excepetion.what());
            message.insert(message.size(), "\nImage's resolution can't be bigger than 5000x5000");
            QMessageBox::warning(this, "Lightning Paint",
                                 message,
                                 QMessageBox::Ok);
        }
    }
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    const QColor newColor = QColorDialog::getColor(this->_workingSpace->getPenColor());
    if (newColor.isValid())
        this->_workingSpace->setPenColor(newColor);
}

void MainWindow::penWidth()
{
    bool ok;
    const int newWidth = QInputDialog::getInt(this, "Pen's options",
                                            "Select pen width:",
                                            this->_workingSpace->getPenWidth(),
                                            1, 50, 1, &ok,
                                            Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    if (ok)
        this->_workingSpace->setPenWidth(newWidth);
}

void MainWindow::flipHorizontal()
{
    this->_workingSpace->flip(true, false);
}

void MainWindow::flipVertical()
{
    this->_workingSpace->flip(false, true);
}

void MainWindow::rotateLeft()
{
    this->_workingSpace->rotate(-90.0);
}

void MainWindow::rotateRight()
{
    this->_workingSpace->rotate(90.0);
}

void MainWindow::changeSize()
{
    bool ok_1, ok_2;
    const int newWidth = QInputDialog::getInt(this, "Resize",
                                       "Input image's width:",
                                       this->_workingSpace->getImageSize().width(),
                                       50, 5000, 10, &ok_1,
                                       Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    if(ok_1)
    {
        const int newHeight = QInputDialog::getInt(this, "Resize",
                                            "Input image's height:",
                                            this->_workingSpace->getImageSize().height(),
                                            50, 5000, 10, &ok_2,
                                            Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
        if (ok_2)
        {
            const QSize newSize(newWidth, newHeight);
            this->_workingSpace->resizeIamge(newSize);
        }
    }
}

void MainWindow::correction()
{
    bool ok;
    const double gamma = QInputDialog::getDouble(this, "Gamma Correction",
                                            "Input required gamma valure",
                                            1.0,
                                            0.05, 50.0, 2, &ok,
                                            Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    if (ok)
        this->_workingSpace->gammaCorrection(gamma);
}

void MainWindow::filtrate()
{
    bool ok;
    const int ratio = QInputDialog::getInt(this, "Median Filtrating",
                                           "Select filtrating ratio:",
                                           1, 1, 20, 1, &ok,
                                           Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    if (ok)
        this->_workingSpace->medianFilter(ratio);
}

void MainWindow::about()
{
    QMessageBox::about(this, "About Lightning Paint",
               "<p>Hint: User documentation for <b>Lightning Paint</b>  application :"
               "<\p><p>"
               "*In order to change current draw operation choose <b>Brush</b> "
               "and select required action."
               "<\p><p>"
               "*In order to change pen's properties choose "
               "<b>Brush->Pen Color... or Brush->Pen Width...</b>"
               "<\p><p>"
               "*To rotate or flip the image choose  <b>Effects</b>, "
               "and select required action."
               "<\p><p>"
               "*If you want to resize the image choose <b>Advanced Image Processing</b>, "
               "and select <b>Resize...</b>."
               "<\p><p>"
               "*If you want to filtrate the image choose <b>Advanced Image Processing</b>, "
               "and select <b>Median Filtration</b>. <b>Ratio</b> is the amount of filtrating cycles,  "
               "i.e. how many times the median filter will be applied to the image"
               "<\p><p>"
               "*If you want to change image's gamma choose <b>Advanced Image Processing</b>, "
               "and select <b>Gamma Correction</b>.");
}

bool MainWindow::maybeSave()
{
    if (this->_workingSpace->isModified())
    {
       const QMessageBox::StandardButton clicked =
               QMessageBox::warning(this, "Lightning Paint",
                                    "The image has been modified.\n"
                                    "Do you want to save your changes?",
                                    QMessageBox::Save |
                                    QMessageBox::Discard |
                                    QMessageBox::Cancel);
        if (clicked == QMessageBox::Save)
            return saveFile("png");
        else if (clicked == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::createDrawActionGruop()
{
    QAction *ellipseAct = new QAction(QIcon(":/icons/ellipse.ico"), "&Ellipse");
    connect(ellipseAct, SIGNAL(triggered()), _workingSpace, SLOT(setCreateEllipse()));
    ellipseAct->setCheckable(true);
    ellipseAct->setActionGroup(&_drawActionGroup);

    QAction *rectangleAct = new QAction(QIcon(":/icons/rectangle.ico"), "&Rectangle");
    connect(rectangleAct, SIGNAL(triggered()), _workingSpace, SLOT(setCreateRectangle()));
    rectangleAct->setCheckable(true);
    rectangleAct->setActionGroup(&_drawActionGroup);

    QAction *lineAct = new QAction(QIcon(":/icons/line.ico"), "&Line");
    connect(lineAct, SIGNAL(triggered()), _workingSpace, SLOT(setCreateLine()));
    lineAct->setCheckable(true);
    lineAct->setActionGroup(&_drawActionGroup);

    QAction *curveAct = new QAction(QIcon(":/icons/curve.ico"), "&Curve");
    connect(curveAct, SIGNAL(triggered()), _workingSpace, SLOT(setCreateCurve()));
    curveAct->setCheckable(true);
    curveAct->setActionGroup(&_drawActionGroup);

    QAction *eraserAct = new QAction(QIcon(":/icons/eraser.ico"), "&Eraser");
    connect(eraserAct, SIGNAL(triggered()), _workingSpace, SLOT(setCreateEraser()));
    eraserAct->setCheckable(true);
    eraserAct->setActionGroup(&_drawActionGroup);

    QAction *fillAct = new QAction(QIcon(":/icons/fill.ico"), "&Fill");
    connect(fillAct, SIGNAL(triggered()), _workingSpace, SLOT(setCreateFilledShape()));
    fillAct->setCheckable(true);
    fillAct->setActionGroup(&_drawActionGroup);

    _drawActionGroup.setExclusive(true);
}

void MainWindow::createEffectsActionGruop()
{
    QAction *flipHorizontalAct = new QAction("Flip Horizontal");
    connect(flipHorizontalAct, SIGNAL(triggered()), this, SLOT(flipHorizontal()));
    flipHorizontalAct->setActionGroup(&_effectsActionGroup);

    QAction *flipVerticalAct = new QAction("Flip Vertical");
    connect(flipVerticalAct, SIGNAL(triggered()), this, SLOT(flipVertical()));
    flipVerticalAct->setActionGroup(&_effectsActionGroup);

    QAction *rotateLeftAct = new QAction("Rotate Left");
    connect(rotateLeftAct, SIGNAL(triggered()), this, SLOT(rotateLeft()));
    rotateLeftAct->setActionGroup(&_effectsActionGroup);

    QAction *rotateRightAct = new QAction("Rotate Right");
    connect(rotateRightAct, SIGNAL(triggered()), this, SLOT(rotateRight()));
    rotateRightAct->setActionGroup(&_effectsActionGroup);

    _effectsActionGroup.setExclusive(true);
}

void MainWindow::createActions()
{
    _newAct = new QAction("Create New...", this);
    _newAct->setShortcuts(QKeySequence::New);
    connect(_newAct, SIGNAL(triggered()), this, SLOT(createNew()));

    _openAct = new QAction("&Open...", this);
    _openAct->setShortcuts(QKeySequence::Open);
    connect(_openAct, SIGNAL(triggered()), this, SLOT(open()));

    foreach (QByteArray format, QImageWriter::supportedImageFormats())
    {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        _saveAsActs.append(action);
    }

    _exitAct = new QAction("E&xit", this);
    _exitAct->setShortcuts(QKeySequence::Quit);
    connect(_exitAct, SIGNAL(triggered()), this, SLOT(close()));

    _undoAct = _undoStack.createUndoAction(nullptr, "&Undo");
    _undoAct->setShortcut(QKeySequence::Undo);

    _redoAct = _undoStack.createRedoAction(nullptr, "&Redo");
    _redoAct->setShortcut(QKeySequence::Redo);

    _penColorAct = new QAction("&Pen Color...", this);
    connect(_penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    _penWidthAct = new QAction("Pen &Width...", this);
    connect(_penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    _resizeAct = new QAction("&Resize image...", this);
    connect(_resizeAct, SIGNAL(triggered()), this, SLOT(changeSize()));

    _correctionAct = new QAction("&Gamma Correction", this);
    connect(_correctionAct, SIGNAL(triggered()), this, SLOT(correction()));

    _filtrateAct = new QAction("&Median Filtration", this);
    connect(_filtrateAct, SIGNAL(triggered()), this, SLOT(filtrate()));

    createDrawActionGruop();

    createEffectsActionGruop();

    _aboutAct = new QAction("&About", this);
    connect(_aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

QToolBar* MainWindow::createToolBar()
{
    QToolBar* toolBar = new QToolBar("Drawing ToolBar");

    toolBar->addActions(this->_drawActionGroup.actions());

    toolBar->addSeparator();

    toolBar->addAction(this->_penWidthAct);
    toolBar->addAction(this->_penColorAct);

    toolBar->setIconSize(QSize(25,25));
    toolBar->setMovable(false);

    return toolBar;
}

void MainWindow::createMenus()
{
    _saveAsMenu = new QMenu("&Save As", this);
    foreach (QAction *action, _saveAsActs)
        _saveAsMenu->addAction(action);

    _fileMenu = new QMenu("&File", this);
    _fileMenu->addAction(_newAct);
    _fileMenu->addAction(_openAct);
    _fileMenu->addMenu(_saveAsMenu);
    _fileMenu->addSeparator();
    _fileMenu->addAction(_exitAct);

    _doRedoMenu = new QMenu("&Edit", this);
    _doRedoMenu->addAction(_undoAct);
    _doRedoMenu->addAction(_redoAct);

    _brushMenu = new QMenu("&Brush", this);
    _brushMenu->addAction(_penColorAct);
    _brushMenu->addAction(_penWidthAct);
    _brushMenu->addSeparator();
    foreach (QAction *action, _drawActionGroup.actions())
        _brushMenu->addAction(action);

    _effectsMenu = new QMenu("&Effects");
    foreach (QAction *action, _effectsActionGroup.actions())
        _effectsMenu->addAction(action);

    _addOptionsMenu = new QMenu("&Advanced Image Processing", this);
    _addOptionsMenu->addAction(_resizeAct);
    _addOptionsMenu->addAction(_correctionAct);
    _addOptionsMenu->addAction(_filtrateAct);


    _helpMenu = new QMenu("&Help", this);
    _helpMenu->addAction(_aboutAct);

    menuBar()->addMenu(_fileMenu);
    menuBar()->addMenu(_doRedoMenu);
    menuBar()->addMenu(_brushMenu);
    menuBar()->addMenu(_effectsMenu);
    menuBar()->addMenu(_addOptionsMenu);
    menuBar()->addMenu(_helpMenu);
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    const QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    const QString fileName =
            QFileDialog::getSaveFileName(this, "Save As",
                                         initialPath,
                                         tr("%1 Files (*.%2);;All Files (*)")
                                         .arg(QString::fromLatin1(
                                                  fileFormat.toUpper()))
                                         .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty())
        return false;
    else
        return this->_workingSpace->saveImage(fileName, fileFormat.constData());
}

} // namespace Draw
