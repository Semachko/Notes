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
    static QList<QString> alltags;
private:
    QList<QString> tags;
    QVBoxLayout* layout;
    QLabel* m_title;
    QTextBrowser* m_text;
    QPushButton* button_delete;
    QToolButton* button_tags;
public:
    explicit Note(const QString title, const QString text, QWidget *parent = nullptr);
    explicit Note(const Note& other, QWidget *parent = nullptr);
    Note& operator=(const Note& other);

    void paintEvent(QPaintEvent *event) override;
    QList<QString> getTags() const;
    QLabel *title() const;
    QTextBrowser *text() const;
};

#endif
