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
    static QList<QAction*>* tags;

private:
    QList<Note*>* notes;

    QMenuBar* menubar;
    QLabel* label_tags;
    QToolButton* tool_tags;
    QMenu* menu_tags;
    QMenu* deleteTagMenu;
    QAction* addTagButton;
    QAction* separatorInMenu;
    QPushButton* button_AddNote;

    FlowLayout* flowLayout;
    QHBoxLayout* menubarLayout;
    QScrollArea* scrollArea;
public:
    MainWindow(QWidget *parent = nullptr);

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
    void GeneralTagDeleted(const QString& deletedTag);
};

#endif // MAINWINDOW_H
