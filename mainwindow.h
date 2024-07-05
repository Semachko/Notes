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
#include "addingtagwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    // Static variable contains all general tags
    static QList<QAction*>* tags;

private:
    // Varibale contains all notes
    QList<Note*>* notes;


    // Top Menu Bar contains list of tags and button to add a note.
    // In list of tags you can choose specific tag, add tag and delete tag.
    QMenuBar* menubar;
    QLabel* label_tags;
    QToolButton* tool_tags;
    QMenu* menu_tags;
    QMenu* deleteTagMenu;
    QAction* addTagButton;
    QAction* separatorInMenu;
    QPushButton* button_AddNote;


    QWidget* centralWidget;
    FlowLayout* flowLayout;
    QHBoxLayout* menubarLayout;
    //ScrollArea, so you can scroll window if there are a lot of notes
    QScrollArea* scrollArea;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void CreatingTag();
    void AddTag(AddingTagWindow* newTag);
    void DeleteTag(const QString tagToDelete);
    void TagChanged(QAction* selectedTag);

    void CreatingNote();
    void AddingNote(AddNoteWindow* window);
    void DeleteNote(Note* noteToDelete);

    void SerializeTags();
    void DeserializeTags();
    void ShowTags();

    void SerializeNotes();
    void DeserializeNotes();
    void ShowNotes();

signals:
    //Signal to notify all notes that a specific tag has been deleted
    void GeneralTagDeleted(const QString& deletedTag);
};

#endif // MAINWINDOW_H
