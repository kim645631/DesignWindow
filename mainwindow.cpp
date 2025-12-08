#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QClipboard>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentFilePath(""), isModified(false)
{
    setupUi(this);
    
    // Connect File menu actions
    connect(actionNew, &QAction::triggered, this, &MainWindow::onNew);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::onOpen);
    connect(actionClose, &QAction::triggered, this, &MainWindow::onClose);
    connect(actionSave, &QAction::triggered, this, &MainWindow::onSave);
    connect(actionASave, &QAction::triggered, this, &MainWindow::onSaveAs);
    connect(actionQuit, &QAction::triggered, this, &MainWindow::onQuit);
    
    // Connect Edit menu actions
    connect(actionUndo, &QAction::triggered, this, &MainWindow::onUndo);
    connect(actionCuT, &QAction::triggered, this, &MainWindow::onCut);
    connect(actionCopy, &QAction::triggered, this, &MainWindow::onCopy);
    connect(actionPaste, &QAction::triggered, this, &MainWindow::onPaste);
    connect(actionAll, &QAction::triggered, this, &MainWindow::onSelectAll);
    
    // Connect Debug menu actions
    connect(actionStartDebug, &QAction::triggered, this, &MainWindow::onStartDebug);
    connect(actionBreak, &QAction::triggered, this, &MainWindow::onBreak);
    connect(actionContinue, &QAction::triggered, this, &MainWindow::onContinue);
    
    // Track text changes
    connect(textEdit, &QTextEdit::textChanged, this, [this]() {
        isModified = true;
        updateWindowTitle();
    });
    
    updateWindowTitle();
}

MainWindow::~MainWindow() {}

void MainWindow::updateWindowTitle() {
    QString title = "DesignWindow";
    if (!currentFilePath.isEmpty()) {
        QFileInfo fileInfo(currentFilePath);
        title = fileInfo.fileName();
    } else {
        title = "未命名";
    }
    if (isModified) {
        title += " *";
    }
    title += " - DesignWindow";
    setWindowTitle(title);
}

// File menu implementations
void MainWindow::onNew() {
    if (isModified) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "新增檔案", 
                                      "是否要儲存目前的檔案?",
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            onSave();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }
    textEdit->clear();
    currentFilePath = "";
    isModified = false;
    updateWindowTitle();
    statusbar->showMessage("已建立新檔案", 3000);
}

void MainWindow::onOpen() {
    if (isModified) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "開啟檔案", 
                                      "是否要儲存目前的檔案?",
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            onSave();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }
    
    QString fileName = QFileDialog::getOpenFileName(this,
                                                     "開啟檔案",
                                                     "",
                                                     "所有檔案 (*.*);;文字檔 (*.txt);;C++ 檔案 (*.cpp *.h)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            textEdit->setPlainText(in.readAll());
            file.close();
            currentFilePath = fileName;
            isModified = false;
            updateWindowTitle();
            statusbar->showMessage("已開啟檔案: " + fileName, 3000);
        } else {
            QMessageBox::warning(this, "錯誤", "無法開啟檔案: " + fileName);
        }
    }
}

void MainWindow::onClose() {
    if (isModified) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "關閉檔案", 
                                      "是否要儲存目前的檔案?",
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            onSave();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }
    textEdit->clear();
    currentFilePath = "";
    isModified = false;
    updateWindowTitle();
    statusbar->showMessage("已關閉檔案", 3000);
}

void MainWindow::onSave() {
    if (currentFilePath.isEmpty()) {
        onSaveAs();
    } else {
        QFile file(currentFilePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << textEdit->toPlainText();
            file.close();
            isModified = false;
            updateWindowTitle();
            statusbar->showMessage("已儲存檔案: " + currentFilePath, 3000);
        } else {
            QMessageBox::warning(this, "錯誤", "無法儲存檔案: " + currentFilePath);
        }
    }
}

void MainWindow::onSaveAs() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                     "另存新檔",
                                                     "",
                                                     "所有檔案 (*.*);;文字檔 (*.txt);;C++ 檔案 (*.cpp *.h)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << textEdit->toPlainText();
            file.close();
            currentFilePath = fileName;
            isModified = false;
            updateWindowTitle();
            statusbar->showMessage("已另存檔案: " + fileName, 3000);
        } else {
            QMessageBox::warning(this, "錯誤", "無法儲存檔案: " + fileName);
        }
    }
}

void MainWindow::onQuit() {
    if (isModified) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "退出程式", 
                                      "是否要儲存目前的檔案?",
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            onSave();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }
    QApplication::quit();
}

// Edit menu implementations
void MainWindow::onUndo() {
    textEdit->undo();
    statusbar->showMessage("已還原", 3000);
}

void MainWindow::onCut() {
    textEdit->cut();
    statusbar->showMessage("已剪下", 3000);
}

void MainWindow::onCopy() {
    textEdit->copy();
    statusbar->showMessage("已複製", 3000);
}

void MainWindow::onPaste() {
    textEdit->paste();
    statusbar->showMessage("已貼上", 3000);
}

void MainWindow::onSelectAll() {
    textEdit->selectAll();
    statusbar->showMessage("已全選", 3000);
}

// Debug menu implementations
void MainWindow::onStartDebug() {
    statusbar->showMessage("開始除錯 (功能示範)", 3000);
    QMessageBox::information(this, "除錯", "開始除錯功能 (F5)");
}

void MainWindow::onBreak() {
    statusbar->showMessage("中斷執行 (功能示範)", 3000);
    QMessageBox::information(this, "除錯", "中斷執行功能 (Ctrl+Pause)");
}

void MainWindow::onContinue() {
    statusbar->showMessage("繼續執行 (功能示範)", 3000);
    QMessageBox::information(this, "除錯", "繼續執行功能 (F10)");
}
