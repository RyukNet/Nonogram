#pragma once

#include <vector>

#include <QWidget>
#include <QGridLayout>
#include <QPaintEvent>
#include <QPoint>

#include "MatrixCell.h"

class GameMatrix : public QWidget
{
    Q_OBJECT
public:
    explicit GameMatrix(quint8 width, quint8 height, QWidget *parent = nullptr);
    int heightForWidth(int) const override;

public slots:
    void setScale(int);
signals:

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    //void dra
    quint8 m_width;
    quint8 m_height;
    std::vector<std::vector<bool>> m_matrix;
    QGridLayout* m_gridLay = nullptr;

    qreal m_gridWidth = 1;
    qreal m_scale;
    QPoint m_mousePos;

};
