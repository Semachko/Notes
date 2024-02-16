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

/*  comboBox = new QComboBox(menubar);
    comboBox->setStyleSheet("QComboBox { font-size: 12pt;}");
    comboBox->addItem("All");
    //          MAKE DESERIALIZATION          //
    comboBox->insertSeparator(comboBox->count());
    comboBox->addItem("Add Tag");
    comboBox->addItem("Delete Tag");*/


    tool_tags = new QToolButton(this);
    tool_tags->setText("  All   ");
    tool_tags->setStyleSheet("QToolButton { font-size: 12pt;}");
    tool_tags->setPopupMode(QToolButton::InstantPopup);
    menu_tags = new QMenu(tool_tags);
    tool_tags->setMenu(menu_tags);

    menu_tags->addAction("All");
    separatorInMenu = menu_tags->addSeparator();
    addTagButton = new QAction("Add tag");
    menu_tags->addAction(addTagButton);
    connect(addTagButton, &QAction::triggered, this, &MainWindow::CreatingTag);

    deleteTagMenu = new QMenu("Delete tag");
    menu_tags->addMenu(deleteTagMenu);




    button_AddNote = new QPushButton("Add",this);
    button_AddNote->setFixedSize(70,34);
    connect(button_AddNote, &QPushButton::clicked, this, &MainWindow::CreatingNote);


    menubarLayout = new QHBoxLayout(menubar);
    QHBoxLayout* leftmenubar = new QHBoxLayout(menubar);
    QHBoxLayout* rightmenubar = new QHBoxLayout(menubar);
    menubarLayout->addLayout(leftmenubar);
    menubarLayout->addLayout(rightmenubar);

    leftmenubar->addWidget(label_tags, 0, Qt::AlignLeft);
    leftmenubar->addWidget(tool_tags, 0, Qt::AlignLeft);
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

    tags = new QList<QAction*>;
    DeserializeTags();
    ShowTags();

    notes = new QList<Note*>;
    DeserializeNotes();
    ShowNotes();
}


void MainWindow::DeserializeTags()
{
    QFile file("tags.json");
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(nullptr, "Error", "Failed to load tags");
        return;
    }
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if(jsonDoc.isNull() || !jsonDoc.isArray()){
        QMessageBox::critical(nullptr, "Error", "Invalid tags data format."); return; }

    QJsonArray jsonArray = jsonDoc.array();
    for (const QJsonValue& value : jsonArray) {
        if (value.isString()) {
            QAction* newTag = new QAction(value.toString());
            tags->append(newTag);
            connect(newTag,&QAction::triggered,this,[this,newTag](){MainWindow::TagChanged(newTag);});
        }
        else { QMessageBox::critical(nullptr, "Error", "Error during loading tags."); return; }
    }
}
void MainWindow::ShowTags()
{
    for(QAction* tag : *tags)
        menu_tags->insertAction(separatorInMenu,tag);

}
void MainWindow::CreatingTag()
{
    AddingTagWindow* addingTagWindow = new AddingTagWindow();
    connect(addingTagWindow, &AddingTagWindow::TagAdded, this, &MainWindow::AddTag);
    addingTagWindow->show();
}
void MainWindow::AddTag(AddingTagWindow* newTagWindow)
{
    QString tag = newTagWindow->getTitleLineEdit()->text();

    if ([&]() {
                for (QAction* action : *tags)
                    if (action->text() == tag)
                        return true;
                return false;
                }())
    {
        QMessageBox::information(nullptr,"Error","There is already tag with the same name.");
        newTagWindow->close();
        return;
    }
    QAction* new_Tag = new QAction (tag);
    tags->push_back(new_Tag);
    newTagWindow->close();
    SerializeTags();
}
void MainWindow::SerializeTags()
{
    QJsonArray jsonArray;
    for (const QAction* tag : *tags)
        jsonArray.append(tag->text());

    QJsonDocument jsonDoc(jsonArray);
    QByteArray jsonData = jsonDoc.toJson();

    QFile file("tags.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(jsonData);
        file.close();
    }
    else QMessageBox::critical(nullptr,"Error","Failed to save tags.");
}
void MainWindow::DeleteTag(const QString& tagToDelete)
{

}
void MainWindow::TagChanged(QAction* selectedTag)
{
    QString tagg = selectedTag->text();
    if(tagg!="All") {
        for(int i = 0; i<flowLayout->count(); i++) {
            Note* note = qobject_cast<Note *>(flowLayout->itemAt(i)->widget());
            if(note->tagsList.contains(tagg))
                flowLayout->itemAt(i)->widget()->show();
            else
                flowLayout->itemAt(i)->widget()->hide();
        }
    }
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

        Note* newNote = new Note(title, text,this);
        newNote->tagsList=tags;
        notes->push_back(newNote);
    }
}
void MainWindow::ShowNotes()
{
    for(int i = 0; i < notes->size() ;i++)
        flowLayout->addWidget((*notes)[i]);
}
void MainWindow::CreatingNote()
{
    AddNoteWindow *addNoteWindow = new AddNoteWindow();
    connect(addNoteWindow, &AddNoteWindow::NoteAdded, this, &MainWindow::AddingNote);
    addNoteWindow->show();
}
void MainWindow::AddingNote(AddNoteWindow* window)
{
    Note* newNote = new Note(window->getTitleLineEdit()->text(),window->getTextTextEdit()->toPlainText(),this);
    flowLayout->addWidget(newNote);
    window->close();

    notes->push_back(newNote);
    SerializeNotes();
}
void MainWindow::SerializeNotes()
{
    QJsonArray jsonArray;
    for (const auto& note : *notes) {
        QJsonObject jsonObj;
        jsonObj["title"] = note->title()->text();
        jsonObj["text"] = note->text()->toPlainText();
        jsonObj["tags"] = QJsonArray::fromStringList(note->tagsList);
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
void MainWindow::DeleteNote(Note *noteToDelete)
{
    qDebug()<<"Work.";
    notes->remove(notes->indexOf(noteToDelete));
    flowLayout->removeWidget(noteToDelete);
    noteToDelete->deleteLater();
    SerializeNotes();
}







