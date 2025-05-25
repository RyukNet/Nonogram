#include "MainWindow.h"

#include <iostream>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_gameEngine = new GameEngine(25, 25, this);

    QWidget* central = new QWidget();
    setCentralWidget(central);

    QVBoxLayout* mainLay = new QVBoxLayout();
    //mainLay->setMargin(4);
    central->setLayout(mainLay);

    m_scalSlider = new QSlider(Qt::Horizontal);
    mainLay->addWidget(m_scalSlider);
    m_scalSlider->setMaximum(1000);
    m_scalSlider->setMinimum(0);
    m_scalSlider->setValue(100);

    QHBoxLayout* matrixViewLay = new QHBoxLayout();
    mainLay->addLayout(matrixViewLay);

    //matrixViewLay->addStretch(1);
    //matrixViewLay->addWidget(new QPushButton("Text"));

    m_gameMatrix = new GameMatrix(m_gameEngine);
    matrixViewLay->addWidget(m_gameMatrix, 1);
    //matrixViewLay->addStretch(1);

    /*
    mainLay->addWidget(new QWidget(), 0, 0);
    mainLay->addWidget(new QWidget(), 0, 1);
    mainLay->addWidget(m_scalSlider, 1, 0);
    mainLay->addWidget(m_gameMatrix, 1, 1);
    mainLay->addWidget(new QWidget(), 1, 2);
    mainLay->addWidget(new QWidget(), 2, 0);
    mainLay->addWidget(new QWidget(), 2, 1);

    //mainLay->itemAt(3)->setAlignment(Qt::AlignCenter);

    mainLay->setColumnStretch(0, 1);
    mainLay->setColumnStretch(1, 1);
    mainLay->setColumnStretch(2, 1);
    mainLay->setRowStretch(0, 1);
    mainLay->setRowStretch(1, 1);
    mainLay->setRowStretch(2, 1);*/

    setMinimumSize(200, 200);

    connect(m_scalSlider, &QSlider::valueChanged, m_gameMatrix, &GameMatrix::setScale);

}

MainWindow::~MainWindow() {}
