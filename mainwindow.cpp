#include "mainwindow.h"
#include <QDebug>
#include <QSize>
#include <QScrollBar>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QMessageBox>
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
    connect(button_AddNote, &QPushButton::clicked, this, &MainWindow::CreatingNote);


    menubarLayout = new QHBoxLayout(menubar);
    QHBoxLayout* leftmenubar = new QHBoxLayout(menubar);
    QHBoxLayout* rightmenubar = new QHBoxLayout(menubar);
    menubarLayout->addLayout(leftmenubar);
    menubarLayout->addLayout(rightmenubar);

    leftmenubar->addWidget(label_tags, 0, Qt::AlignLeft);
    leftmenubar->addWidget(comboBox, 0, Qt::AlignLeft);
    leftmenubar->addStretch();
    rightmenubar->addWidget(button_AddNote, 0, Qt::AlignRight);


    QWidget *centralWidget = new QWidget(this);
    flowLayout = new FlowLayout(centralWidget, 10, 15, 20);
    centralWidget->setLayout(flowLayout);
    centralWidget->setContentsMargins(10, 10, 10, 10);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(centralWidget);
    setCentralWidget(scrollArea);

    DeserializeNotes();
    ShowNotes();
}

void MainWindow::DeserializeNotes()
{
    QFile file("data.json");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "Error", "Failed to load notes.");
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull() || !jsonDoc.isArray()) {
        QMessageBox::critical(nullptr, "Error", "Invalid notes data format."); return; }

    QJsonArray jsonArray = jsonDoc.array();

    for (const QJsonValue& val : jsonArray) {
        if (!val.isObject()) {
            QMessageBox::critical(nullptr, "Error", "Invalid notes data format.");
            return;
        }

        QJsonObject jsonObj = val.toObject();
        QString title = jsonObj["title"].toString();
        QString text = jsonObj["text"].toString();
        QJsonArray tagsArray = jsonObj["tags"].toArray();
        QStringList tags;
        for (const QJsonValue& tagVal : tagsArray) {
            if (tagVal.isString()) {
                tags.append(tagVal.toString());
            }
        }

        Note newNote(title, text);
        newNote.tagsList=tags;
        notes.push_back(newNote);
    }
}
void MainWindow::ShowNotes()
{
    for(int i = 0; i < notes.size() ;i++)
        flowLayout->addWidget(&notes[i]);
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
        jsonObj["tags"] = QJsonArray::fromStringList(note.tagsList);
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
    else QMessageBox::critical(nullptr,"Error","Failed to save note.");
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



