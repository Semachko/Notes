#include "addingtagwindow.h"

QLineEdit *AddingTagWindow::getTitleLineEdit() const {  return titleLineEdit; }

//
// Small window to add new general tag
//

AddingTagWindow::AddingTagWindow() : QWidget()
{
    titleLineEdit = new QLineEdit;
    titleLineEdit->setAlignment(Qt::AlignCenter);
    titleLineEdit->setPlaceholderText("title");
    titleLineEdit->setStyleSheet("QLineEdit { background-color: white; border: 1px solid gray; border-radius: 10px; }");

    addButton = new QPushButton("Add");
    cancelButton = new QPushButton("Cancel");

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QVBoxLayout *textLayout = new QVBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    textLayout->addWidget(titleLineEdit);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(textLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(addButton, &QPushButton::clicked, this, [this](){emit TagAdded(this);});
    connect(cancelButton, &QPushButton::clicked, this, [this](){this->close();});

}





