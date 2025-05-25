#include "GameMatrix.h"

#include <iostream>
#include <algorithm>

#include <QPainter>
#include <QLine>
#include <QRect>
#include <QPen>
#include <QFont>
#include <QFontMetricsF>
#include <QSizePolicy>

#include "MatrixGenerator.h"

#include <QDebug>

#define Q_REAL(x) ((qreal) x)

GameMatrix::GameMatrix(GameEngine* engine, QWidget *parent)
    : QWidget{parent}
    ,m_columns(engine->columns())
    ,m_rows(engine->rows())
    ,m_matrix(engine->rows(), std::vector<CellState>(engine->columns(), CellState::NEUTRAL))
    ,m_mousePos(0, 0)
    ,m_backGroundColor(255, 0, 0, 0)
    ,m_gridColor(255, 255, 255)
    ,m_checkColor(Qt::black)
    ,m_crossColor(Qt::red)
    ,m_matrixArea(0, 0, 0, 0)
    ,m_highlightColor(98, 171, 245)
{
    setMouseTracking(true);
    QSizePolicy sizePolicy = this->sizePolicy();
    //sizePolicy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    //sizePolicy.setVerticalPolicy(QSizePolicy::MinimumExpanding);
    sizePolicy.setWidthForHeight(true);
    sizePolicy.setHeightForWidth(true);
    sizePolicy.setHorizontalPolicy(QSizePolicy::Minimum);
    sizePolicy.setVerticalPolicy(QSizePolicy::Minimum);
    setSizePolicy(sizePolicy);
    
    //setFixedHeight(1000);
    //setFixedWidth(1000);
    m_rowsTasks = engine->columnsTasks();
    std::for_each(m_rowsTasks.begin(), m_rowsTasks.end(), [&](const std::vector<quint8> row){
        if(row.size() > m_rowMaxTasksCount){
            m_rowMaxTasksCount = (quint8) row.size();
        }
    });
    m_colsTasks = engine->rowsTasks();
    std::for_each(m_colsTasks.begin(), m_colsTasks.end(), [&](const std::vector<quint8> col){
        if(col.size() > m_colMaxTasksCount){
            m_colMaxTasksCount = (quint8) col.size();
        }
    });
    std::cout << "max row tasks : " << (int) m_rowMaxTasksCount << std::endl;
    std::cout << "max col tasks : " << (int) m_colMaxTasksCount << std::endl;
}

int GameMatrix::heightForWidth(int w) const{
    return (qreal)w / ((qreal)m_columns / (qreal)m_rows);
}

void GameMatrix::resizeEvent(QResizeEvent* event){
    QRectF widgetRect = rect();
    
    qreal horMinCell = widgetRect.width() / Q_REAL(m_columns + m_rowMaxTasksCount * 2);
    qreal verMinCell = widgetRect.height() / Q_REAL(m_rows + m_colMaxTasksCount);
    
    m_cellDimension = std::min(horMinCell, verMinCell);
    
    m_horGridMargins = (widgetRect.width() - (m_cellDimension * (m_columns + m_rowMaxTasksCount * 2))) / 2;
    m_verGridMargins = (widgetRect.height() - (m_cellDimension * (m_rows + m_colMaxTasksCount))) / 2;
    
    m_matrixArea = widgetRect;
    m_matrixArea.setWidth(widgetRect.width() - (m_horGridMargins + (m_cellDimension * m_rowMaxTasksCount)));
    m_matrixArea.setHeight(widgetRect.height() - m_verGridMargins);
    m_matrixArea.setX(widgetRect.x() + m_horGridMargins + (m_cellDimension * m_rowMaxTasksCount));
    m_matrixArea.setY(widgetRect.y() + m_verGridMargins + (m_cellDimension * m_colMaxTasksCount));

    m_fontSize = m_cellDimension * 0.64;

    QWidget::resizeEvent(event);
}

