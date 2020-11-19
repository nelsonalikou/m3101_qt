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
#include <QThread>
#include <QtConcurrent>
#include <iostream>

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

public slots:
    void addItem (const QString &);
};

class Worker : public QObject
{
    Q_OBJECT
private:
    QString path;
public:
    Worker(const QString &);
    ~Worker();
    void process();
signals:
    void newItem(const QString &);
    void finished();
};

#endif // IMAGELIBRARY_H
