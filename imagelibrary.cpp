#include <QApplication>
#include <QSettings>
#include <QCoreApplication>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <iostream>
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
    Worker work(dir);

    //Connexion du signal à addItem
    QObject::connect(&work, &Worker::newItem,
                     this, &ImageLibrary::addItem);

    //Déclenchement de worker
    work.process();

}



Worker::Worker(const QString &path) : QObject()
{
    this->path = path;
}


Worker::~Worker()
{

}

void Worker::process()
{
    //File d'attente
    QStringList wait;
    wait << this->path;

    QDir dir(this->path);

    // Edition des filtres
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg";


    //Boucle de traitement
    while(!wait.isEmpty())
    {
      QString jsp = wait.takeFirst();

      //std::cout << jsp.toStdString() << std::endl;

      QFileInfo info(jsp);

      if(info.isDir())
      {
          QDir dir(jsp);

          QFileInfoList fileInfos = dir.entryInfoList(filters, QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

          //for(const QFileInfo &i: fileInfos)
          for (int i=0; i < fileInfos.size();i++)
          {
              wait << fileInfos[i].absoluteFilePath();
              //std::cout << fileInfos[i].absoluteFilePath().toStdString() << std::endl;
          }


       }
      else
      {
          emit newItem(info.absoluteFilePath());
      }
    }


}



void ImageLibrary::addItem (const QString & item)
{
  QStringList list = model.stringList ();
  model.setStringList (list << item);
}
