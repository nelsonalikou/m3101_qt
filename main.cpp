#include "imagelibrary.h"
#include <QSettings>
#include <QCoreApplication>
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("alik0002");
    QCoreApplication::setApplicationName("ImageLibrary");
    QApplication a(argc, argv);
    ImageLibrary w;

    //test
    /*QSettings settings;
    QString start = settings.value("start",QDir::homePath()).toString();
    Worker work(start);
    work.process();*/

    w.show();
    return a.exec();
}
