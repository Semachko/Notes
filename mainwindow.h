#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGridLayout>
#include <QApplication>
#include <QMenuBar>
#include <QLabel>
#include <QComboBox>
#include <QScrollArea>
#include <QList>
#include "note.h"
#include "flowlayout.h"
#include "addnotewindow.h"

class MainWindow : public QMainWindow
{
private:
    QList<Note> notes;

    QMenuBar* menubar;
    QLabel* label_tags;
    QComboBox* comboBox;
    QPushButton* button_AddNote;

    FlowLayout* flowLayout;
    QHBoxLayout* menubarLayout;
    QScrollArea* scrollArea;
public:
    MainWindow(QWidget *parent = nullptr);
public slots:
    void AddingTag();
    void DeletingTag();
    void CreatingNote();
    void AddingNote(AddNoteWindow* window);
    void currentIndexChanged(int index);

    void SerializeNote(const Note* newnote);
    //QJsonArray serializeObjects(const QList<MyClass>& objects);
};

#endif // MAINWINDOW_H
