#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("SRecoder");
//    w.setWindowFlags(Qt::Window| Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint| Qt::WindowMaximizeButtonHint );
    w.show();

    return a.exec();
}
