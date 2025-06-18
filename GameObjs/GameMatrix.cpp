#include "GameMatrix.h"

#include <iostream>
#include <algorithm>
#include <execution>

#include <QPainter>
#include <QLine>
#include <QRect>
#include <QPen>
#include <QFont>
#include <QFontMetricsF>
#include <QSizePolicy>

#include "MatrixGenerator.h"
#include "ColorRepository.h"

#include <QDebug>

#define Q_REAL(x) ((qreal) x)

GameMatrix::GameMatrix(GameEngine* engine, QWidget *parent)
    : QWidget{parent}
    ,m_engine(engine)
    ,m_columns(engine->columns())
    ,m_rows(engine->rows())
    ,m_matrix(engine->rows(), std::vector<CellState>(engine->columns(), CellState::NEUTRAL))
    ,m_backGroundColor(255, 255, 255, 255)
    ,m_gridColor(0, 0, 0)
    ,m_checkColor(10, 46, 74)
    ,m_crossColor(117, 29, 31)
    ,m_matrixArea(0, 0, 0, 0)
    ,m_highlightColor(98, 171, 245)
{
    setMouseTracking(true);
    auto rowsNumbers = m_engine->rowsTasks();
    std::for_each(rowsNumbers.begin(), rowsNumbers.end(), [&](const std::vector<quint8> row){
        std::vector<Task> taskRow(0);
        std::for_each(row.begin(), row.end(), [&](const quint8& val){
            Task task;
            task.task = val;
            task.crossed = false;
            taskRow.push_back(task);
        });
        m_rowsTasks.push_back(taskRow);

    });

    std::for_each(m_rowsTasks.begin(), m_rowsTasks.end(), [&](const std::vector<Task> row){
        if(row.size() > m_rowMaxTasksCount){
            m_rowMaxTasksCount = (quint8) row.size();
        }
    });

    auto colsNumbers = m_engine->columnsTasks();
    std::for_each(colsNumbers.begin(), colsNumbers.end(), [&](const std::vector<quint8> col){
        std::vector<Task> taskCol(0);
        std::for_each(col.begin(), col.end(), [&](const quint8& val){
            Task task;
            task.task = val;
            task.crossed = false;
            taskCol.push_back(task);
        });
        m_colsTasks.push_back(taskCol);
    });
    std::for_each(m_colsTasks.begin(), m_colsTasks.end(), [&](const std::vector<Task> col){
        if(col.size() > m_colMaxTasksCount){
            m_colMaxTasksCount = (quint8) col.size();
        }
    });
    std::cout << "max row tasks : " << (int) m_rowMaxTasksCount << std::endl;
    std::cout << "max col tasks : " << (int) m_colMaxTasksCount << std::endl;

    connect(this, &GameMatrix::colTasksCrossed, this, &GameMatrix::checkAndCrossCol);
    connect(this, &GameMatrix::rowTasksCrossed, this, &GameMatrix::checkAndCrossRow);
}

int GameMatrix::heightForWidth(int w) const{
    return (qreal)w / ((qreal)m_columns / (qreal)m_rows);
}

void GameMatrix::matrixToBool(std::vector<std::vector<bool>>& matrix){
    matrix.clear();
    for(quint8 x = 0; x < m_matrix.size(); x++){
        std::vector<bool> row(0);
        for(quint8 y = 0; y < m_matrix[x].size(); y++){
            bool val = (m_matrix[x][y] == CellState::CHECKED);
            row.push_back(val);
        }
        matrix.push_back(row);
    }
}

void GameMatrix::restart(){
    std::for_each(std::execution::par_unseq, m_matrix.begin(), m_matrix.end(), [&](std::vector<CellState>& row){
        std::for_each(std::execution::par_unseq, row.begin(), row.end(), [&](CellState& cell){
            cell = NEUTRAL;
        });
    });

    std::for_each(std::execution::par_unseq, m_rowsTasks.begin(), m_rowsTasks.end(), [&](std::vector<Task>& vec){
        std::for_each(std::execution::par_unseq, vec.begin(), vec.end(), [&](Task& task){
            task.crossed = false;
        });
    });

    std::for_each(std::execution::par_unseq, m_colsTasks.begin(), m_colsTasks.end(), [&](std::vector<Task>& vec){
        std::for_each(std::execution::par_unseq, vec.begin(), vec.end(), [&](Task& task){
            task.crossed = false;
        });
    });

    update();
}

