#include "mainwindow.h"
#include <QDebug>
#include <QSize>
#include <QScrollBar>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDir>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    menubar = this->menuBar();
    menubar->setFixedHeight(51);
    menubar->setStyleSheet("QMenuBar { background-color: rgb(50, 50, 50); }");


    label_tags = new QLabel("Tags: ",menubar);
    label_tags->setStyleSheet("QLabel { font-size: 14pt; font-weight: bold; color: rgb(255, 255, 255); }");

    comboBox = new QComboBox(menubar);
    comboBox->setStyleSheet("QComboBox { font-size: 12pt;}");
    comboBox->addItem("All");
    //          MAKE DESERIALIZATION          //
    comboBox->insertSeparator(comboBox->count());
    comboBox->addItem("Add Tag");
    comboBox->addItem("Delete Tag");

    button_AddNote = new QPushButton("Add",this);
    button_AddNote->setFixedSize(70,34);


    menubarLayout = new QHBoxLayout(menubar);
    QHBoxLayout* leftmenubar = new QHBoxLayout(menubar);
    QHBoxLayout* rightmenubar = new QHBoxLayout(menubar);
    menubarLayout->addLayout(leftmenubar);
    menubarLayout->addLayout(rightmenubar);

    leftmenubar->addWidget(label_tags, 0, Qt::AlignLeft);
    leftmenubar->addWidget(comboBox, 0, Qt::AlignLeft);
    leftmenubar->addStretch();
    rightmenubar->addWidget(button_AddNote, 0, Qt::AlignRight);

    //menu = new QMenu("Tags",this);
    //menubar->addMenu(menu);

    //addTag = new QAction("Add tag", this);
    //menu->addAction(addTag);


    QWidget *centralWidget = new QWidget(this);
    flowLayout = new FlowLayout(centralWidget, 10, 15, 20);
    centralWidget->setLayout(flowLayout);
    centralWidget->setContentsMargins(10, 10, 10, 10);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(centralWidget);
    setCentralWidget(scrollArea);


    connect(button_AddNote, &QPushButton::clicked, this, &MainWindow::CreatingNote);
}

void MainWindow::CreatingNote()
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

    SerializeNote(newNote);
}

void MainWindow::SerializeNote(const Note *newnote)
{
    notes.push_back(*newnote);
    QJsonArray jsonArray;
    for (const auto& note : notes) {
        QJsonObject jsonObj;
        jsonObj["title"] = note.title()->text();
        jsonObj["text"] = note.text()->toPlainText();
        jsonObj["tags"] = QJsonArray::fromStringList(note.getTags());
        jsonArray.append(jsonObj);
    }
    QJsonDocument jsonDoc(jsonArray);
    QByteArray jsonData = jsonDoc.toJson();
    QFile file("data.json");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(jsonData);
        file.close();
    }
    else qDebug()<<"error!";
}

void MainWindow::currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
        break;
    }
}


void MainWindow::AddingTag()
{

}
void MainWindow::DeletingTag()
{

}



