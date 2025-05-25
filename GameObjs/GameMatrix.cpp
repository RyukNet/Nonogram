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
    //,m_mousePos((0, 0), NONE)
    ,m_backGroundColor(255, 0, 0, 0)
    ,m_gridColor(0, 0, 0)
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

    m_colTasksArea = widgetRect;
    m_colTasksArea.setX(m_matrixArea.x());
    m_colTasksArea.setY(widgetRect.y() + m_verGridMargins);
    m_colTasksArea.setWidth(m_matrixArea.width());
    m_colTasksArea.setHeight(m_colMaxTasksCount * m_cellDimension);

    m_leftRowTasksArea = widgetRect;
    m_leftRowTasksArea.setX(widgetRect.x() + m_horGridMargins);
    m_leftRowTasksArea.setY(m_matrixArea.y());
    m_leftRowTasksArea.setWidth(m_rowMaxTasksCount * m_cellDimension);
    m_leftRowTasksArea.setHeight(m_matrixArea.height());

    m_rightRowTasksArea = widgetRect;
    m_rightRowTasksArea.setX(m_matrixArea.x() + (m_columns * m_cellDimension));
    m_rightRowTasksArea.setWidth(m_rowMaxTasksCount * m_cellDimension);
    m_rightRowTasksArea.setY(m_matrixArea.y());
    m_rightRowTasksArea.setHeight(m_matrixArea.height());


    m_fontSize = m_cellDimension * 0.64;

    QWidget::resizeEvent(event);
}

