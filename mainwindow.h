#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QDir>
#include "fileitem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getFileTree(FileItem *root, QDir &dir);
    QTreeWidgetItem * getTree(FileItem *item);
    QString showFileSize(long long size);

    QString path;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
