#pragma once

#include <QMainWindow>
#include <QPushButton>

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

    QWidget* m_headerWidget = nullptr;
    QPushButton* m_doneButton = nullptr;
    QPushButton* m_startOverButton = nullptr;
    QPushButton* m_newPuzzleButton = nullptr;
};
