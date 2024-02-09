#ifndef ADDNOTEWINDOW_H
#define ADDNOTEWINDOW_H
#include "note.h"
#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>



class AddNoteWindow : public QWidget
{
    Q_OBJECT
private:
    QLineEdit *titleLineEdit;
    QTextEdit *textTextEdit;
    QPushButton *addButton;
    QPushButton *cancelButton;
public:
    AddNoteWindow();
signals:
    void NoteAdded(AddNoteWindow* window, Note* note);
public slots:
    void callNoteAdded();
};

#endif // ADDNOTEWINDOW_H
