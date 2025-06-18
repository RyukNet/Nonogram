#include "MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QFont>

#include "AppStyle.h"

int main(int argc, char *argv[])
{
    QApplication::setStyle(new AppStyle());
    QApplication a(argc, argv);
    /*{
        QFile file("://qss/Dark.qss");
        if(file.open(QIODevice::ReadOnly)){
            QByteArray qss_str = file.readAll();
            a.setStyleSheet(qss_str);
        }
    }*/


    MainWindow w;

    QFont font = a.font();
    font.setPixelSize(16);
    font.setFamily("Segoe UI");
    a.setFont(font);

    w.show();
    return a.exec();
}
