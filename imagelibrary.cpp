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
#include "imagelibrary.h"


ImageLibrary::ImageLibrary(QWidget *parent)
    : QMainWindow(parent)
{
    this->view.setModel(&model);
    this->toolbar.addAction("GO !", this, &ImageLibrary::go);
    addToolBar(&toolbar);
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
    }


    //Création d'une instance de worker
    //Worker work(dir);

    //Connexion du signal à addItem
    //connect(&work, &Worker::newItem,
     //       this, &ImageLibrary::addItem);

    //Déclenchement de worker
    //work.process();


    /*QThread * thread = new QThread;
    Worker * worker = new Worker (dir);
    worker->moveToThread (thread);
    connect (thread, &QThread::started, worker, &Worker::process);
    connect (worker, &Worker::finished, thread, &QThread::quit);
    connect (worker, &Worker::finished, worker, &Worker::deleteLater);
    connect (thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start ();
    connect(worker, &Worker::newItem, this, &ImageLibrary::addItem);
    */
    //QtConcurrent::run(worker, &Worker::process);

    QtConcurrent::run([=](){
        Worker worker(dir);
        connect(&worker, &Worker::newItem, this, &ImageLibrary::addItem);
        worker.process();
    });

}



Worker::Worker(const QString &path) : QObject(),path(path)
{

}

Worker::~Worker()
{

}

void Worker::process()
{
    //File d'attente
    QStringList wait;
    wait << this->path;

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
      }
    }

}



void ImageLibrary::addItem (const QString & item)
{
  QStringList list = model.stringList ();
  model.setStringList (list << item);
}
