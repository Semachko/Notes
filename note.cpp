#include "note.h"

Note::Note(const QString title, const QString text, QWidget *parent) : QWidget(parent)
{
    m_title = new QLabel(title);
    m_text = new QTextBrowser(this);
    m_text->setPlainText(text);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_title);
    layout->addWidget(m_text);
    setLayout(layout);
}
