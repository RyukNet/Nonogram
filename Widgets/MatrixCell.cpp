#include "MatrixCell.h"

#include <QPainter>
#include <QPainterPath>
#include <QPen>


#include <QDebug>

MatrixCell::MatrixCell(QWidget *parent)
    : QAbstractButton(parent)
    ,m_neutralColor(150, 150, 150, 0)
    ,m_neutralEnteredColor(150, 150, 150, 50)
    ,m_neutralClickedColor(150, 150, 150, 30)

    ,m_checkedColor(71, 158, 245)
    ,m_checkedEnteredColor(98, 171, 245)
    ,m_checkedClickedColor(40, 134, 222)

    ,m_crossColor(197, 15, 31)
    ,m_crossEnteredColor(177, 14, 28)
    ,m_crossClickedColor(150, 11, 24)

    ,m_state(MatrixCell::NEUTRAL)
    ,m_currentColor(m_neutralColor)
{
    setMouseTracking(true);
    setMinimumSize(24, 24);
}


void MatrixCell::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.addRoundedRect(rect(), 4, 4);

    QPen pen;
    painter.setPen(pen);
    /*switch(m_state){
    case NEUTRAL:
        //m_currentColor = m_neutralColor;
        m_currentColor = m_checkedColor;
        break;
    case CHECKED:
        m_currentColor = m_checkedColor;
        break;
    case CROSS:
        m_currentColor = m_crossColor;
        break;
    }*/
    painter.fillPath(path, m_currentColor);


}

bool MatrixCell::event(QEvent *e){
    QMouseEvent* me = reinterpret_cast<QMouseEvent*>(e);
    //if(me != nullptr){
        qDebug() << this->text() << "Direct";
    //}
    return QAbstractButton::event(e);
}

void MatrixCell::enterEvent(QEvent *e){
    QMouseEvent* me = reinterpret_cast<QMouseEvent*>(e);
    assert(me != nullptr);

    switch(m_state){
    case NEUTRAL:
        m_currentColor = m_neutralEnteredColor;
        break;
    case CHECKED:
        m_currentColor = m_checkedEnteredColor;
        break;
    case CROSS:
        m_currentColor = m_crossEnteredColor;
        break;
    }
    repaint();
    QAbstractButton::enterEvent(e);
}

void MatrixCell::leaveEvent(QEvent *e){
    switch(m_state){
    case NEUTRAL:
        m_currentColor = m_neutralColor;
        break;
    case CHECKED:
        m_currentColor = m_checkedColor;
        break;
    case CROSS:
        m_currentColor = m_crossColor;
        break;
    }
    repaint();
    QAbstractButton::leaveEvent(e);
}

void MatrixCell::mouseMoveEvent(QMouseEvent *e){
    /*if(rect().contains(e->pos())){
        if(e->buttons() & Qt::LeftButton){
            qDebug() << this->text() << "entered while clicked !";
        }else{
            qDebug() << this->text() << "No click";
        }
    }
    qDebug() << this->text() << "Direct";
    repaint();
    QAbstractButton::mousePressEvent(e);*/
}

void MatrixCell::mousePressEvent(QMouseEvent *e){
    switch(m_state){
    case NEUTRAL:
        m_currentColor = m_neutralClickedColor;
        break;
    case CHECKED:
        m_currentColor = m_checkedClickedColor;
        break;
    case CROSS:
        m_currentColor = m_crossClickedColor;
        break;
    }
    repaint();
    QAbstractButton::mousePressEvent(e);
}

void MatrixCell::mouseReleaseEvent(QMouseEvent *e){
    if(e->button() == Qt::LeftButton){
        if(m_state != CHECKED){
            m_state = CHECKED;
        }else{
            m_state = NEUTRAL;
        }
    }else if(e->button() == Qt::RightButton){
        if(m_state != CROSS){
            m_state = CROSS;
        }else{
            m_state = NEUTRAL;
        }
    }
    switch(m_state){
    case NEUTRAL:
        m_currentColor = m_neutralColor;
        break;
    case CHECKED:
        m_currentColor = m_checkedColor;
        break;
    case CROSS:
        m_currentColor = m_crossColor;
        break;
    }
    repaint();
    QAbstractButton::mouseReleaseEvent(e);
}

// Neutral colors ---------------------------------

QColor MatrixCell::neutralColor() const{
    return m_neutralColor;
}

void MatrixCell::setNeutralColor(QColor color){
    m_neutralColor = color;
}

QColor MatrixCell::neutralEnteredColor() const{
    return m_neutralEnteredColor;
}

void MatrixCell::setNeutralEnteredColor(QColor color){
    m_neutralEnteredColor = color;
}

QColor MatrixCell::neutralClickedColor() const{
    return m_neutralClickedColor;
}

void MatrixCell::setNeutralClickedColor(QColor color){
    m_neutralClickedColor = color;
}

// Checked colors -------------------

QColor MatrixCell::checkedColor() const{
    return m_checkedColor;
}

void MatrixCell::setCheckedColor(QColor color){
    m_checkedColor = color;
}

QColor MatrixCell::checkedEnteredColor() const{
    return m_checkedEnteredColor;
}

void MatrixCell::setCheckedEnteredColor(QColor color){
    m_checkedEnteredColor = color;
}

QColor MatrixCell::checkedClickedColor() const{
    return m_checkedClickedColor;
}

void MatrixCell::setCheckedClickedColor(QColor color){
    m_checkedClickedColor = color;
}

// Crossed colors -------------------

QColor MatrixCell::crossColor() const{
    return m_crossColor;
}

void MatrixCell::setCrossColor(QColor color){
    m_crossColor = color;
}

QColor MatrixCell::crossEnteredColor() const{
    return m_crossEnteredColor;
}

void MatrixCell::setCrossEnteredColor(QColor color){
    m_crossEnteredColor = color;
}

QColor MatrixCell::crossClickedColor() const{
    return m_crossClickedColor;
}

void MatrixCell::setCrossClickedColor(QColor color){
    m_crossClickedColor = color;
}
