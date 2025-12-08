#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    
    // Connect save actions to slots
    connect(actionSave, &QAction::triggered, this, &MainWindow::onSaveTriggered);
    connect(actionASave, &QAction::triggered, this, &MainWindow::onSaveAsTriggered);
}

MainWindow::~MainWindow() {}

void MainWindow::onSaveTriggered()
{
    if (currentFilePath.isEmpty()) {
        // If no file is open, treat Save as Save As
        onSaveAsTriggered();
    } else {
        // Save to current file
        saveToFile(currentFilePath);
    }
}

void MainWindow::onSaveAsTriggered()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("另存新檔"),
        QString(),
        tr("文字檔案 (*.txt);;所有檔案 (*)")
    );
    
    if (!filePath.isEmpty()) {
        if (saveToFile(filePath)) {
            currentFilePath = filePath;
        }
    }
}

bool MainWindow::saveToFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(
            this,
            tr("儲存失敗"),
            tr("無法儲存檔案：%1").arg(filePath)
        );
        return false;
    }
    
    QTextStream out(&file);
    out << textEdit->toPlainText();
    file.close();
    
    statusbar->showMessage(tr("檔案已儲存：%1").arg(filePath), 3000);
    return true;
}
