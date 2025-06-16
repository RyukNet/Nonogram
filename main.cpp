#include "MainWindow.h"

#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QFont font = a.font();
    font.setPixelSize(16);
    font.setFamily("Segoe UI");
    a.setFont(font);

    w.show();
    return a.exec();
}
