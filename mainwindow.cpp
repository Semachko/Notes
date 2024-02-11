#include "mainwindow.h"
#include "note.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    menubar = this->menuBar();
    menu = new QMenu("Tags",this);
    menubar->addMenu(menu);

    addTag = new QAction("Add tag", this);
    addNote = new QAction("Add note", this);

    menu->addAction(addTag);
    menubar->addAction(addNote);


    QWidget *centralWidget = new QWidget(this);
    flowLayout = new FlowLayout(centralWidget, 10, 15, 20);
    //flowLayout->setSpacing(15);
    centralWidget->setLayout(flowLayout);
    centralWidget->setContentsMargins(10, 10, 10, 10);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(centralWidget);
    setCentralWidget(scrollArea);


    connect(addTag, &QAction::triggered, this, &MainWindow::AddingTag);
    connect(addNote, &QAction::triggered, this, &MainWindow::CreatingNote);

}
void MainWindow::AddingTag(bool checked)
{

}
void MainWindow::CreatingNote(bool checked)
{
    AddNoteWindow *addNoteWindow = new AddNoteWindow();
    connect(addNoteWindow, &AddNoteWindow::NoteAdded, this, &MainWindow::AddingNote);
    addNoteWindow->show();
}
void MainWindow::AddingNote(AddNoteWindow* window)
{
    Note* newNote = new Note(window->getTitleLineEdit()->text(),window->getTextTextEdit()->toPlainText());
    flowLayout->addWidget(newNote);
    window->close();
}
