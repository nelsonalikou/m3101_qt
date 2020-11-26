#ifndef IMAGELIBRARY_H
#define IMAGELIBRARY_H
#define THUMBNAIL_SIZE 128
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
#include <QFuture>
#include <QList>

// Classe Item
class Item
{
private:
    QString path;
    QImage thumbnail;
public:
    Item(const QString &, const QImage &);
    ~Item();
};



class Model : public QAbstractListModel
{
    Q_OBJECT
private:
    QList<Item> items;
public:
    Model();
    ~Model();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &, int = Qt::DisplayRole) const;

public slots:
    void addItem (const QString &, const QImage &);
    void setThumbnail (int, const QImage &);
};

// Classe Image Library
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


//Classe Worker
class Worker : public QObject
{
    Q_OBJECT
private:
    QString path;
    QFutureWatcher<Item> watcher;
public:
    Worker(const QString &);
    ~Worker();
    void process();
    static Item MappedItem(const QString & path);
    void processItem(int);
    static QImage Thumbnail(const QString &);

signals:
    void newItem(const QString &);
    void finished();
    void newThumbnail(int, const QImage &);


};



#endif // IMAGELIBRARY_H
