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

QList<QAction*>* MainWindow::tags = nullptr;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    menubar = this->menuBar();
    menubar->setFixedHeight(51);
    menubar->setStyleSheet("QMenuBar { background-color: rgb(50, 50, 50); }");

    label_tags = new QLabel("Tags: ",menubar);
    label_tags->setContentsMargins(0,0,0,4);
    label_tags->setStyleSheet("QLabel { font-size: 14pt; font-weight: bold; color: rgb(255, 255, 255); }");

    //Main ToolButton in which you can select, add and delete a tag
    tool_tags = new QToolButton(this);
    tool_tags->setText("  All   ");
    tool_tags->setStyleSheet("QToolButton { font-size: 12pt;}");
    tool_tags->setPopupMode(QToolButton::InstantPopup);
    menu_tags = new QMenu(tool_tags);
    tool_tags->setMenu(menu_tags);

    //Tag that shows all notes
    QAction* tagAll = new QAction("  All   ",menu_tags);
    menu_tags->addAction(tagAll);
    connect(tagAll,&QAction::triggered,this,[this,tagAll](){MainWindow::TagChanged(tagAll);});

    separatorInMenu = menu_tags->addSeparator();
    //Button that opens a AddingTagWindow to add a new tag
    addTagButton = new QAction("Add tag");
    menu_tags->addAction(addTagButton);
    connect(addTagButton, &QAction::triggered, this, &MainWindow::CreatingTag);

    //Menu with all general tags, where you can select tag to delete
    deleteTagMenu = new QMenu("Delete tag");
    menu_tags->addMenu(deleteTagMenu);

    //Button that opens Note-Window to add a new note
    button_AddNote = new QPushButton("Add",this);
    button_AddNote->setFixedSize(70,34);
    connect(button_AddNote, &QPushButton::clicked, this, &MainWindow::CreatingNote);


    //
    // Layouts: ...
    menubarLayout = new QHBoxLayout(menubar);
    QHBoxLayout* leftmenubar = new QHBoxLayout(menubar);
    QHBoxLayout* rightmenubar = new QHBoxLayout(menubar);
    menubarLayout->addLayout(leftmenubar);
    menubarLayout->addLayout(rightmenubar);

    leftmenubar->addWidget(label_tags, 0, Qt::AlignLeft);
    leftmenubar->addWidget(tool_tags, 0, Qt::AlignLeft);
    leftmenubar->addStretch();
    rightmenubar->addWidget(button_AddNote, 0, Qt::AlignRight);


    centralWidget = new QWidget(this);
    flowLayout = new FlowLayout(centralWidget, 10, 15, 20);
    centralWidget->setLayout(flowLayout);
    centralWidget->setLayout(flowLayout);
    centralWidget->setContentsMargins(10, 10, 10, 10);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(centralWidget);
    setCentralWidget(scrollArea);


    //Deserializing tags and notes from files
    //Showing tags and notes in relevant areas
    MainWindow::tags = new QList<QAction*>;
    DeserializeTags();
    ShowTags();
    notes = new QList<Note*>;
    DeserializeNotes();
    ShowNotes();
}

