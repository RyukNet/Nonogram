#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QTimer>
#include <QTime>

#include "GameEngine.h"
#include "GameMatrix.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void setConnections();
    GameEngine* m_gameEngine = nullptr;
    QLabel* m_timerLabel = nullptr;
    QTimer* m_timer = nullptr;
    QTime m_gameTime;
    unsigned int m_days = 0;
    GameMatrix* m_gameMatrix = nullptr;

    QWidget* m_headerWidget = nullptr;
    QPushButton* m_doneButton = nullptr;
    QPushButton* m_startOverButton = nullptr;
    QPushButton* m_newPuzzleButton = nullptr;
    QSpinBox* m_widthInput = nullptr;
    QSpinBox* m_heightInput = nullptr;
};