void GameMatrix::resizeGrid(){
    m_columns = m_engine->columns();
    m_rows = m_engine->rows();
    auto rowsNumbers = m_engine->rowsTasks();
    m_rowsTasks.clear();
    std::for_each(rowsNumbers.begin(), rowsNumbers.end(), [&](const std::vector<quint8> row){
        std::vector<Task> taskRow(0);
        std::for_each(row.begin(), row.end(), [&](const quint8& val){
            Task task;
            task.task = val;
            task.crossed = false;
            taskRow.push_back(task);
        });
        m_rowsTasks.push_back(taskRow);

    });

    m_rowMaxTasksCount = 0;
    std::for_each(m_rowsTasks.begin(), m_rowsTasks.end(), [&](const std::vector<Task> row){
        if(row.size() > m_rowMaxTasksCount){
            m_rowMaxTasksCount = (quint8) row.size();
        }
    });

    auto colsNumbers = m_engine->columnsTasks();
    m_colsTasks.clear();
    std::for_each(colsNumbers.begin(), colsNumbers.end(), [&](const std::vector<quint8> col){
        std::vector<Task> taskCol(0);
        std::for_each(col.begin(), col.end(), [&](const quint8& val){
            Task task;
            task.task = val;
            task.crossed = false;
            taskCol.push_back(task);
        });
        m_colsTasks.push_back(taskCol);
    });

    m_colMaxTasksCount = 0;
    std::for_each(m_colsTasks.begin(), m_colsTasks.end(), [&](const std::vector<Task> col){
        if(col.size() > m_colMaxTasksCount){
            m_colMaxTasksCount = (quint8) col.size();
        }
    });
    m_matrix.clear();
    m_matrix = std::vector<std::vector<CellState>>(m_rows, std::vector<CellState>(m_columns, CellState::NEUTRAL));

    emit resizeEvent(nullptr);
    repaint();
}

bool GameMatrix::pointInGameAreas(const QPointF& pos) const{
    return (m_matrixArea.contains(pos) || m_colTasksArea.contains(pos) ||
            m_leftRowTasksArea.contains(pos) || m_rightRowTasksArea.contains(pos));
}