void GameMatrix::paintEvent(QPaintEvent* event){
    QRect widgetRect = rect();
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QFont font = painter.font();
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setWeight(QFont::DemiBold);
    font.setPixelSize(m_fontSize);
    painter.setFont(font);
    
    /// Background
    painter.fillRect(rect(), m_backGroundColor);
    
    /// Grid
    QPen gridPen;
    gridPen.setColor(m_gridColor);
    gridPen.setWidth(1);
    painter.setPen(gridPen);
    //painter.setBrush(m_gridColor);
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

        if(col == 0 || std::abs(col - m_rowMaxTasksCount) % 5 == 0 || (col == m_rowMaxTasksCount * 2 + m_columns)){
            gridPen.setWidthF(2);
        }else{
            gridPen.setWidthF(0.4);
        }
        painter.setPen(gridPen);
        
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
        if(row == 0 || std::abs(row - m_colMaxTasksCount) % 5 == 0 || (row == m_colMaxTasksCount + m_rows)){
            gridPen.setWidthF(2);
        }else{
            gridPen.setWidthF(0.4);
        }
        painter.setPen(gridPen);
        
        QPointF startPoint;
        startPoint.setX(startX);
        startPoint.setY(coordY);
        
        QPointF endPoint;
        endPoint.setX(endX);
        endPoint.setY(coordY);
        
        QLineF line(startPoint, endPoint);
        painter.drawLine(line);
    }

    QFontMetricsF fontMetric(painter.font());

    /// Write Tasks
    {
        /// Columns tasks
        qreal colX = m_horGridMargins + (m_cellDimension * m_rowMaxTasksCount);
        for(quint8 col = 0; col < m_colsTasks.size(); col++){
            int diff = m_colMaxTasksCount - m_colsTasks[col].size();
            qreal colY = m_verGridMargins + Q_REAL(diff + 1) * m_cellDimension - painter.font().pixelSize() / 2;
            for(quint8 cell = 0; cell < m_colsTasks[col].size(); cell++){
                QString numText = QString::number(m_colsTasks[col][cell]);
                qreal textWidth = fontMetric.horizontalAdvance(numText);
                qreal tmpColX = colX + (m_cellDimension - textWidth) / 2;

                painter.drawText(QPointF(tmpColX, colY), numText);
                colY += m_cellDimension;
            }
            colX += m_cellDimension;
        }
    }
    {
        /// Rows tasks
        qreal colY = m_verGridMargins + (m_cellDimension * (m_colMaxTasksCount + 1)) - painter.font().pixelSize() / 2;
        for(quint8 row = 0; row < m_rowsTasks.size(); row++){
            int diff = m_rowMaxTasksCount - m_rowsTasks[row].size();
            qreal col1X = m_horGridMargins + Q_REAL(diff) * m_cellDimension;
            qreal col2X = m_horGridMargins + (m_columns + m_rowMaxTasksCount) * m_cellDimension;
            for(quint8 cell = 0; cell < m_rowsTasks[row].size(); cell++){
                QString numText1 = QString::number(m_rowsTasks[row][cell]);
                qreal textWidth1 = fontMetric.horizontalAdvance(numText1);
                qreal tmpCol1X = col1X + (m_cellDimension - textWidth1) / 2;
                painter.drawText(QPointF(tmpCol1X, colY), numText1);
                col1X += m_cellDimension;

                QString numText2 = QString::number(m_rowsTasks[row][cell]);
                qreal textWidth2  = fontMetric.horizontalAdvance(numText2);
                qreal tmpCol2X = col2X +(m_cellDimension - textWidth2) / 2;

                painter.drawText(QPointF(tmpCol2X, colY), numText2);
                col2X += m_cellDimension;
            }
            colY += m_cellDimension;
        }
    }

    /// For Debug
    //painter.fillRect(m_colTasksArea, Qt::red);
    //painter.fillRect(m_leftRowTasksArea, Qt::red);
    //painter.fillRect(m_rightRowTasksArea, Qt::red);

    /// Paint highlight
    if(!m_mousePos.pos.isNull()){
        QPen highlightPen(QColor(255, 255, 0));
        QRectF currentRect;
        switch(m_mousePos.area){
        case MATRIX:
            highlightPen = m_highlightColor;
            currentRect.setRect(m_matrixArea.x(), m_matrixArea.y(), m_matrixArea.width(), m_matrixArea.height());
            break;
        case TOP_TASKS:
            highlightPen.setColor(QColor(255, 0, 0));
            currentRect.setRect(m_colTasksArea.x(), m_colTasksArea.y(), m_colTasksArea.width(), m_colTasksArea.height());
            break;
        case LEFT_TASKS:
            highlightPen.setColor(QColor(0, 255, 0));
            currentRect.setRect(m_leftRowTasksArea.x(), m_leftRowTasksArea.y(), m_leftRowTasksArea.width(), m_leftRowTasksArea.height());
            break;
        case RIGHT_TASKS :
            highlightPen.setColor(QColor(0, 0, 255));
            currentRect.setRect(m_rightRowTasksArea.x(), m_rightRowTasksArea.y(), m_rightRowTasksArea.width(), m_rightRowTasksArea.height());
            break;
        default:
            break;
        }
        
        QPointF mousePos = m_mousePos.pos - currentRect.topLeft();
        int cellX = (int)(mousePos.rx() / m_cellDimension);
        int cellY = (int)(mousePos.ry() / m_cellDimension);
        
        qreal highlightX = currentRect.x() + (Q_REAL(cellX)) * m_cellDimension;
        qreal highlightY = currentRect.y() + (Q_REAL(cellY)) * m_cellDimension;
        QRect hightlightedCell(highlightX, highlightY, m_cellDimension + 2, m_cellDimension + 2);
        
        painter.setPen(highlightPen);
        painter.drawRect(hightlightedCell);
    }
}

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
    m_mousePos.pos = QPoint();
    m_mousePos.area = Area::NONE;
    repaint();
    QWidget::leaveEvent(e);
}

void GameMatrix::mouseMoveEvent(QMouseEvent* e){
    if(m_matrixArea.contains(e->pos()) || m_colTasksArea.contains(e->pos()) ||
        m_leftRowTasksArea.contains(e->pos()) || m_rightRowTasksArea.contains(e->pos())){
        m_mousePos.pos = e->pos();
        if(m_matrixArea.contains(m_mousePos.pos)){
            m_mousePos.area = Area::MATRIX;
        }else if(m_colTasksArea.contains(m_mousePos.pos)){
            m_mousePos.area = Area::TOP_TASKS;
        }else if(m_leftRowTasksArea.contains(m_mousePos.pos)){
            m_mousePos.area = Area::LEFT_TASKS;
        }else if(m_rightRowTasksArea.contains(m_mousePos.pos)){
            m_mousePos.area = Area::RIGHT_TASKS;
        }
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
        m_mousePos.pos = QPoint();
        m_mousePos.area = Area::NONE;
    }
    repaint();
    QWidget::mouseMoveEvent(e);
}
