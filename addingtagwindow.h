#ifndef ADDINGTAGWINDOW_H
#define ADDINGTAGWINDOW_H
#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

// Small window to add new general tag

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
    //Signal to notify main window that we added tag
    void TagAdded(AddingTagWindow* window);
};

#endif // ADDINGTAGWINDOW_H
