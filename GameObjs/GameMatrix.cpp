#include "GameMatrix.h"

#include <iostream>

#include <QPainter>
#include <QLine>
#include <QRect>
#include <QPen>
#include <QSizePolicy>

#include "MatrixGenerator.h"

#include <QDebug>

GameMatrix::GameMatrix(GameEngine* engine, QWidget *parent)
    : QWidget{parent}
    ,m_width(engine->width())
    ,m_height(engine->height())
    ,m_matrix(engine->height(), std::vector<CellState>(engine->width(), CellState::NEUTRAL))
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

    /*for(quint8 h = 0; h < height; h++){
        for(quint8 w = 0; w < width; w++){
            m_matrix[h][w] = MatrixGenerator::GenerateNext();
        }
    }*/
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
    std::vector<QRectF> crossedRects;
    for(quint8 h = 0; h < m_height; h++){
        for(quint8 w = 0; w < m_width; w++){
            if(m_matrix[h][w] != CellState::NEUTRAL){
                QRectF cell;
                cell.setX(w * cellWidth + cellMargins);
                cell.setY(h * cellHeight + cellMargins);
                cell.setWidth(cellWidth - (cellMargins * 2));
                cell.setHeight(cellHeight - (cellMargins * 2));
                if(m_matrix[h][w] == CellState::CHECKED){
                    checkedRects.push_back(cell);
                }else if(m_matrix[h][w] == CellState::CROSSED){
                    crossedRects.push_back(cell);
                }
            }
        }
    }
    pen.setWidth(0);
    painter.setPen(pen);
    painter.setBrush(Qt::black);
    painter.drawRects(checkedRects.data(), checkedRects.size());
    painter.setBrush(Qt::red);
    painter.drawRects(crossedRects.data(), crossedRects.size());

    if(!m_mousePos.isNull() && rect().contains(m_mousePos)){
        qreal cellX = (int)((qreal)m_mousePos.rx() / (qreal)cellWidth) * cellWidth;
        qreal cellY = (int)((qreal)m_mousePos.ry() / (qreal)cellHeight) * cellHeight;
        QRect hightlightedCell(cellX + 2, cellY + 2, cellWidth - 2, cellHeight - 2);
        painter.setBrush(QColor(98, 171, 245));
        painter.drawRect(hightlightedCell);
    }

    if(m_selectionBuffer.m_valid){
        std::vector<QRectF> selectedRects(0);
        for(quint8 h = m_selectionBuffer.start_Y; h <= m_selectionBuffer.end_Y; h++){
            for(quint8 w = m_selectionBuffer.start_X; w <= m_selectionBuffer.end_X; w++){
                QRectF cell;
                cell.setX(w * cellWidth + cellMargins);
                cell.setY(h * cellHeight + cellMargins);
                cell.setWidth(cellWidth - (cellMargins * 2));
                cell.setHeight(cellHeight - (cellMargins * 2));
                selectedRects.push_back(cell);
            }
        }
        switch(m_currentMode){
        case VOIDING:
            painter.setBrush(QColor(40, 134, 222));
            break;
        case CHECKING:
            painter.setBrush(Qt::black);
            break;
        case CROSSING:
            painter.setBrush(Qt::red);
            break;
        }
        painter.drawRects(selectedRects.data(), selectedRects.size());
        m_selectionBuffer.start_X = 0;
        m_selectionBuffer.start_Y = 0;
        m_selectionBuffer.end_X = 0;
        m_selectionBuffer.end_X = 0;
        m_selectionBuffer.m_valid = false;
    }

}

void GameMatrix::mousePressEvent(QMouseEvent *e){
    m_selectBegin = e->pos();
    qreal cellWidth = ((qreal)rect().width()) / ((qreal)m_width);
    qreal cellHeight = ((qreal)rect().height()) / ((qreal)m_height);
    int cellX = (int)((qreal)m_selectBegin.x() / (qreal)cellWidth);
    int cellY = (int)((qreal)m_selectBegin.y() / (qreal)cellHeight);


    if(e->button() == Qt::LeftButton){
        bool checked = m_matrix[cellY][cellX] == CellState::CHECKED;
        if(checked){
            m_currentMode = ActionMode::VOIDING;
        }else{
            m_currentMode = ActionMode::CHECKING;
        }
    }else if(e->button() == Qt::RightButton){
        bool crossed = m_matrix[cellY][cellX] == CellState::CROSSED;
        if(crossed){
            m_currentMode = ActionMode::VOIDING;
        }else{
            m_currentMode = ActionMode::CROSSING;
        }
    }

    QWidget::mousePressEvent(e);
}