QPoint GameMatrix::getCellCoord(const QPointF& pos) const{
    if(!pointInGameAreas(pos)){
        return QPoint();
    }
    int cellX = (pos.x() - m_horGridMargins) / m_cellDimension;
    int cellY = (pos.y() - m_verGridMargins) / m_cellDimension;
    QPoint point(cellX, cellY);
    return point;
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

    /// Background
    QPainter painter(this);
    painter.fillRect(widgetRect, QColor("transparent"));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QFont font = painter.font();
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setWeight(QFont::DemiBold);
    font.setPixelSize(m_fontSize);
    painter.setFont(font);

    //painter.fillRect(rect(), m_backGroundColor);

    /// Grid
    QPen gridPen;
    gridPen.setColor(m_gridColor);
    gridPen.setWidth(1);
    painter.setPen(gridPen);
    painter.fillRect(m_matrixArea, m_backGroundColor);
    painter.fillRect(m_colTasksArea, m_backGroundColor);
    painter.fillRect(m_leftRowTasksArea, m_backGroundColor);
    painter.fillRect(m_rightRowTasksArea, m_backGroundColor);
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
            gridPen.setWidthF(1);
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
        qreal crossMargins = m_cellDimension / 4;
        for(quint8 col = 0; col < m_colsTasks.size(); col++){
            int diff = m_colMaxTasksCount - m_colsTasks[col].size();
            //qreal colY = m_verGridMargins + Q_REAL(diff + 1) * m_cellDimension - painter.font().pixelSize() / 2;
            qreal colY = m_verGridMargins + Q_REAL(diff + 1) * m_cellDimension;
            for(quint8 cell = 0; cell < m_colsTasks[col].size(); cell++){
                QString numText = QString::number(m_colsTasks[col][cell].task);
                qreal textColY = colY - painter.font().pixelSize() / 2;
                qreal textWidth = fontMetric.horizontalAdvance(numText);
                qreal textColX = colX + (m_cellDimension - textWidth) / 2;

                //painter.setPen(ColorRepository::textColor());
                painter.drawText(QPointF(textColX, textColY), numText);

                // TODO check if crossed and paint cross if true
                if(m_colsTasks[col][cell].crossed){
                    QPointF firstLineTopLeft(colX + crossMargins, colY - m_cellDimension + crossMargins);
                    QPointF firstLineBottomRight(colX + m_cellDimension - crossMargins, colY - crossMargins);

                    QPointF secondLineBottomLeft(colX + crossMargins, colY - crossMargins);
                    QPointF secondLineTopRight(colX + m_cellDimension - crossMargins, colY - m_cellDimension + crossMargins);

                    QPen currentPen = painter.pen();
                    QPen crossPen(m_crossColor);
                    crossPen.setWidth(m_cellDimension / 12);
                    painter.setPen(crossPen);
                    painter.drawLine(firstLineTopLeft, firstLineBottomRight);
                    painter.drawLine(secondLineBottomLeft, secondLineTopRight);
                    painter.setPen(currentPen);
                }


                colY += m_cellDimension;
            }
            colX += m_cellDimension;
        }
    }
    {
        /// Rows tasks
        //qreal colY = m_verGridMargins + (m_cellDimension * (m_colMaxTasksCount + 1)) - painter.font().pixelSize() / 2;
        qreal colY = m_verGridMargins + (m_cellDimension * (m_colMaxTasksCount + 1));
        qreal crossMargins = m_cellDimension / 4;
        for(quint8 row = 0; row < m_rowsTasks.size(); row++){
            int diff = m_rowMaxTasksCount - m_rowsTasks[row].size();
            qreal col1X = m_horGridMargins + Q_REAL(diff) * m_cellDimension;
            qreal col2X = m_horGridMargins + (m_columns + m_rowMaxTasksCount) * m_cellDimension;
            for(quint8 cell = 0; cell < m_rowsTasks[row].size(); cell++){
                QString numText1 = QString::number(m_rowsTasks[row][cell].task);
                qreal textColY = colY - painter.font().pixelSize() / 2;
                qreal textWidth1 = fontMetric.horizontalAdvance(numText1);
                qreal textCol1X = col1X + (m_cellDimension - textWidth1) / 2;

                //painter.setPen(ColorRepository::textColor());
                painter.drawText(QPointF(textCol1X, textColY), numText1);

                // TODO check if crossed and paint cross if true
                if(m_rowsTasks[row][cell].crossed){
                    QPointF firstLineTopLeft(col1X + crossMargins, colY - m_cellDimension + crossMargins);
                    QPointF firstLineBottomRight(col1X + m_cellDimension - crossMargins, colY - crossMargins);

                    QPointF secondLineBottomLeft(col1X + crossMargins, colY - crossMargins);
                    QPointF secondLineTopRight(col1X + m_cellDimension - crossMargins, colY - m_cellDimension + crossMargins);

                    QPen currentPen = painter.pen();
                    QPen crossPen(m_crossColor);
                    crossPen.setWidth(m_cellDimension / 12);
                    painter.setPen(crossPen);
                    painter.drawLine(firstLineTopLeft, firstLineBottomRight);
                    painter.drawLine(secondLineBottomLeft, secondLineTopRight);
                    painter.setPen(currentPen);
                }
                col1X += m_cellDimension;


                QString numText2 = QString::number(m_rowsTasks[row][cell].task);
                qreal textWidth2  = fontMetric.horizontalAdvance(numText2);
                qreal tmpCol2X = col2X + (m_cellDimension - textWidth2) / 2;
                painter.drawText(QPointF(tmpCol2X, textColY), numText2);

                // TODO check if crossed and paint cross if true
                if(m_rowsTasks[row][cell].crossed){
                    QPointF firstLineTopLeft(col2X + crossMargins, colY - m_cellDimension + crossMargins);
                    QPointF firstLineBottomRight(col2X + m_cellDimension - crossMargins, colY - crossMargins);

                    QPointF secondLineBottomLeft(col2X + crossMargins, colY - crossMargins);
                    QPointF secondLineTopRight(col2X + m_cellDimension - crossMargins, colY - m_cellDimension + crossMargins);

                    QPen currentPen = painter.pen();
                    QPen crossPen(m_crossColor);
                    crossPen.setWidth(m_cellDimension / 12);
                    painter.setPen(crossPen);
                    painter.drawLine(firstLineTopLeft, firstLineBottomRight);
                    painter.drawLine(secondLineBottomLeft, secondLineTopRight);
                    painter.setPen(currentPen);
                }
                col2X += m_cellDimension;
            }
            colY += m_cellDimension;
        }
    }

    /// Cells
    qreal cellMargins = m_cellDimension - (m_cellDimension * 0.90);
    {
        std::vector<QRectF> checkRects;
        std::vector<QLineF> crossedLines;
        for(quint8 h = 0; h < m_rows; h++){
            for(quint8 w = 0; w < m_columns; w++){
                if(m_matrix[h][w] != CellState::NEUTRAL){
                    QRectF cell;
                    cell.setX(m_horGridMargins + ((w + m_rowMaxTasksCount) * m_cellDimension) + cellMargins);
                    cell.setY(m_verGridMargins + ((h + m_colMaxTasksCount) * m_cellDimension) + cellMargins);
                    cell.setWidth(m_cellDimension - (cellMargins * 2));
                    cell.setHeight(m_cellDimension - (cellMargins * 2));
                    if(m_matrix[h][w] == CellState::CHECKED){
                        checkRects.push_back(cell);
                    }else if(m_matrix[h][w] == CellState::CROSSED){
                        qreal crossMargin = m_cellDimension * 0.33;
                        qreal topLeftX = m_horGridMargins + ((w + m_rowMaxTasksCount) * m_cellDimension) + crossMargin;
                        qreal topLeftY = m_verGridMargins + ((h + m_colMaxTasksCount) * m_cellDimension) + crossMargin;
                        qreal bottomRightX = topLeftX + m_cellDimension - (crossMargin * 2);
                        qreal bottomRightY = topLeftY + m_cellDimension - (crossMargin * 2);
                        QPointF topLeft(topLeftX, topLeftY);
                        QPointF bottomRight(bottomRightX, bottomRightY);
                        QLineF firstLine(topLeft, bottomRight);

                        qreal bottomLeftX = topLeftX;
                        qreal bottomLeftY = bottomRightY;
                        qreal topRightX = bottomRightX;
                        qreal topRightY = topLeftY;
                        QPointF bottomLeft(bottomLeftX, bottomLeftY);
                        QPointF topRight(topRightX, topRightY);
                        QLineF secondLine(bottomLeft, topRight);

                        crossedLines.push_back(firstLine);
                        crossedLines.push_back(secondLine);
                    }
                }
            }
        }
        QPen noBorderPen = painter.pen();
        noBorderPen.setWidth(0);
        noBorderPen.setStyle(Qt::PenStyle::NoPen);
        painter.setPen(noBorderPen);

        painter.setBrush(m_checkColor);
        painter.drawRects(checkRects.data(), checkRects.size());

        QPen currentPen = painter.pen();

        QPen crossPen(m_crossColor);
        crossPen.setWidth(m_cellDimension / 8);
        painter.setPen(crossPen);

        painter.setBrush(m_crossColor);
        painter.drawLines(crossedLines.data(), crossedLines.size());

        painter.setPen(currentPen);

    }

    /// Paint selection if any
    if(m_selectionBuffer.valid){
        if(m_selectionBuffer.area == MATRIX){
            std::vector<QRectF> selectedRects(0);
            std::vector<QLineF> crossedLines(0);
            int startX = std::min(m_selectionBuffer.startCell.x(), m_selectionBuffer.endCell.x());
            int startY = std::min(m_selectionBuffer.startCell.y(), m_selectionBuffer.endCell.y());
            int endX = std::max(m_selectionBuffer.startCell.x(), m_selectionBuffer.endCell.x());
            int endY = std::max(m_selectionBuffer.startCell.y(), m_selectionBuffer.endCell.y());

            for(quint8 h = startY; h <= endY; h++){
                for(quint8 w = startX; w <= endX; w++){

                    QRectF cell;
                    cell.setX(m_horGridMargins + (w * m_cellDimension) + cellMargins);
                    cell.setY(m_verGridMargins + (h * m_cellDimension) + cellMargins);
                    cell.setWidth(m_cellDimension - (cellMargins * 2));
                    cell.setHeight(m_cellDimension - (cellMargins * 2));
                    selectedRects.push_back(cell);
                    if(m_selectionBuffer.actionMode == CROSSING){
                        qreal crossMarginValue = m_cellDimension * 0.33;
                        QLineF antiSlash(QPointF(m_horGridMargins + (w * m_cellDimension) + crossMarginValue, m_verGridMargins + (h * m_cellDimension) + crossMarginValue),
                                         QPointF(m_horGridMargins + ((w + 1) * m_cellDimension) - crossMarginValue, m_verGridMargins + ((h + 1) * m_cellDimension) - crossMarginValue));
                        QLineF slash(QPointF(m_horGridMargins + (w * m_cellDimension) + crossMarginValue, m_verGridMargins + ((h + 1) * m_cellDimension) - crossMarginValue),
                                     QPointF(m_horGridMargins + ((w + 1) * m_cellDimension) - crossMarginValue, m_verGridMargins + (h * m_cellDimension) + crossMarginValue));

                        crossedLines.push_back(antiSlash);
                        crossedLines.push_back(slash);
                    }
                }
            }
            switch(m_selectionBuffer.actionMode){
            case VOIDING:
                painter.setBrush(m_backGroundColor);
                painter.drawRects(selectedRects.data(), selectedRects.size());
                break;
            case CHECKING:
                painter.setBrush(m_checkColor);
                painter.drawRects(selectedRects.data(), selectedRects.size());
                break;
            case CROSSING:
            {
                painter.setBrush(m_backGroundColor);
                painter.drawRects(selectedRects.data(), selectedRects.size());

                QPen crossPen(m_crossColor);
                crossPen.setWidth(m_cellDimension / 8);
                painter.setPen(crossPen);
                painter.setBrush(m_crossColor);
                painter.drawLines(crossedLines.data(), crossedLines.size());
            }break;
            }
        }
    }

    /// For Debug
    //painter.fillRect(m_colTasksArea, Qt::red);
    //painter.fillRect(m_leftRowTasksArea, Qt::red);
    //painter.fillRect(m_rightRowTasksArea, Qt::red);

    /// Paint highlight
    if(!m_mousePos.pos.isNull()){
        QPen highlightPen(m_highlightColor);
        highlightPen = m_highlightColor;
        QRectF currentRect;
        switch(m_mousePos.area){
        case MATRIX:
            currentRect.setRect(m_matrixArea.x(), m_matrixArea.y(), m_matrixArea.width(), m_matrixArea.height());
            break;
        case TOP_TASKS:
            currentRect.setRect(m_colTasksArea.x(), m_colTasksArea.y(), m_colTasksArea.width(), m_colTasksArea.height());
            break;
        case LEFT_TASKS:
            currentRect.setRect(m_leftRowTasksArea.x(), m_leftRowTasksArea.y(), m_leftRowTasksArea.width(), m_leftRowTasksArea.height());
            break;
        case RIGHT_TASKS:
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
        painter.setBrush(QColor(0, 0, 0, 0));
        painter.drawRect(hightlightedCell);
    }
}

