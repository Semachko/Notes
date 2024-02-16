#ifndef NOTE_H
#define NOTE_H
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QTextBrowser>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>
class Note : public QWidget {
    Q_OBJECT

public:
    static QStringList alltags;

    QStringList tagsList;
private:
    QLabel* m_title;
    QTextBrowser* m_text;
    QPushButton* button_delete;
    QToolButton* button_tags;
    QMenu* tagsMenu;
    QMenu* addTagMenu;
    QMenu* deleteTagMenu;

public:
    void initWidgets(const QString title, const QString text);
    explicit Note(const QString title, const QString text, QWidget *parent = nullptr);
    explicit Note(const Note& other, QWidget *parent = nullptr);
    Note& operator=(const Note& other);

    void paintEvent(QPaintEvent *event) override;
    QLabel *title() const;
    QTextBrowser *text() const;
};

#endif
