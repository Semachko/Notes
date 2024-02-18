#ifndef ADDNOTEWINDOW_H
#define ADDNOTEWINDOW_H
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
    QPushButton *addButton;
    QPushButton *cancelButton;
public:
    QLineEdit *titleLineEdit;
    QTextEdit *textTextEdit;
    AddNoteWindow();
signals:
    void NoteAdded(AddNoteWindow* window);
};

#endif // ADDNOTEWINDOW_H
