#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;

    window.resize(950,600);
    window.show();
    return a.exec();
}

