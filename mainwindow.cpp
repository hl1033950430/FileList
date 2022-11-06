#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPushButton>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "fileitem.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTreeWidget *fileTree = ui->fileTree;
    QPushButton *queryButton = ui->queryButton;
    QPushButton *openButton = ui->openButton;

    // 初始化文件列表
    QStringList titles;
    titles.append("文件名");
    titles.append("大小");
    titles.append("目录");
    fileTree->setColumnCount(3);
    fileTree->setHeaderLabels(titles);

    // 生成文件树
    FileItem root;
    root.fileName = "7-Zip";
    root.absolutePath = "D:\\";
    root.isDir = true;
    QDir dir(root.absolutePath);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Dirs | QDir::Files | QDir::Hidden);
    getFileTree(&root, dir);

    // 转换成显示的树
    QTreeWidgetItem *tree = getTree(&root);
    fileTree->addTopLevelItem(tree);

    connect(ui->openButton, &QPushButton::clicked, [=](){
        this->path = QFileDialog::getOpenFileName(this, "");
        qDebug() << this->path;
    });

}

void MainWindow::getFileTree(FileItem *root, QDir &dir) {
    if (dir.isEmpty() || !dir.isReadable()) {
        root->size = 0;
    }
    QFileInfoList list = dir.entryInfoList();
    long long totalSize = 0;
    for (int i=0; i<list.size(); i++) {
        QFileInfo child = list[i];
        FileItem *item = new FileItem();
        item->isDir = true;
        item->fileName = child.fileName();
        item->absolutePath = child.absoluteFilePath();
        root->childs.append(item);
        if (child.isDir()) {
            dir.cd(child.fileName());
            getFileTree(item, dir);
            dir.cd("..");
        } else {
            item->size = child.size();
        }
        totalSize += item->size;
    }
    root->size = totalSize;
}

QTreeWidgetItem * MainWindow::getTree(FileItem *item) {
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(0, item->fileName);
    treeItem->setText(1, showFileSize(item->size));
    treeItem->setText(2, item->isDir ? "*" : "");
    if (!item->isDir) {
        return treeItem;
    }

    for (int i=0; i<item->childs.size(); i++) {
        FileItem *child = item->childs[i];
        treeItem->addChild(getTree(child));
    }
    return treeItem;
}

QString MainWindow::showFileSize(long long size) {
    int kb = 1024;
    int mb = kb * 1024;
    int gb = mb * 1024;
    QString result;
    if (size / gb > 0) {
        result = QString::number(size * 1.0 / gb, 'f', 1) + " GB";
    } else if (size / mb > 0) {
        result = QString::number(size * 1.0 / mb, 'f', 2) + " MB";
    } else if (size / kb > 0) {
        result = QString::number(size * 1.0 / kb, 'f', 2) + " KB";
    } else {
        result = QString::number(size) + " Byte";
    }
    return result;
}


MainWindow::~MainWindow()
{
    delete ui;
}

