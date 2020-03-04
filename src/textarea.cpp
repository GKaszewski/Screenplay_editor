#include "textarea.hpp"

#include <QKeyEvent>
#include <QAbstractItemView>
#include <QScrollBar>

TextArea::TextArea(QWidget * parent) : QTextEdit(parent) {

}

TextArea::~TextArea() {
    delete c;
}

void TextArea::setCompleter(QCompleter *c) {
    if(this->c)
        this->c->disconnect(this);

    this->c = c;

    if(!this->c)
            return;

    this->c->setWidget(this);
    this->c->setCompletionMode(QCompleter::CompletionMode::PopupCompletion);
    this->c->setCaseSensitivity(Qt::CaseInsensitive);

    QObject::connect(this->c, QOverload<const QString&>::of(&QCompleter::activated), this, &TextArea::insertCompletion);
}

QCompleter *TextArea::completer() const{
    return c;
}

void TextArea::insertCompletion(const QString& completion) {
    if(c->widget() != this) return;

    auto cursor = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    cursor.movePosition(QTextCursor::Left);
    cursor.movePosition(QTextCursor::EndOfWord);
    cursor.insertText(completion.right(extra));
    setTextCursor(cursor);
}

QString TextArea::textUnderCursor() const {
    auto cursor = textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    return cursor.selectedText();
}

void TextArea::focusInEvent(QFocusEvent *e) {
    if (c)
        c->setWidget(this);
    QTextEdit::focusInEvent(e);
}

void TextArea::keyPressEvent(QKeyEvent *e)  {
    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

    const bool isShortcut = (e->modifiers().testFlag(Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut) // do not process the shortcut when we have a completer
        QTextEdit::keyPressEvent(e);
    const bool ctrlOrShift = e->modifiers().testFlag(Qt::ControlModifier) ||
                                 e->modifiers().testFlag(Qt::ShiftModifier);
        if (!c || (ctrlOrShift && e->text().isEmpty()))
            return;

        static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
        const bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
        QString completionPrefix = textUnderCursor();

        if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                          || eow.contains(e->text().right(1)))) {
            c->popup()->hide();
            return;
        }

        if (completionPrefix != c->completionPrefix()) {
            c->setCompletionPrefix(completionPrefix);
            c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
        }
        QRect cr = cursorRect();
        cr.setWidth(c->popup()->sizeHintForColumn(0)
                    + c->popup()->verticalScrollBar()->sizeHint().width());
        c->complete(cr); // popup it up!
}

