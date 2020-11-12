#include "imagelibrary.h"
#include <QSettings>
#include <QCoreApplication>
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("alik0002");
    QCoreApplication::setApplicationName("ImageLibrary");
    QApplication a(argc, argv);
    ImageLibrary w;
    w.show();
    return a.exec();
}
