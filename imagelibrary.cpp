#include <QApplication>
#include <QSettings>
#include <QCoreApplication>
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
}

Worker::Worker()
{

}


Worker::~Worker()
{

}
