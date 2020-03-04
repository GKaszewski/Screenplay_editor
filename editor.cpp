#include "editor.hpp"
#include "./ui_editor.h"

#include <QFileDialog>
#include <QStandardPaths>

Editor::Editor(QWidget *parent)  : QMainWindow(parent) , ui(new Ui::Editor) {
    ui->setupUi(this);
    textArea = ui->textEdit;
}

Editor::~Editor() {
    delete ui;
}

void Editor::on_actionNew_triggered() {
    textArea->clear();
}

void Editor::on_actionOpen_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open document"), QStandardPaths::writableLocation(QStandardPaths::StandardLocation::DocumentsLocation), tr("Text files (*.txt); All files (*)"));
    if(filename.isEmpty()) {
        statusBar()->showMessage(tr("Could not open a file!"));
        return;
    }else {
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly)) {
            statusBar()->showMessage("Unable to open a file!");
            return;
        }

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_14);
        textArea->clear();
        QString data;
        in >> data;
        textArea->setText(data);
    }

}

void Editor::on_actionSave_triggered() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save document"), QStandardPaths::writableLocation(QStandardPaths::StandardLocation::DocumentsLocation), tr("Text files (*.txt); All files (*)"));
    if(filename.isEmpty()) return;
    else {
        QFile file(filename);
        if(!file.open(QIODevice::WriteOnly)) {
            statusBar()->showMessage("Couldn't save the file!");
            return;
        }

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_14);
        out << textArea->toPlainText();
    }
}

void Editor::on_actionCut_triggered() {
    textArea->cut();
}

void Editor::on_actionCopy_triggered() {
    textArea->copy();
}

void Editor::on_actionPaste_triggered() {
    textArea->paste();
}

void Editor::on_actionUndo_triggered() {
    textArea->undo();
}

void Editor::on_actionRedo_triggered() {
    textArea->redo();
}
