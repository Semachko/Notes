#include "note.h"
#include "mainwindow.h"
#include <QPalette>
#include <QPainter>



QLabel *Note::title() const {return m_title;}
QTextBrowser *Note::text() const {return m_text;}


//Method to initialize constructors
void Note::initWidgets(const QString title, const QString text)
{
    m_title = new QLabel(title,this);
    m_title->setContentsMargins(15,0,0,5);
    m_title->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_title->setStyleSheet("color: white; font-size: 14pt; font-weight: bold;");

    button_change = new QPushButton("Change",this);
    button_change->setFixedWidth(60);
    connect(button_change,&QPushButton::clicked, this, &Note::OpenWindowToChangeNote);

    m_text = new QTextBrowser(this);
    m_text->setPlainText(text);
    m_text->setStyleSheet("color: white; font-size: 11pt; background-color: rgb(60, 60, 60); border: none;");

    button_delete = new QPushButton("Delete",this);
    connect(button_delete, &QPushButton::clicked, this, [this](){emit Note::DeletingNote(this);});
    connect(this, &Note::DeletingNote, m_parent, &MainWindow::DeleteNote);
    connect(m_parent, &MainWindow::GeneralTagDeleted, this, &Note::DeleteTag);

    button_tags = new QToolButton(this);
    button_tags->setText("      Tags        ");
    button_tags->setFixedHeight(24);
    button_tags->setPopupMode(QToolButton::InstantPopup);

    // Menu that containg list of notes, and option to add and delete tag from note:
    /*
    Tags:
        <tag>
        <tag>
        ...
        AddTag
        DeleteTag:
                <tag>
                <tag>
                ...
    */
    tagsMenu = new QMenu(this);
    button_tags->setMenu(tagsMenu);
    addTagMenu = new QMenu("Add tag");
    connect(addTagMenu, &QMenu::aboutToShow, this, &Note::showTagsToAdd);
    deleteTagMenu = new QMenu("Delete tag");
    connect(deleteTagMenu, &QMenu::aboutToShow, this, &Note::showTagsToDelete);

    QHBoxLayout* layout_upper = new QHBoxLayout();
    layout_upper->addWidget(m_title);
    layout_upper->addStretch();
    layout_upper->addWidget(button_change);

    QHBoxLayout* layout_bottom = new QHBoxLayout();
    layout_bottom->addWidget(button_tags);
    layout_bottom->addStretch();
    layout_bottom->addWidget(button_delete);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addItem(layout_upper);
    layout->addWidget(m_text);
    layout->addItem(layout_bottom);
    layout->setContentsMargins(15, 15, 15, 15);

    setMaximumWidth(270);
    setMinimumHeight(330);

}
Note::Note(const QString title, const QString text, MainWindow *parent, const QStringList tags_List)
    : QWidget(parent),
    tagsList(tags_List),
    m_parent(parent)
{
    //Calling method to initialize constructor
    initWidgets(title,text);

    //Filling tagsMenu with tags
    for(const QString& name_tag : tags_List) {
        QAction* newTag = new QAction(name_tag,tagsMenu);
        newTag->setEnabled(false);
        tagsMenu->insertAction(separatorInMenu,newTag);
    }
    separatorInMenu = tagsMenu->addSeparator();
    tagsMenu->addMenu(addTagMenu);
    tagsMenu->addMenu(deleteTagMenu);
}

Note::~Note()
{
    delete m_title;
    delete button_change;
    delete m_text;
    delete button_delete;
    delete button_tags;
    delete tagsMenu;
    delete separatorInMenu;
    delete addTagMenu;
    delete deleteTagMenu;
    delete m_parent;
}


// Opens window to change note`s text
void Note::OpenWindowToChangeNote()
{
    AddNoteWindow *changeNoteWindow = new AddNoteWindow();
    connect(changeNoteWindow, &AddNoteWindow::NoteAdded, this, &Note::ChangeNote);
    changeNoteWindow->titleLineEdit->setText(m_title->text());
    changeNoteWindow->textTextEdit->setText(m_text->toPlainText());
    changeNoteWindow->show();
}
void Note::ChangeNote(AddNoteWindow *window)
{
    m_title->setText(window->titleLineEdit->text());
    m_text->setText(window->textTextEdit->toPlainText());
    window->close();

    m_parent->SerializeNotes();
}
// Method to show tags that note does not have
// Tags in addTagMenu are shown dynamically
void Note::showTagsToAdd()
{
    addTagMenu->clear();
    for(const QAction* tag : *MainWindow::tags)
    {
        //Checking is tagsList variable does not contain specific tag from general tags varibale
        if(!tagsList.contains(tag->text()))
        {
            QAction* tagToAdd = new QAction(tag->text(),addTagMenu);
            addTagMenu->addAction(tagToAdd);
            connect(tagToAdd,&QAction::triggered,this,[this,tagToAdd](){AddTag(tagToAdd->text());});
        }
    }
}
// Method to show curent note`s tags in menu, so you can delete it
// Tags in deleteTagMenu are shown dynamically
void Note::showTagsToDelete()
{
    deleteTagMenu->clear();
    for(const QAction* tag : *MainWindow::tags)
    {
        if(tagsList.contains(tag->text()))
        {
            QAction* tagToDelete = new QAction(tag->text(),addTagMenu);
            deleteTagMenu->addAction(tagToDelete);
            connect(tagToDelete,&QAction::triggered,this,[this,tagToDelete](){DeleteTag(tagToDelete->text());});
        }
    }
}
// Adding tag to note
void Note::AddTag(const QString& tagToAdd)
{
    tagsList.push_back(tagToAdd);
    QAction* newTag = new QAction(tagToAdd,tagsMenu);
    newTag->setEnabled(false);
    tagsMenu->insertAction(separatorInMenu,newTag);
    m_parent->SerializeNotes();
}
// Deleting tag from note
void Note::DeleteTag(const QString& tagToDelete)
{
    int indexOfTagToDelete = tagsList.indexOf(tagToDelete);
    if(indexOfTagToDelete!=-1)
    {
        tagsList.remove(indexOfTagToDelete);
        for(QAction* action : tagsMenu->actions())
            if(action->text()==tagToDelete)
            { tagsMenu->removeAction(action); break; }
        m_parent->SerializeNotes();
    }
}

// Drowing notes
void Note::paintEvent(QPaintEvent *event)
{
    //Drowing background
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(60, 60, 60));
    painter.drawRoundedRect(rect(), 13, 13);

    // Drowing border
    painter.setPen(QPen(QColor(110, 110, 110), 5));
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 13, 13);

    QWidget::paintEvent(event);
}



/*Note::Note(const Note &other, MainWindow *parent) : QWidget(parent)
{
    initWidgets(other.m_title->text(),other.m_text->toPlainText());

    tagsList = other.tagsList;

    for(int i=0; i<other.tagsList.size();i++)
        tagsMenu->addAction(other.tagsList[i]);

    separatorInMenu = tagsMenu->addSeparator();
    tagsMenu->addMenu(addTagMenu);
    tagsMenu->addMenu(deleteTagMenu);
}
Note &Note::operator=(const Note &other)
{
    if (this == &other)
        return *this;
    delete m_title;
    delete m_text;
    delete button_delete;
    delete button_tags;
    delete tagsMenu;
    delete addTagMenu;
    delete deleteTagMenu;

    initWidgets(other.m_title->text(),other.m_text->toPlainText());

    tagsList = other.tagsList;

    for(int i=0; i<other.tagsList.size();i++)
        tagsMenu->addAction(other.tagsList[i]);

    separatorInMenu = tagsMenu->addSeparator();
    return *this;
}
*/



