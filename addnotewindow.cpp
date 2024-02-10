#include "addnotewindow.h"


AddNoteWindow::AddNoteWindow() : QWidget(nullptr){
    titleLineEdit = new QLineEdit;
    titleLineEdit->setAlignment(Qt::AlignCenter);
    textTextEdit = new QTextEdit;
    addButton = new QPushButton("Add");
    cancelButton = new QPushButton("Cancel");

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QVBoxLayout *textLayout = new QVBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    textLayout->addWidget(titleLineEdit);
    textLayout->addWidget(textTextEdit);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(textLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);


    connect(addButton, &QPushButton::clicked, this, &AddNoteWindow::callNoteAdded);
    connect(cancelButton, &QPushButton::clicked, this, [this](){this->close();});
}

void AddNoteWindow::callNoteAdded() {  emit AddNoteWindow::NoteAdded(this); }
QLineEdit *AddNoteWindow::getTitleLineEdit() const { return titleLineEdit; }
QTextEdit *AddNoteWindow::getTextTextEdit() const { return textTextEdit; }
