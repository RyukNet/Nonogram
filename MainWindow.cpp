#include "MainWindow.h"

#include <QHBoxLayout>

#include "MatrixCell.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* central = new QWidget();
    setCentralWidget(central);
    QHBoxLayout* HLay = new QHBoxLayout();
    HLay->setSpacing(8);
    central->setLayout(HLay);
    auto button1 = new MatrixCell();
    button1->setText("1");
    HLay->addWidget(button1);
    auto button2 = new MatrixCell();
    button2->setText("2");
    HLay->addWidget(button2);
}

MainWindow::~MainWindow() {}
