#pragma once

#include <vector>

#include <QWidget>
#include <QGridLayout>
#include <QPaintEvent>
#include <QPoint>

#include "GameEngine.h"

class GameMatrix : public QWidget
{
    Q_OBJECT
public:
    enum CellState{
        NEUTRAL,
        CHECKED,
        CROSSED,
    };
    enum ActionMode{
        VOIDING = NEUTRAL,
        CHECKING = CHECKED,
        CROSSING = CROSSED,
    };
    explicit GameMatrix(GameEngine* engine, QWidget *parent = nullptr);
    int heightForWidth(int) const override;

public slots:
    void setScale(int);
signals:

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    //void dra
    quint8 m_width;
    quint8 m_height;
    std::vector<std::vector<CellState>> m_matrix;
    QGridLayout* m_gridLay = nullptr;

    qreal m_gridWidth = 1;
    qreal m_scale;
    QPoint m_mousePos;

    ActionMode m_currentMode;
    QPoint m_selectBegin;
    struct SelectionBuffer{
        quint8 start_X;
        quint8 start_Y;
        quint8 end_X;
        quint8 end_Y;
        bool m_valid = false;
    } m_selectionBuffer;

};
