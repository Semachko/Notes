#ifndef NOTE_H
#define NOTE_H
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QTextBrowser>

class Note : public QWidget {
    Q_OBJECT
private:
    QLabel *m_title;
    QTextBrowser *m_text;
public:
    explicit Note(const QString title, const QString text, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
};

#endif