void GameMatrix::paintEvent(QPaintEvent* event){
    QRect widgetRect = rect();
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QFont font = painter.font();
    font.setWeight(QFont::DemiBold);
    font.setPixelSize(m_fontSize);
    painter.setFont(font);
    
    // Background
    painter.fillRect(rect(), m_backGroundColor);
    
    // Grid
    painter.setBrush(m_gridColor);
    for(quint8 col = 0;
         col <= m_rowMaxTasksCount * 2 + m_columns;
         col++){
        
        qreal coordX = ((qreal) col) * m_cellDimension + m_horGridMargins;
        qreal startY = m_verGridMargins;
        qreal endY = widgetRect.height() - m_verGridMargins;
        
        if(col == 0 || col == m_rowMaxTasksCount * 2 + m_columns){
            startY += (m_cellDimension * m_colMaxTasksCount);
        }else if((col > 0 && col < m_rowMaxTasksCount) ||
                   (col > m_columns + m_rowMaxTasksCount && col < m_rowMaxTasksCount * 2 + m_columns)){
            continue;
        }
        
        QPointF startPoint;
        startPoint.setX(coordX);
        startPoint.setY(startY);
        
        QPointF endPoint;
        endPoint.setX(coordX);
        endPoint.setY(endY);
        
        QLineF line(startPoint, endPoint);
        painter.drawLine(line);
    }
    for(quint8 row = 0;
         row <= m_colMaxTasksCount + m_rows;
         row++){
        
        qreal startX = m_horGridMargins;
        qreal endX = widgetRect.width() - m_horGridMargins;
        qreal coordY = ((qreal) row) * m_cellDimension + m_verGridMargins;
        
        if(row == 0){
            startX += (m_cellDimension * m_rowMaxTasksCount);
            endX -= (m_cellDimension * m_rowMaxTasksCount);
        }else if(row > 0 && row < m_colMaxTasksCount){
            continue;
        }
        
        QPointF startPoint;
        startPoint.setX(startX);
        startPoint.setY(coordY);
        
        QPointF endPoint;
        endPoint.setX(endX);
        endPoint.setY(coordY);
        
        QLineF line(startPoint, endPoint);
        painter.drawLine(line);
    }


    
    painter.fillRect(m_matrixArea, Qt::red);
    if(!m_mousePos.isNull()){
        
        QPointF mousePos = m_mousePos - m_matrixArea.topLeft();
        int cellX = (int)(mousePos.rx() / m_cellDimension);
        int cellY = (int)(mousePos.ry() / m_cellDimension);
        
        qreal highlightX = m_horGridMargins + (m_rowMaxTasksCount + Q_REAL(cellX)) * m_cellDimension;
        qreal highlightY = m_verGridMargins + (m_colMaxTasksCount + Q_REAL(cellY)) * m_cellDimension;
        QRect hightlightedCell(highlightX, highlightY, m_cellDimension, m_cellDimension);
        
        painter.setBrush(m_highlightColor);
        painter.drawRect(hightlightedCell);

        //painter.fillRect(hightlightedCell, m_highlightColor);
    }
}

/*void GameMatrix::paintEvent(QPaintEvent* event){

    QPainter painter(this);
    QColor backgroundColor(40, 134, 222);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), backgroundColor);

    qreal cellWidth = ((qreal)rect().width()) / ((qreal)m_columns);
    qreal cellHeight = ((qreal)rect().height()) / ((qreal)m_rows);

    QPen pen(QColor(12, 59, 94), m_gridWidth);
    painter.setPen(pen);
    for(qreal w = 0; w < m_columns; w++){
        if(((int)w) % 5 == 0){
            pen.setWidth(m_gridWidth + 3);
            painter.setPen(pen);
        }else{
            pen.setWidth(m_gridWidth);
            painter.setPen(pen);
        }
        QLineF line;

        QPointF start;
        start.setX(w * ((qreal)this->rect().width() / (qreal)m_columns));
        start.setY(0);

        QPointF end;
        end.setX(w * ((qreal)this->rect().width() / (qreal)m_columns));
        end.setY(rect().height());

        line.setP1(start);
        line.setP2(end);

        painter.drawLine(line);

    }
    for(qreal h = 0; h < m_rows; h++){
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
        start.setY(h * ((qreal)this->rect().height() / (qreal)m_rows));

        QPointF end;
        end.setX(rect().width());
        end.setY(h * ((qreal)this->rect().height() / (qreal)m_rows));

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
    for(quint8 h = 0; h < m_rows; h++){
        for(quint8 w = 0; w < m_columns; w++){
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
    painter.setBrush(QColor(117, 29, 31));
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
            painter.setBrush(QColor(117, 29, 31));
            break;
        }
        painter.drawRects(selectedRects.data(), selectedRects.size());
        m_selectionBuffer.start_X = 0;
        m_selectionBuffer.start_Y = 0;
        m_selectionBuffer.end_X = 0;
        m_selectionBuffer.end_X = 0;
        m_selectionBuffer.m_valid = false;
    }

}*/

void GameMatrix::mousePressEvent(QMouseEvent *e){
    /*m_selectBegin = e->pos();
    qreal cellWidth = ((qreal)rect().width()) / ((qreal)m_columns);
    qreal cellHeight = ((qreal)rect().height()) / ((qreal)m_rows);
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
    }*/
    
    QWidget::mousePressEvent(e);
}

void GameMatrix::mouseReleaseEvent(QMouseEvent *e){
    /*QPointF startSelect;
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

    qreal cellWidth = ((qreal)rect().width()) / ((qreal)m_columns);
    qreal cellHeight = ((qreal)rect().height()) / ((qreal)m_rows);

    QRectF selectionRect(startSelect, endSelect);
    int startCellX = (int)((qreal)selectionRect.x() / (qreal)cellWidth);
    int startCellY = (int)((qreal)selectionRect.y() / (qreal)cellHeight);


    int endCellX = (int)((qreal)selectionRect.right() / (qreal)cellWidth);
    int endCellY = (int)((qreal)selectionRect.bottom() / (qreal)cellHeight);


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

    m_selectBegin = QPoint();*/
    
    repaint();
    QWidget::mouseReleaseEvent(e);
}

void GameMatrix::leaveEvent(QEvent* e){
    m_mousePos = QPoint();
    repaint();
    QWidget::leaveEvent(e);
}

void GameMatrix::mouseMoveEvent(QMouseEvent* e){
    if(m_matrixArea.contains(e->pos())){
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
            
            qreal cellWidth = ((qreal)rect().width()) / ((qreal)m_columns);
            qreal cellHeight = ((qreal)rect().height()) / ((qreal)m_rows);
            
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
    }else{
        m_mousePos = QPoint();
    }
    repaint();
    QWidget::mouseMoveEvent(e);
}
