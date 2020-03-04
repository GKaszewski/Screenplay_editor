#ifndef TEXTAREA_HPP
#define TEXTAREA_HPP

#include <QWidget>
#include <QCompleter>
#include <QTextEdit>

class TextArea : public QTextEdit {
    Q_OBJECT

public:
    TextArea(QWidget* parent = nullptr);
    ~TextArea();

    void setCompleter(QCompleter * c);
    QCompleter * completer() const;

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;

private slots:
    void insertCompletion(const QString & completion);

private:
    QString textUnderCursor() const;

    QCompleter* c = nullptr;
};

#endif // TEXTAREA_HPP
