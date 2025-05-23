#pragma once

#include <vector>

#include <QWidget>
#include <QGridLayout>

class GameMatrix : public QWidget
{
    Q_OBJECT
public:
    explicit GameMatrix(quint8 width, quint8 height, QWidget *parent = nullptr);

signals:

protected:
    quint8 m_width;
    quint8 m_height;
    std::vector<std::vector<bool>> m_matrix;
    QGridLayout* m_gridLay = nullptr;
};
