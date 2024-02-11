#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGridLayout>
#include <QApplication>
#include <QMenuBar>
#include <QScrollArea>
#include "flowlayout.h"
#include "addnotewindow.h"

class MainWindow : public QMainWindow
{
private:
    QScrollArea *scrollArea;
    FlowLayout *flowLayout;
    QMenuBar *menubar;
    QMenu *menu;
    QAction *addTag;
    QAction *addNote;
public:
    MainWindow(QWidget *parent = nullptr);
public slots:
    void AddingTag(bool checked);
    void CreatingNote(bool checked);
    void AddingNote(AddNoteWindow* window);
};

#endif // MAINWINDOW_H
