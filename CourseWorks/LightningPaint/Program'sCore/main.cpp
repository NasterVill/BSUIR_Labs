#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Draw::MainWindow window;
    window.show();

    return app.exec();
}
