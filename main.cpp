#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;

    window.resize(900,650);
    window.show();
    return a.exec();
}

