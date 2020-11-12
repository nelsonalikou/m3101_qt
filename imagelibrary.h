#ifndef IMAGELIBRARY_H
#define IMAGELIBRARY_H

#include <QMainWindow>
#include <qstringlistmodel.h>
#include <qlistview.h>
#include <QToolBar>
#include <QMessageBox>
#include <QDir>
#include <QSettings>
#include <QFileDialog>

class ImageLibrary : public QMainWindow
{
    Q_OBJECT
private:
    QStringListModel model;
    QListView view;
    QToolBar toolbar;

public:
    ImageLibrary(QWidget *parent = nullptr);
    ~ImageLibrary();
    void go();
};

class Worker : public QObject
{
    Q_OBJECT
private:
    QStringList path;
public:
    Worker();
    ~Worker();
    process();
};

#endif // IMAGELIBRARY_H
