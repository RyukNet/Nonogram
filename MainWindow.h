#pragma once

#include <QMainWindow>
#include <QSlider>

#include "GameMatrix.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    QSlider* m_scalSlider = nullptr;
    GameMatrix* m_gameMatrix = nullptr;
};
