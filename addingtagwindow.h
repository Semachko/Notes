#ifndef ADDINGTAGWINDOW_H
#define ADDINGTAGWINDOW_H
#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

class AddingTagWindow : public QWidget
{
    Q_OBJECT
private:
    QLineEdit *titleLineEdit;
    QPushButton *addButton;
    QPushButton *cancelButton;
public:
    AddingTagWindow();
    QLineEdit *getTitleLineEdit() const;

signals:
    void TagAdded(AddingTagWindow* window);
};

#endif // ADDINGTAGWINDOW_H