void GameMatrix::mousePressEvent(QMouseEvent *e){

    QPoint cellCoord = getCellCoord(m_mousePos.pos);
    if(cellCoord.isNull()){
        m_selectionBuffer.startCell = QPoint();
        m_selectionBuffer.endCell = QPoint();
        m_selectionBuffer.area = NONE;
        m_selectionBuffer.valid = false;
        repaint();
        QWidget::mousePressEvent(e);
        return;
    }

    m_selectionBuffer.startCell = cellCoord;
    m_selectionBuffer.endCell = cellCoord;
    m_selectionBuffer.area = m_mousePos.area;

    if(m_mousePos.area == MATRIX){
        m_selectionBuffer.valid = true;
        QPoint relativeCellCoord = cellCoord;
        relativeCellCoord.setX(relativeCellCoord.x() - m_rowMaxTasksCount);
        relativeCellCoord.setY(relativeCellCoord.y() - m_colMaxTasksCount);
        CellState cellState = m_matrix[relativeCellCoord.y()][relativeCellCoord.x()];
        if(e->button() == Qt::LeftButton){
            if(cellState == CHECKED){
                m_selectionBuffer.actionMode = VOIDING;
            }else{
                m_selectionBuffer.actionMode = CHECKING;
            }
        }else if(e->button() == Qt::RightButton){
            if(cellState == CROSSED){
                m_selectionBuffer.actionMode = VOIDING;
            }else{
                m_selectionBuffer.actionMode = CROSSING;
            }
        }
    }else if(m_mousePos.area == TOP_TASKS){
        QPoint relativeCellCoord = cellCoord;
        relativeCellCoord.setX(relativeCellCoord.x() - m_rowMaxTasksCount);

        int diff = m_colMaxTasksCount - m_colsTasks[relativeCellCoord.x()].size();
        if(relativeCellCoord.y() >= (diff)){
            m_selectionBuffer.valid = true;
            relativeCellCoord.setY(relativeCellCoord.y() - diff);
            if(m_colsTasks[relativeCellCoord.x()][relativeCellCoord.y()].crossed){
                m_selectionBuffer.actionMode = VOIDING;
            }else{
                m_selectionBuffer.actionMode = CROSSING;
            }
        }else{ // Clicked on blank cell
            m_selectionBuffer.valid = false;
        }
    }else if(m_mousePos.area == LEFT_TASKS){
        QPoint relativeCellCoord = cellCoord;
        relativeCellCoord.setY(relativeCellCoord.y() - m_colMaxTasksCount);
        int diff = m_rowMaxTasksCount - m_rowsTasks[relativeCellCoord.y()].size();
        if(relativeCellCoord.x() >= diff){
            m_selectionBuffer.valid = true;
            relativeCellCoord.setX(relativeCellCoord.x() - diff);
            if(m_rowsTasks[relativeCellCoord.y()][relativeCellCoord.x()].crossed){
                m_selectionBuffer.actionMode = VOIDING;
            }else{
                m_selectionBuffer.actionMode = CROSSING;
            }
        }else{ // Clicked on blank cell
            m_selectionBuffer.valid = false;
        }
    }else if(m_mousePos.area == RIGHT_TASKS){
        QPoint relativeCellCoord = cellCoord;
        relativeCellCoord.setY(relativeCellCoord.y() - m_colMaxTasksCount);
        relativeCellCoord.setX(relativeCellCoord.x() - (m_rowMaxTasksCount + m_columns));
        if(relativeCellCoord.x() < m_rowsTasks[relativeCellCoord.y()].size()){
            m_selectionBuffer.valid = true;
            if(m_rowsTasks[relativeCellCoord.y()][relativeCellCoord.x()].crossed){
                m_selectionBuffer.actionMode = VOIDING;
            }else{
                m_selectionBuffer.actionMode = CROSSING;
            }
        }else{ // Clicked on blank cell
            m_selectionBuffer.valid = false;
        }
    }
    repaint();
    QWidget::mousePressEvent(e);
}

