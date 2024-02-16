#include "note.h"
#include "mainwindow.h"
#include <QPalette>
#include <QPainter>
QList<QString> Note::alltags = {};

QLabel *Note::title() const {return m_title;}
QTextBrowser *Note::text() const {return m_text;}


void Note::initWidgets(const QString title, const QString text)
{
    m_title = new QLabel(title,this);
    m_title->setContentsMargins(15,0,0,5);
    m_title->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_title->setStyleSheet("color: white; font-size: 14pt; font-weight: bold;");

    m_text = new QTextBrowser(this);
    m_text->setPlainText(text);
    m_text->setStyleSheet("color: white; font-size: 11pt; background-color: rgb(60, 60, 60); border: none;");

    button_delete = new QPushButton("Delete",this);
    connect(button_delete, &QPushButton::clicked, this, [this](){qDebug()<<"init signal"; emit Note::DeletingNote(this);});
    connect(this, &Note::DeletingNote, m_parent, &MainWindow::DeleteNote);

    button_tags = new QToolButton(this);
    button_tags->setText("      Tags        ");
    button_tags->setFixedHeight(24);
    button_tags->setPopupMode(QToolButton::InstantPopup);

    tagsMenu = new QMenu(this);
    button_tags->setMenu(tagsMenu);
    addTagMenu = new QMenu("Add tag");
    deleteTagMenu = new QMenu("Delete tag");

    tagsMenu->addAction("All")->setEnabled(false);


    QHBoxLayout* layout_bottom = new QHBoxLayout();
    layout_bottom->addWidget(button_tags);
    layout_bottom->addStretch();
    layout_bottom->addWidget(button_delete);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_title);
    layout->addWidget(m_text);
    layout->addItem(layout_bottom);
    layout->setContentsMargins(15, 15, 15, 15);

    setMaximumWidth(270);
    setMinimumHeight(330);
}

Note::Note(const QString title, const QString text, MainWindow *parent) : QWidget(parent), m_parent(parent)
{
    initWidgets(title,text);

    tagsMenu->addSeparator();
    tagsMenu->addMenu(addTagMenu);
    tagsMenu->addMenu(deleteTagMenu);
}

Note::Note(const Note &other, MainWindow *parent) : QWidget(parent)
{
    initWidgets(other.m_title->text(),other.m_text->toPlainText());

    tagsList = other.tagsList;

    for(int i=0; i<other.tagsList.size();i++)
        tagsMenu->addAction(other.tagsList[i]);

    tagsMenu->addSeparator();

    for(int i=0; i<Note::alltags.size();i++){
        if(!tagsList.contains(alltags[i]))
            addTagMenu->addAction(tagsList[i]);
    }
    for(int i=0; i<tagsList.size();i++){
        deleteTagMenu->addAction(tagsList[i]);
    }

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

    tagsMenu->addSeparator();

    for(int i=0; i<Note::alltags.size();i++){
        if(!tagsList.contains(alltags[i]))
            addTagMenu->addAction(tagsList[i]);
    }
    for(int i=0; i<tagsList.size();i++){
        deleteTagMenu->addAction(tagsList[i]);
    }


    return *this;
}

void Note::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(60, 60, 60));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 13, 13);
    QWidget::paintEvent(event);
}





