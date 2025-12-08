#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow,
                        Ui_MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File menu slots
    void onNew();
    void onOpen();
    void onClose();
    void onSave();
    void onSaveAs();
    void onQuit();
    
    // Edit menu slots
    void onUndo();
    void onCut();
    void onCopy();
    void onPaste();
    void onSelectAll();
    
    // Debug menu slots
    void onStartDebug();
    void onBreak();
    void onContinue();

private:
    QString currentFilePath;
    bool isModified;
    void updateWindowTitle();
};
#endif // MAINWINDOW_H
