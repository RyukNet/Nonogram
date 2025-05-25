#pragma once

#include <vector>

#include <QWidget>
#include <QGridLayout>
#include <QPaintEvent>
#include <QResizeEvent>
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
    void resizeEvent(QResizeEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    //void dra
    quint8 m_columns;
    quint8 m_rows;
    std::vector<std::vector<CellState>> m_matrix;
    std::vector<std::vector<quint8>> m_rowsTasks;
    std::vector<std::vector<quint8>> m_colsTasks;

    // Variables for drawing/painting
    quint8 m_rowMaxTasksCount = 0; // The row with most task numbers
    quint8 m_colMaxTasksCount = 0; // The row with most task numbers
    qreal m_cellDimension = 0;
    qreal m_verGridMargins = 0;
    qreal m_horGridMargins = 0;

    QColor m_backGroundColor;
    QColor m_gridColor;
    QColor m_checkColor;
    QColor m_crossColor;

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