void GameMatrix::mouseReleaseEvent(QMouseEvent *e){
    QPointF startSelect;
    startSelect.setX(std::min(e->pos().x(), m_selectBegin.x()));
    startSelect.setY(std::min(e->pos().y(), m_selectBegin.y()));

    QPointF endSelect;
    endSelect.setX(std::max(e->pos().x(), m_selectBegin.x()));
    endSelect.setY(std::max(e->pos().y(), m_selectBegin.y()));

    if(endSelect.x() > rect().width()){
        endSelect.setX(rect().width() - 3);
    }else if(endSelect.x() < 0){
        endSelect.setX(0);
    }

    if(endSelect.y() > rect().height()){
        endSelect.setY(rect().height() - 3);
    }else if(endSelect.y() < 0){
        endSelect.setY(0);
    }

    if(startSelect.x() > rect().width()){
        startSelect.setX(rect().width() - 3);
    }else if(startSelect.x() < 0){
        startSelect.setX(0);
    }

    if(startSelect.y() > rect().height()){
        startSelect.setY(rect().height() - 3);
    }else if(startSelect.y() < 0){
        startSelect.setY(0);
    }

    qreal cellWidth = ((qreal)rect().width()) / ((qreal)m_width);
    qreal cellHeight = ((qreal)rect().height()) / ((qreal)m_height);

    QRectF selectionRect(startSelect, endSelect);
    int startCellX = (int)((qreal)selectionRect.x() / (qreal)cellWidth);
    int startCellY = (int)((qreal)selectionRect.y() / (qreal)cellHeight);

    //qDebug() << "start(" << startCellX << ", " << startCellY << ")";

    int endCellX = (int)((qreal)selectionRect.right() / (qreal)cellWidth);
    int endCellY = (int)((qreal)selectionRect.bottom() / (qreal)cellHeight);

    //qDebug() << "end(" << endCellX << ", " << endCellY << ")";

    for(int h = startCellY; h <= endCellY; h++){
        for(int w = startCellX; w <= endCellX; w++){
            switch(m_currentMode){
            case CHECKING:
                m_matrix[h][w] =  CellState::CHECKED;
                break;
            case CROSSING:
                m_matrix[h][w] =  CellState::CROSSED;
                break;
            case VOIDING:
                m_matrix[h][w] =  CellState::NEUTRAL;
                break;
            }

        }
    }

    m_selectBegin = QPoint();
    repaint();
    QWidget::mouseReleaseEvent(e);
}

void GameMatrix::leaveEvent(QEvent* e){
    m_mousePos = QPoint();
    repaint();
    QWidget::leaveEvent(e);
}

void GameMatrix::mouseMoveEvent(QMouseEvent* e){
    m_mousePos = e->pos();
    if(e->buttons() & (Qt::LeftButton | Qt::RightButton)){
        QPointF startSelect;
        startSelect.setX(std::min(e->pos().x(), m_selectBegin.x()));
        startSelect.setY(std::min(e->pos().y(), m_selectBegin.y()));

        QPointF endSelect;
        endSelect.setX(std::max(e->pos().x(), m_selectBegin.x()));
        endSelect.setY(std::max(e->pos().y(), m_selectBegin.y()));

        if(endSelect.x() > rect().width()){
            endSelect.setX(rect().width() - 3);
        }else if(endSelect.x() < 0){
            endSelect.setX(0);
        }

        if(endSelect.y() > rect().height()){
            endSelect.setY(rect().height() - 3);
        }else if(endSelect.y() < 0){
            endSelect.setY(0);
        }

        if(startSelect.x() > rect().width()){
            startSelect.setX(rect().width() - 3);
        }else if(startSelect.x() < 0){
            startSelect.setX(0);
        }

        if(startSelect.y() > rect().height()){
            startSelect.setY(rect().height() - 3);
        }else if(startSelect.y() < 0){
            startSelect.setY(0);
        }

        qreal cellWidth = ((qreal)rect().width()) / ((qreal)m_width);
        qreal cellHeight = ((qreal)rect().height()) / ((qreal)m_height);

        QRectF selectionRect(startSelect, endSelect);
        int startCellX = (int)((qreal)selectionRect.x() / (qreal)cellWidth);
        int startCellY = (int)((qreal)selectionRect.y() / (qreal)cellHeight);

        int endCellX = (int)((qreal)selectionRect.right() / (qreal)cellWidth);
        int endCellY = (int)((qreal)selectionRect.bottom() / (qreal)cellHeight);

        m_selectionBuffer.start_X = startCellX;
        m_selectionBuffer.start_Y = startCellY;
        m_selectionBuffer.end_X = endCellX;
        m_selectionBuffer.end_Y = endCellY;
        m_selectionBuffer.m_valid = true;
    }
    repaint();
    QWidget::mouseMoveEvent(e);
}
