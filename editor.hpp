#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <QMainWindow>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class Editor; }
QT_END_NAMESPACE

class Editor : public QMainWindow {
    Q_OBJECT
public:
    Editor(QWidget *parent = nullptr);
    ~Editor();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

private:
    Ui::Editor *ui;
    QTextEdit * textArea;
};
#endif // EDITOR_HPP
