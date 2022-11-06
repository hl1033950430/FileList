#ifndef FILEITEM_H
#define FILEITEM_H

#include <QString>
#include <QList>

class FileItem
{
public:
    FileItem();
    QString fileName;
    QString absolutePath;
    bool isDir;
    long long size;
    QList<FileItem*> childs;
};

#endif // FILEITEM_H
