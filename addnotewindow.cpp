#include "addnotewindow.h"

//
// Small window to add new Note
//

AddNoteWindow::AddNoteWindow() : QWidget(){
    titleLineEdit = new QLineEdit;
    titleLineEdit->setAlignment(Qt::AlignCenter);
    titleLineEdit->setPlaceholderText("title");
    titleLineEdit->setStyleSheet("QLineEdit { background-color: white; border: 1px solid gray; border-radius: 10px; }");

    textTextEdit = new QTextEdit;
    textTextEdit->setPlaceholderText("text");
    textTextEdit->setStyleSheet("QTextEdit { background-color: white; border: 1px solid gray; border-radius: 10px; }");

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

    connect(addButton, &QPushButton::clicked, this, [this](){ emit AddNoteWindow::NoteAdded(this); });
    connect(cancelButton, &QPushButton::clicked, this, [this](){this->close();});
}

AddNoteWindow::~AddNoteWindow()
{
    delete addButton;
    delete cancelButton;
    delete titleLineEdit;
    delete textTextEdit;
}

