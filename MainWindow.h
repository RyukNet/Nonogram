#pragma once

#include <QMainWindow>
#include <QSlider>

#include "GameEngine.h"
#include "GameMatrix.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    GameEngine* m_gameEngine = nullptr;
    GameMatrix* m_gameMatrix = nullptr;

};
