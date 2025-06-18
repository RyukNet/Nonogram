#include "MainWindow.h"

#include <iostream>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QPushButton>
#include <QSizePolicy>

#include <QDebug>
#include "ColorRepository.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_gameEngine = new GameEngine(5, 5, this);

    m_timer = new QTimer(this);
    m_timer->setTimerType(Qt::PreciseTimer);
    m_timer->setInterval(1000);
    m_gameTime = QTime(0, 0, 0);

    QWidget* central = new QWidget();
    setCentralWidget(central);

    QVBoxLayout* mainLay = new QVBoxLayout();
    central->setLayout(mainLay);

    QHBoxLayout* matrixViewLay = new QHBoxLayout();
    mainLay->addLayout(matrixViewLay);

    QVBoxLayout* gameVLay = new QVBoxLayout();
    matrixViewLay->addLayout(gameVLay);

    QHBoxLayout* headerLay = new QHBoxLayout();
    gameVLay->addLayout(headerLay);

    m_timerLabel = new QLabel("00:00:00");
    headerLay->addWidget(m_timerLabel, 0, Qt::AlignCenter);

    m_themeToggle = new ToggleSwitch();
    headerLay->addWidget(m_themeToggle);

    m_gameMatrix = new GameMatrix(m_gameEngine);
    gameVLay->addWidget(m_gameMatrix, 1);

    {
        QHBoxLayout* footerHLay = new QHBoxLayout();
        mainLay->addLayout(footerHLay, 0);

        footerHLay->addStretch(1);
        QVBoxLayout* widgetsLay = new QVBoxLayout();
        footerHLay->addLayout(widgetsLay);
        footerHLay->addStretch(1);

        m_doneButton = new QPushButton("Done");
        widgetsLay->addWidget(m_doneButton, 0);

        m_progressMsg = new QLabel("");
        widgetsLay->addWidget(m_progressMsg, 0, Qt::AlignHCenter);

        QHBoxLayout* buttonsHLay = new QHBoxLayout();
        widgetsLay->addLayout(buttonsHLay, 0);

        m_startOverButton = new QPushButton("Start Over");
        buttonsHLay->addWidget(m_startOverButton);

        m_newPuzzleButton = new QPushButton("New Puzzle");
        buttonsHLay->addWidget(m_newPuzzleButton);

        m_widthInput = new QSpinBox();
        m_widthInput->setMinimum(5);
        m_widthInput->setMaximum(50);
        m_widthInput->setValue(5);
        m_widthInput->setSingleStep(5);
        buttonsHLay->addWidget(m_widthInput);

        m_heightInput = new QSpinBox();
        m_heightInput->setMinimum(5);
        m_heightInput->setMaximum(50);
        m_heightInput->setSingleStep(5);
        m_heightInput->setValue(5);
        buttonsHLay->addWidget(m_heightInput);
    }

    setMinimumSize(800, 800);
    setConnections();
    m_timer->start();
}

void MainWindow::setConnections(){
    m_timer->callOnTimeout([this](){
        if(m_gameTime.hour() == 23 && m_gameTime.minute() == 59 && m_gameTime.second() == 59){
            m_days++;
        }
        m_gameTime = m_gameTime.addSecs(1);
        QString timeText = m_gameTime.toString();
        if(m_days > 0){
            QString daysText = QString("%1").arg(m_days);
            daysText.push_back("d:");
            timeText.push_front(daysText);
        }

        m_timerLabel->setText(timeText);
    });

    connect(m_startOverButton, &QPushButton::clicked, this, [&](){
        m_doneButton->setEnabled(true);
        m_gameMatrix->restart();
        m_progressMsg->setText("");
    });

    connect(m_newPuzzleButton, &QPushButton::clicked, this, [&](){
        quint8 width = (quint8)m_widthInput->value();
        quint8 height = (quint8)m_heightInput->value();
        m_gameEngine->setSize(width, height);

        m_doneButton->setEnabled(true);
        m_timerLabel->setText("00:00:00");
        m_gameTime.setHMS(0, 0, 0, 0);
        m_timer->start();
        m_progressMsg->setText("");
    });

    connect(m_gameEngine, &GameEngine::resized, m_gameMatrix, &GameMatrix::resizeGrid);

    connect(m_themeToggle, &ToggleSwitch::checked, this, [&](bool check){
        //Theme::setDarkMode(check);
        ColorRepository::setDarkMode(check);
    });

    connect(m_doneButton, &QPushButton::clicked, this, [&](){
        std::vector<std::vector<bool>> matrix;
        m_gameMatrix->matrixToBool(matrix);
        quint32 errors = m_gameEngine->validateGrid(matrix);
        qDebug() << "Erros : " << errors;
        m_progressMsg->setText(QString("%1").arg(errors));
    });

    connect(m_gameEngine, &GameEngine::valid, this, [&](){
        m_doneButton->setEnabled(false);
        m_timer->stop();
    });
}

MainWindow::~MainWindow() {}
