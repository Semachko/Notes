#include "note.h"
#include <QPalette>
#include <QPainter>

QList<QString> Note::alltags = {};

QList<QString> Note::getTags() const {return tags;}
QLabel *Note::title() const {return m_title;}
QTextBrowser *Note::text() const {return m_text;}

Note::Note(const QString title, const QString text, QWidget *parent) : QWidget(parent)
{
    m_title = new QLabel(title,this);
    m_title->setContentsMargins(15,0,0,5);
    m_title->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_title->setStyleSheet("color: white; font-size: 14pt; font-weight: bold;");

    m_text = new QTextBrowser(this);
    m_text->setPlainText(text);
    m_text->setStyleSheet("color: white; font-size: 11pt; background-color: rgb(60, 60, 60); border: none;");


    button_delete = new QPushButton("Delete",this);

    button_tags = new QToolButton(this);
    button_tags->setText("      Tags        ");
    button_tags->setFixedHeight(24);
    QMenu *tagsMenu = new QMenu(this);
    button_tags->setMenu(tagsMenu);
    button_tags->setPopupMode(QToolButton::InstantPopup);

    // ADD DESERIALIZE //

    //================//



    tagsMenu->addSeparator();
    QMenu* addTagMenu = new QMenu();
    QMenu* deleteTagMenu = new QMenu();
    QAction *addAction = tagsMenu->addAction("Add tag");
    QAction *deleteAction = tagsMenu->addAction("Delete tag");

    addAction->setMenu(addTagMenu);
    deleteAction->setMenu(deleteTagMenu);







    QHBoxLayout* layout_bottom = new QHBoxLayout();
    layout_bottom->addWidget(button_tags);
    layout_bottom->addStretch();
    layout_bottom->addWidget(button_delete);


    layout = new QVBoxLayout(this);
    layout->addWidget(m_title);
    layout->addWidget(m_text);
    layout->addItem(layout_bottom);
    layout->setContentsMargins(15, 15, 15, 15);

    setMaximumWidth(270);
    setMinimumHeight(330);
}

Note::Note(const Note &other, QWidget *parent) :QWidget(parent)
{
    tags = other.tags;
    m_title = new QLabel(other.m_title->text(), this);
    m_title->setContentsMargins(15,0,0,5);
    m_title->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_title->setStyleSheet("color: white; font-size: 14pt; font-weight: bold;");

    m_text = new QTextBrowser(this);
    m_text->setPlainText(other.m_text->toPlainText());
    m_text->setStyleSheet("color: white; font-size: 11pt; background-color: rgb(60, 60, 60); border: none;");

    button_delete = new QPushButton("Delete",this);

    button_tags = new QToolButton(this);
    button_tags->setText("      Tags        ");
    button_tags->setFixedHeight(24);
    QMenu *tagsMenu = new QMenu(this);
    button_tags->setMenu(tagsMenu);
    button_tags->setPopupMode(QToolButton::InstantPopup);

    tagsMenu->addSeparator();

    QMenu* addTagMenu = new QMenu(tagsMenu);
    QMenu* deleteTagMenu = new QMenu(tagsMenu);
    for(int i=0; i<Note::alltags.size();i++)
        if(!tags.contains(alltags[i]))
            addTagMenu->addAction(tags[i]);

    for(int i=0; i<tags.size();i++)
        deleteTagMenu->addAction(tags[i]);


    QHBoxLayout* layout_bottom = new QHBoxLayout();
    layout_bottom->addWidget(button_tags);
    layout_bottom->addStretch();
    layout_bottom->addWidget(button_delete);


    layout = new QVBoxLayout(this);
    layout->addWidget(m_title);
    layout->addWidget(m_text);
    layout->addItem(layout_bottom);
    layout->setContentsMargins(15, 15, 15, 15);

    setMaximumWidth(270);
    setMinimumHeight(330);
}

Note &Note::operator=(const Note &other)
{
    if (this == &other)
        return *this;
    delete layout;
    delete m_title;
    delete m_text;
    delete button_delete;
    delete button_tags;


    tags = other.tags;
    m_title = new QLabel(other.m_title->text(), this); // Создаем новые объекты
    m_title->setContentsMargins(15,0,0,5);
    m_title->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_title->setStyleSheet("color: white; font-size: 14pt; font-weight: bold;");

    m_text = new QTextBrowser(this);
    m_text->setPlainText(other.m_text->toPlainText());
    m_text->setStyleSheet("color: white; font-size: 11pt; background-color: rgb(60, 60, 60); border: none;");

    button_delete = new QPushButton("Delete",this);

    button_tags = new QToolButton(this);
    button_tags->setText("      Tags        ");
    button_tags->setFixedHeight(24);

    QMenu *tagsMenu = new QMenu(this);
    button_tags->setMenu(tagsMenu);
    button_tags->setPopupMode(QToolButton::InstantPopup);
    tagsMenu->addSeparator();
    QMenu* addTagMenu = new QMenu(tagsMenu);
    QMenu* deleteTagMenu = new QMenu(tagsMenu);
    for(int i=0; i<alltags.size();i++)
        if(!tags.contains(alltags[i]))
            addTagMenu->addAction(tags[i]);
    for(int i=0; i<tags.size();i++)
        deleteTagMenu->addAction(tags[i]);

    QHBoxLayout* layout_bottom = new QHBoxLayout();
    layout_bottom->addWidget(button_tags);
    layout_bottom->addStretch();
    layout_bottom->addWidget(button_delete);

    layout = new QVBoxLayout(this);
    layout->addWidget(m_title);
    layout->addWidget(m_text);
    layout->addItem(layout_bottom);
    layout->setContentsMargins(15, 15, 15, 15);

    setMaximumWidth(270);
    setMinimumHeight(330);

    return *this;
}


void Note::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(60, 60, 60));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 13, 13);
    QWidget::paintEvent(event);
}
