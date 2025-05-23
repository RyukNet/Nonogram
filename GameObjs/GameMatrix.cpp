#include "GameMatrix.h"

#include <iostream>

#include <QPainter>
#include <QLine>
#include <QRect>
#include <QPen>
#include <QSizePolicy>

#include "MatrixGenerator.h"

#include <QDebug>

GameMatrix::GameMatrix(quint8 width, quint8 height, QWidget *parent)
    : QWidget{parent}
    ,m_width(width)
    ,m_height(height)
    ,m_matrix(height, std::vector<bool>(width, false))
    ,m_scale(1)
    ,m_mousePos(0, 0)
{
    //m_gridLay = new QGridLayout(this);
    //this->setLayout(m_gridLay);

    setMouseTracking(true);
    QSizePolicy sizePolicy = this->sizePolicy();
    //sizePolicy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    //sizePolicy.setVerticalPolicy(QSizePolicy::MinimumExpanding);
    sizePolicy.setWidthForHeight(true);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);

    //setFixedHeight(1000);
    //setFixedWidth(1000);

    for(quint8 h = 0; h < height; h++){
        for(quint8 w = 0; w < width; w++){
            m_matrix[h][w] = MatrixGenerator::GenerateNext();
        }
    }
    /*for(quint8 h = 0; h < height; h++){
        for(quint8 w = 0; w < width; w++){
            std::cout << ((m_matrix[h][w])? "X" : " ") << "|";
        }
        std::cout << std::endl;
    }*/
}

void GameMatrix::setScale(int value){
    m_scale = ((double) value) / 100;
    repaint();
}

int GameMatrix::heightForWidth(int h) const{
    return h;
}

void GameMatrix::paintEvent(QPaintEvent* event){

    QPainter painter(this);
    painter.scale(m_scale, m_scale);
    QColor backgroundColor(40, 134, 222);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), backgroundColor);

    qreal cellWidth = ((qreal)rect().width()) / ((qreal)m_width);
    qreal cellHeight = ((qreal)rect().height()) / ((qreal)m_height);

    QPen pen(QColor(12, 59, 94), m_gridWidth);
    painter.setPen(pen);
    for(qreal w = 0; w < m_width; w++){
        if(((int)w) % 5 == 0){
            pen.setWidth(m_gridWidth + 3);
            painter.setPen(pen);
        }else{
            pen.setWidth(m_gridWidth);
            painter.setPen(pen);
        }
        QLineF line;

        QPointF start;
        start.setX(w * ((qreal)this->rect().width() / (qreal)m_width));
        start.setY(0);

        QPointF end;
        end.setX(w * ((qreal)this->rect().width() / (qreal)m_width));
        end.setY(rect().height());

        line.setP1(start);
        line.setP2(end);

        painter.drawLine(line);

    }
    for(qreal h = 0; h < m_height; h++){
        if(((int)h) % 5 == 0){
            pen.setWidth(m_gridWidth + 2);
            painter.setPen(pen);
        }else{
            pen.setWidth(m_gridWidth);
            painter.setPen(pen);
        }
        QLineF line;

        QPointF start;
        start.setX(0);
        start.setY(h * ((qreal)this->rect().height() / (qreal)m_height));

        QPointF end;
        end.setX(rect().width());
        end.setY(h * ((qreal)this->rect().height() / (qreal)m_height));

        line.setP1(start);
        line.setP2(end);

        painter.drawLine(line);
    }
    pen.setWidth(m_gridWidth + 3);
    painter.setPen(pen);
    painter.drawRect(rect());

    pen.setWidth(m_gridWidth);
    painter.setPen(pen);

    qreal cellMargins = 2;

    std::vector<QRectF> checkedRects;
    for(quint8 h = 0; h < m_height; h++){
        for(quint8 w = 0; w < m_width; w++){
            if(m_matrix[h][w]){
                QRectF cell;
                cell.setX(w * cellWidth + cellMargins);
                cell.setY(h * cellHeight + cellMargins);
                cell.setWidth(cellWidth - (cellMargins * 2));
                cell.setHeight(cellHeight - (cellMargins * 2));
                checkedRects.push_back(cell);
            }
        }
    }
    pen.setWidth(0);
    painter.setPen(pen);
    painter.setBrush(Qt::black);
    painter.drawRects(checkedRects.data(), checkedRects.size());

    if(!m_mousePos.isNull() && rect().contains(m_mousePos)){
        qreal cellX = (int)((qreal)m_mousePos.rx() / (qreal)cellWidth) * cellWidth;
        qreal cellY = (int)((qreal)m_mousePos.ry() / (qreal)cellHeight) * cellHeight;
        QRect hightlightedCell(cellX + 2, cellY + 2, cellWidth - 2, cellHeight - 2);
        painter.setBrush(QColor(98, 171, 245));
        painter.drawRect(hightlightedCell);
    }

}

void GameMatrix::leaveEvent(QEvent* e){
    m_mousePos = QPoint();
    repaint();
    QWidget::leaveEvent(e);
}

void GameMatrix::mouseMoveEvent(QMouseEvent* e){
    m_mousePos = e->pos();
    repaint();
    QWidget::mouseMoveEvent(e);
}
