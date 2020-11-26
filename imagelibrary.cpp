#include <QApplication>
#include <QSettings>
#include <QCoreApplication>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <iostream>
#include <QThread>
#include <QtConcurrent>
#include <QFuture>
#include <QList>
#include <QThread>
#include <QListView>
#include "imagelibrary.h"


ImageLibrary::ImageLibrary(QWidget *parent)
    : QMainWindow(parent)
{
    this->view.setModel(&model);
    this->toolbar.addAction("GO !", this, &ImageLibrary::go);
    addToolBar(&toolbar);

    this->view.setViewMode(QListView::IconMode);
    this->view.setGridSize(QSize (THUMBNAIL_SIZE, THUMBNAIL_SIZE));
    setCentralWidget(&view);

}

ImageLibrary::~ImageLibrary()
{

}

void ImageLibrary::go()
{
    QSettings settings;
    QString start = settings.value("start",QDir::homePath()).toString();
    QString dir = QFileDialog::getExistingDirectory(this,tr("Open Directory"),start);



    if (dir.isEmpty()){
        QMessageBox::warning(this, "Info","Failure");
    }
    else
    {
        settings.setValue("start",dir);
        QMessageBox::information(this, "Info","Success");


        QThread * thread = new QThread;
         //Worker worker(dir);
        Worker * worker = new Worker (dir);
        worker->moveToThread (thread);



        connect(worker, &Worker::newItem, &model, &Model::addItem);

        connect (thread, &QThread::started, worker, &Worker::process);
        connect (worker, &Worker::finished, thread, &QThread::quit);
        connect (worker, &Worker::finished, worker, &Worker::deleteLater);
        connect (thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start ();

        connect(worker, &Worker::newThumbnail, &model, &Model::setThumbnail);
        worker->process();

        settings.setValue("lastDir", dir);
    }


    //Création d'une instance de worker
    //Worker work(dir);

    //Connexion du signal à addItem
    //connect(&work, &Worker::newItem,
     //       this, &ImageLibrary::addItem);

    //Déclenchement de worker
    //work.process();


    QtConcurrent::run([=](){
        Worker worker(dir);
        connect(&worker, &Worker::newItem, this, &ImageLibrary::addItem);
        worker.process();
    });

}


//Fonction d'ajout des items dans la librairie
/*void ImageLibrary::addItem (const QString & item)
{
  QStringList list = model.stringList ();
  model.setStringList (list << item);
}
*/


Worker::Worker(const QString &path) : QObject(),path(path), watcher()
{

}

Worker::~Worker()
{

}


//Methode de recuperation des images et d'emissions des signaux
void Worker::process()
{
    //File d'attente
    QStringList wait;
    wait << this->path;

    QStringList paths;

    // Edition des filtres
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg";


    //Boucle de traitement
    while(!wait.isEmpty())
    {
      QString premierElement = wait.takeFirst();

      //std::cout << premierElement.toStdString() << std::endl;

      QFileInfo info(path,premierElement);

      if(info.isDir())
      {
          QDir dir(info.absoluteFilePath());

          QFileInfoList fileInfos = dir.entryInfoList(filters, QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

          //for(const QFileInfo &i: fileInfos) est une autre posibilité
          for (int i=0; i < fileInfos.size();i++)
          {
              wait << fileInfos[i].absoluteFilePath();
              std::cout << fileInfos[i].absoluteFilePath().toStdString() << std::endl;
          }


       }
      else
      {
          //std::cout << info.absoluteFilePath().toStdString() << std::endl;
          emit newItem(info.absoluteFilePath());
          paths << info.absoluteFilePath();
      }
    }
    connect(&this->watcher, &QFutureWatcher<Item>::finished, this, &Worker::finished);
    connect(&watcher, &QFutureWatcher<Item>::resultReadyAt, this, &Worker::processItem);
    watcher.setFuture( QtConcurrent::mapped(path, Worker::MappedItem));

}


//Fonction de création de la miniature das le worker
QImage Worker::Thumbnail(const QString & path)
{
    QImage img (path);
    return img.scaled(QSize (THUMBNAIL_SIZE, THUMBNAIL_SIZE), Qt::KeepAspectRatio);
}


Item Worker::MappedItem(const QString & path)
{
    //std::cout << "MappedItem " << path.toStdString() << std::endl;
    Item item(path, Worker::Thumbnail(path));
    return item;
}


void Worker::processItem(int k)
{
    std::cout << "processItem " << k << std::endl;
    emit newThumbnail( k, watcher.resultAt(k).thumbnail);
}





Item::Item(const QString &path, const QImage &thumbnail) : path(path),thumbnail(thumbnail)
{

}

Item::~Item()
{

}


Model::Model() : items()
{

}


Model::~Model()
{

}


int Model::rowCount(const QModelIndex &index) const
{
    /*int row = 0;
    if (index.isValid())
    {
        row = index.row();
    }
    return row;*/
    return this->items.size();
}


QVariant Model::data(const QModelIndex &index, int role) const{
    return index.data(role);
}


void Model::addItem(const QString &path, const QImage &thumbnail)
{
    Item item (path,thumbnail);
    beginInsertRows(QModelIndex (), rowCount(), rowCount());
    this->items << item;
    endInsertRows();
}