MainWindow::~MainWindow()
{
    delete notes;
    delete menubar;
    delete label_tags;
    delete tool_tags;
    delete menu_tags;
    delete deleteTagMenu;
    delete addTagButton;
    delete separatorInMenu;
    delete button_AddNote;
    delete centralWidget;
    delete flowLayout;
    delete menubarLayout;
    delete scrollArea;
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
            MainWindow::tags->append(newTag);
        }
        else { QMessageBox::critical(nullptr, "Error", "Error during loading tags."); return; }
    }
}
void MainWindow::ShowTags()
{
    // Loop going through all general tags
    for(QAction* tag : *MainWindow::tags)
    {
        // Checking if there is already a corresponding tag in menu_tags.
        // In case there is no corresponding tag, we are adding this tag to menu_tags and to deleteTagMenu.
        if([&](){   for(QAction* tagInMenu : menu_tags->actions())
                        if(tag->text()==tagInMenu->text())
                            return false;
                    return true;}())
            {
                QAction* tagInSelectMenu = new QAction(tag->text());
                tagInSelectMenu->setParent(menu_tags);
                menu_tags->insertAction(separatorInMenu,tagInSelectMenu);
                connect(tagInSelectMenu,&QAction::triggered,this,[this,tagInSelectMenu](){MainWindow::TagChanged(tagInSelectMenu);});

                QAction* tagInDeleteMenu = new QAction(tag->text());
                tagInDeleteMenu->setParent(deleteTagMenu);
                deleteTagMenu->addAction(tagInDeleteMenu);
                connect(tagInDeleteMenu,&QAction::triggered,this,[this,tagInDeleteMenu](){DeleteTag(tagInDeleteMenu->text());});
            }
    }
}
// Opens new window to add a new tag
void MainWindow::CreatingTag()
{
    AddingTagWindow* addingTagWindow = new AddingTagWindow();
    connect(addingTagWindow, &AddingTagWindow::TagAdded, this, &MainWindow::AddTag);
    addingTagWindow->show();
}
void MainWindow::AddTag(AddingTagWindow* newTagWindow)
{
    QString tag = newTagWindow->getTitleLineEdit()->text();

    // Checking if there is already a corresponding tag with the same name.
    if ([&](){  for (QAction* action : *MainWindow::tags)
                    if (action->text() == tag)
                        return true;
                return false;}())
    {
        QMessageBox::information(nullptr,"Error","There is already tag with the same name.");
        newTagWindow->close();
        return;
    }

    QAction* new_Tag = new QAction (tag);
    MainWindow::tags->push_back(new_Tag);
    newTagWindow->close();
    ShowTags();
    SerializeTags();
}
void MainWindow::SerializeTags()
{
    QJsonArray jsonArray;
    for (const QAction* tag : *MainWindow::tags)
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

void MainWindow::DeleteTag(const QString tagToDelete)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Deleting tag", "Are you sure you want to delete tag "+tagToDelete+"?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        for(QAction* tag : menu_tags->actions())
        {
            if(tag->text()==tagToDelete)
                menu_tags->removeAction(tag);
        }
        for(QAction* tag : deleteTagMenu->actions())
        {
            if(tag->text()==tagToDelete)
                deleteTagMenu->removeAction(tag);
        }
        for(int i = 0; i<tags->size(); i++)
        {
            if((*tags)[i]->text()==tagToDelete)
                tags->remove(i);
        }
        // Serializing list of tags (becos we deleted one, so it changes)
        SerializeTags();
        emit MainWindow::GeneralTagDeleted(tagToDelete);
    }
}
// Showing notes with selected tag
void MainWindow::TagChanged(QAction* selectedTag)
{
    // Clearing whole mainwindow from widgets
    flowLayout->clear();
    QString tagg = selectedTag->text();

    if(tagg!="  All   ") {
        //Adding notes with selected tag to layout and displaying it. Other notes hide
        for(auto& note : *notes) {
            if(note->tagsList.contains(tagg)) {
                flowLayout->addWidget(note);
                note->show();
            }
            else
                note->hide();
        }

    }// In case we select "All" in tags menu, add all widgets and show it.
    else {
        for(auto& note : *notes) {
            flowLayout->addWidget(note);
            note->show();
        }
    }
    tool_tags->setText("  "+tagg+"   ");

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

        Note* newNote = new Note(title, text, this, tags);
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
    Note* newNote = new Note(window->titleLineEdit->text(),window->textTextEdit->toPlainText(),this);
    flowLayout->addWidget(newNote);
    window->close();

    // Saving new note
    notes->push_back(newNote);
    SerializeNotes();
}
void MainWindow::SerializeNotes()
{
    QJsonArray jsonArray;
    for (const Note* note : *notes) {
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
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Deleting note", "Are you sure you want to delete note?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        notes->remove(notes->indexOf(noteToDelete));
        flowLayout->removeWidget(noteToDelete);
        noteToDelete->deleteLater();
        SerializeNotes();

    }
}







