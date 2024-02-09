#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    menubar = this->menuBar();
    menu = new QMenu("Tags",this);
    menubar->addMenu(menu);

    addTag = new QAction("Add tag", this);
    addNote = new QAction("Add note", this);

    menu->addAction(addTag);
    menubar->addAction(addNote);

    gridlayout = new QGridLayout();
    this->setLayout(gridlayout);

    connect(addTag, &QAction::triggered, this, &MainWindow::AddingTag);
    connect(addNote, &QAction::triggered, this, &MainWindow::CreatingNote);
}
void MainWindow::AddingTag(bool checked)
{
    qDebug()<<"clicked";
}
void MainWindow::CreatingNote(bool checked)
{
    AddNoteWindow *addNoteWindow = new AddNoteWindow();
    connect(addNoteWindow, &AddNoteWindow::NoteAdded, this, &MainWindow::AddingNote);
    addNoteWindow->show();
}
void MainWindow::AddingNote(AddNoteWindow* window, Note* note)
{
    window->close();
}
