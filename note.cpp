#include "note.h"
#include <QPalette>
#include <QPainter>
Note::Note(const QString title, const QString text, QWidget *parent) : QWidget(parent)
{
    m_title = new QLabel(title);
    m_title->setContentsMargins(15,0,0,0);
    m_title->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_title->setStyleSheet("color: white; font-size: 14pt; font-weight: bold;");

    m_text = new QTextBrowser(this);
    m_text->setPlainText(text);
    m_text->setStyleSheet("color: white; font-size: 11pt; background-color: rgb(60, 60, 60); border: none;");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_title);
    layout->addWidget(m_text);
    layout->setContentsMargins(15, 15, 15, 15);

    setMaximumSize(300,400);
    setMinimumSize(100,50);
}


void Note::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(60, 60, 60));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 13, 13);
    QWidget::paintEvent(event);
}