void GameMatrix::mouseReleaseEvent(QMouseEvent *e){
    int startX = std::min(m_selectionBuffer.startCell.x(), m_selectionBuffer.endCell.x());
    int startY = std::min(m_selectionBuffer.startCell.y(), m_selectionBuffer.endCell.y());
    int endX = std::max(m_selectionBuffer.startCell.x(), m_selectionBuffer.endCell.x());
    int endY = std::max(m_selectionBuffer.startCell.y(), m_selectionBuffer.endCell.y());
    if(m_selectionBuffer.area == MATRIX){
        startX -= m_rowMaxTasksCount;
        endX -= m_rowMaxTasksCount;
        startY -= m_colMaxTasksCount;
        endY -= m_colMaxTasksCount;
        QRect selectionArea(startX, startY, (endX - startX) + 1, (endY - startY) + 1);
        for(int h = startY; h <= selectionArea.bottom(); h++){
            for(int w = startX; w <= selectionArea.right(); w++){
                switch(m_selectionBuffer.actionMode){
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
        std::vector<std::vector<bool>> matrix;
        this->matrixToBool(matrix);
        m_engine->validateGrid(matrix);
    }else if(m_selectionBuffer.area == TOP_TASKS){
        startX -= m_rowMaxTasksCount;
        if(m_selectionBuffer.valid){
            quint8 diffY = m_colMaxTasksCount - m_colsTasks[startX].size();
            if(startY < diffY){
                startY = 0;
                endY -= (diffY - startY);
            }else{
                startY -= diffY;
                endY -= diffY;
            }
            /*for(int h = startY; h <= endY ; h++){
                m_colsTasks[startX][h].crossed = m_selectionBuffer.actionMode == CROSSING;
            }*/
            crossColTasks(startX, startY, endY, m_selectionBuffer.actionMode == CROSSING);
        }
    }else if(m_mousePos.area == LEFT_TASKS){
        startY -= m_colMaxTasksCount;
        if(m_selectionBuffer.valid){
            quint8 diffX = m_rowMaxTasksCount - m_rowsTasks[startY].size();
            if(startX < diffX){
                startX = 0;
                endX -= (diffX - startX);
            }else{
                startX -= diffX;
                endX -= diffX;
            }
            /*for(int w = startX; w <= endX; w++){
                m_rowsTasks[startY][w].crossed = m_selectionBuffer.actionMode == CROSSING;
            }*/
            crossRowTasks(startY, startX, endX, m_selectionBuffer.actionMode == CROSSING);
        }
    }else if(m_mousePos.area == RIGHT_TASKS){
        startY -= m_colMaxTasksCount;
        startX -= (m_columns + m_rowMaxTasksCount);
        endX -= (m_columns + m_rowMaxTasksCount);
        if(m_selectionBuffer.valid){
            if(endX >= m_rowsTasks[startY].size()){
                endX = m_rowsTasks[startY].size() - 1;
            }
            /*for(int w = startX; w <= endX; w++){
                m_rowsTasks[startY][w].crossed = m_selectionBuffer.actionMode == CROSSING;
            }*/
            crossRowTasks(startY, startX, endX, m_selectionBuffer.actionMode == CROSSING);
        }
    }

    m_selectionBuffer.startCell = QPoint();
    m_selectionBuffer.endCell = QPoint();
    m_selectionBuffer.area = NONE;
    m_selectionBuffer.valid = false;
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
    if(pointInGameAreas(e->pos())){
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
            if(m_selectionBuffer.area == MATRIX && m_matrixArea.contains(e->pos())){
                m_selectionBuffer.endCell = getCellCoord(e->pos());
                //qDebug() << "Selection dim : " << std::abs(m_selectionBuffer.startCell.x() - m_selectionBuffer.endCell.x()) << ", " << std::abs(m_selectionBuffer.startCell.y() - m_selectionBuffer.endCell.y());
            }else if(m_selectionBuffer.area == TOP_TASKS && m_colTasksArea.contains(e->pos())){
                m_selectionBuffer.endCell.setY(getCellCoord(e->pos()).y());
                //qDebug() << "Selection dim : " << std::abs(m_selectionBuffer.startCell.x() - m_selectionBuffer.endCell.x()) << ", " << std::abs(m_selectionBuffer.startCell.y() - m_selectionBuffer.endCell.y());
            }else if((m_selectionBuffer.area == LEFT_TASKS && m_leftRowTasksArea.contains(e->pos())) ||
                       (m_selectionBuffer.area == RIGHT_TASKS && m_rightRowTasksArea.contains(e->pos()))){
                m_selectionBuffer.endCell.setX(getCellCoord(e->pos()).x());
                //qDebug() << "Selection dim : " << std::abs(m_selectionBuffer.startCell.x() - m_selectionBuffer.endCell.x()) << ", " << std::abs(m_selectionBuffer.startCell.y() - m_selectionBuffer.endCell.y());
            }
        }
    }else{
        m_mousePos.pos = QPoint();
        m_mousePos.area = Area::NONE;
    }
    repaint();
    QWidget::mouseMoveEvent(e);
}

void GameMatrix::crossColTasks(size_t col, quint8 start, quint8 end, bool cross){
    for(int h = start; h <= end; h++){
        m_colsTasks[col][h].crossed = cross;
    }
    bool allCrossed = std::all_of(m_colsTasks[col].begin(), m_colsTasks[col].end(), [](const Task& task){
        return (task.crossed == true);
    });
    if(allCrossed){
        //qDebug() << "Column " << col << " tasks are all crossed";
        emit colTasksCrossed(col);
    }
}

void GameMatrix::crossRowTasks(size_t row, quint8 start, quint8 end, bool cross){
    for(int w = start; w <= end; w++){
        m_rowsTasks[row][w].crossed = cross;
    }
    bool allCrossed = std::all_of(m_rowsTasks[row].begin(), m_rowsTasks[row].end(), [](const Task& task){
        return (task.crossed == true);
    });
    if(allCrossed){
        //qDebug() << "Row " << row << " tasks are all crossed";
        emit rowTasksCrossed(row);
    }
}

void GameMatrix::checkAndCrossCol(size_t col){
    if(!isCheckableColAgainstTasksCross(col)){
        return;
    }
    for(quint8 row = 0; row < m_rows; row++){
        if(m_matrix[row][col] == NEUTRAL){
            m_matrix[row][col] = CROSSED;
        }
    }
}

void GameMatrix::checkAndCrossRow(size_t row){
    if(!isCheckableRowAgainstTasksCross(row)){
        return;
    }
    for(quint8 col = 0; col < m_columns; col++){
        if(m_matrix[row][col] == NEUTRAL){
            m_matrix[row][col] = CROSSED;
        }
    }
}

bool GameMatrix::isCheckableColAgainstTasksCross(size_t col){
    std::vector<Task>& colTask = m_colsTasks.at(col);

    // Construct a vector of checked values from the matrix
    std::vector<quint8> checkedVector(0);
    bool previousChecked = false;
    quint8 counter = 0;
    for(size_t row = 0; row < m_rows; row++){
        if(m_matrix[row][col] != CHECKED){
            if(previousChecked){
                checkedVector.push_back(counter);
                previousChecked = false;
                counter = 0;
            }
        }else{
            previousChecked = true;
            counter++;
        }
    }
    // last item
    if(previousChecked){
        checkedVector.push_back(counter);
    }

    // Check with task
    if(colTask.size() != checkedVector.size()){
        return false;
    }
    for(size_t i = 0; i < colTask.size(); i++){
        if(colTask[i].task != checkedVector[i]){
            return false;
        }
    }

    return true;
}

bool GameMatrix::isCheckableRowAgainstTasksCross(size_t row){
    std::vector<Task>& rowTask = m_rowsTasks.at(row);

    // Construct a vector of checked values from the matrix
    std::vector<quint8> checkedVector(0);
    bool previousChecked = false;
    quint8 counter = 0;
    for(size_t col = 0; col < m_columns; col++){
        if(m_matrix[row][col] != CHECKED){
            if(previousChecked){
                checkedVector.push_back(counter);
                previousChecked = false;
                counter = 0;
            }
        }else{
            previousChecked = true;
            counter++;
        }
    }
    // last item
    if(previousChecked){
        checkedVector.push_back(counter);
    }

    // Check with task
    if(rowTask.size() != checkedVector.size()){
        return false;
    }
    for(size_t i = 0; i < rowTask.size(); i++){
        if(rowTask[i].task != checkedVector[i]){
            return false;
        }
    }
    return true;
}
