#ifndef NOTE_H
#define NOTE_H
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QTextBrowser>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>
#include <QMainWindow>
#include "addnotewindow.h"
class MainWindow;

class Note : public QWidget {
    Q_OBJECT

public:
    // List of note`s tags
    QStringList tagsList;
private:
    QLabel* m_title;
    QPushButton* button_change; // Button to change text in note
    QTextBrowser* m_text;
    QPushButton* button_delete;
    QToolButton* button_tags; // ToolButton in which you can select, add and delete tag from note
    QMenu* tagsMenu;
    QAction* separatorInMenu;
    QMenu* addTagMenu;
    QMenu* deleteTagMenu;

    MainWindow* m_parent;
public:
    void initWidgets(const QString title, const QString text);
    explicit Note(const QString title, const QString text, MainWindow* parent = nullptr, const QStringList tags_List = {});

    // Copy constructor and operator=

    /*explicit Note(const Note& other, MainWindow* parent = nullptr);
    Note& operator=(const Note& other);*/

    //


    void paintEvent(QPaintEvent* event) override;
    QLabel* title() const;
    QTextBrowser* text() const;
    void showTagsToAdd();
    void showTagsToDelete();
    void AddTag(const QString& tagToAdd);
    void DeleteTag(const QString& tagToDelete);
    void OpenWindowToChangeNote();
    void ChangeNote(AddNoteWindow* window);
signals:
    // Signal to notify main window that we deleted a note
    void DeletingNote(Note* noteToDelete);
};

#endif
