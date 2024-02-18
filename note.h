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
class MainWindow;

class Note : public QWidget {
    Q_OBJECT

public:
    QStringList tagsList;
private:
    QLabel* m_title;
    QTextBrowser* m_text;
    QPushButton* button_delete;
    QToolButton* button_tags;
    QMenu* tagsMenu;
    QAction* separatorInMenu;
    QMenu* addTagMenu;
    QMenu* deleteTagMenu;

    MainWindow* m_parent;
public:
    void initWidgets(const QString title, const QString text);
    explicit Note(const QString title, const QString text, MainWindow* parent = nullptr, const QStringList tags_List = {});
    /*explicit Note(const Note& other, MainWindow* parent = nullptr);
    Note& operator=(const Note& other);*/

    void paintEvent(QPaintEvent* event) override;
    QLabel* title() const;
    QTextBrowser* text() const;
    void showTagsToAdd();
    void showTagsToDelete();
    void AddTag(const QString& tagToAdd);
    void DeleteTag(const QString& tagToDelete);
signals:
    void DeletingNote(Note* noteToDelete);
};

#endif
