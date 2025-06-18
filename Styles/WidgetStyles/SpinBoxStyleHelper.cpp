#include "SpinBoxStyleHelper.h"

#include <QApplication>
#include <QPainter>
#include <QSpinBox>
#include <QDebug>

#include "ColorRepository.h"

static const uint8_t radius = 4;
static const uint8_t sub_radius = 2;

void SpinBoxStyleHelper::drawSpinBox(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget){
    /*painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    setupPainterForShape(option, painter, widget);
    painter->drawRoundedRect(QRectF(option->rect).adjusted(0.5, 0.5, -0.5, -0.5), radius, radius);
    painter->restore();*/
    drawFrameBox(option, painter, widget);
    drawEditFieldBox(option, painter, widget);
    drawFrameUp(option, painter, widget);
    drawFrameDown(option, painter, widget);
}

void SpinBoxStyleHelper::setupPainterForFrameBox(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget){
    Q_UNUSED(widget)
    painter->setPen(ColorRepository::buttonOutlineColor());
    if(!(option->state & QStyle::State_Enabled)){
        painter->setBrush(ColorRepository::buttonDisabledBackground());
    }else{
        painter->setBrush(ColorRepository::baseBackground());
    }
}

void SpinBoxStyleHelper::setupPainterForEditFieldBox(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget){
    Q_UNUSED(widget)
    painter->setBrush(ColorRepository::buttonBackground());
    if(!(option->state & QStyle::State_Enabled)){
        painter->setBrush(ColorRepository::buttonDisabledBackground());
    }
}

void SpinBoxStyleHelper::setupPainterForBoxUp(const QStyleOptionSpinBox *option, QPainter *painter, QRectF subRect, const QWidget *widget){
    Q_UNUSED(widget)
    painter->setPen(QColor(0, 0, 0, 0));
    if(!(option->state & QStyle::State_Enabled)){
        painter->setBrush(ColorRepository::buttonDisabledBackground());
    }else{
        if(option->stepEnabled & QSpinBox::StepUpEnabled){
            painter->setBrush(ColorRepository::baseBackground());
            if(option->state & QStyle::State_Sunken){
                //TODO: Calculate mouse position before set color
                /*QPointF absoluteWidgetTopLeft = widget->mapToGlobal(QPoint(0, 0));
                QRectF absoluteSubControlRect(absoluteWidgetTopLeft.x() + widget->rect().width() - subRect.width(),
                                              absoluteWidgetTopLeft.y() + widget->rect().height() - subRect.height(),
                                              subRect.width(),
                                              subRect.height());
                subRect.setTopLeft(absoluteWidgetTopLeft);
                auto cursorPos = QCursor::pos();
                if(absoluteSubControlRect.contains(QCursor::pos())){
                    painter->setBrush(ColorRepository::buttonPressedBackground());
                }else{
                    painter->setBrush(ColorRepository::baseBackground());
                }*/
                QPointF absoluteWidgetTopLeft = widget->mapToGlobal(QPoint(0, 0));
                subRect.setTopLeft(absoluteWidgetTopLeft);
                if(subRect.contains(QCursor::pos())){
                    painter->setBrush(ColorRepository::buttonPressedBackground());
                }else{
                    painter->setBrush(ColorRepository::baseBackground());
                }
            }
        }else{
            painter->setBrush(ColorRepository::buttonDisabledBackground());
        }
    }
}

void SpinBoxStyleHelper::setupPainterForBoxDown(const QStyleOptionSpinBox *option, QPainter *painter, QRectF subRect, const QWidget *widget){
    Q_UNUSED(widget)
    painter->setPen(QColor(0, 0, 0, 0));
    if(!(option->state & QStyle::State_Enabled)){
        painter->setBrush(ColorRepository::buttonDisabledBackground());
    }else{
        if(option->stepEnabled & QSpinBox::StepDownEnabled){
            painter->setBrush(ColorRepository::baseBackground());
            if(option->state & QStyle::State_Sunken){
                //TODO: Calculate mouse position before set color
                QPointF absoluteWidgetTopLeft = widget->mapToGlobal(QPoint(0, 0));
                subRect.setTopLeft(absoluteWidgetTopLeft);
                if(subRect.contains(QCursor::pos())){
                    painter->setBrush(ColorRepository::buttonPressedBackground());
                }else{
                    painter->setBrush(ColorRepository::baseBackground());
                }
            }
        }else{
            painter->setBrush(ColorRepository::buttonDisabledBackground());
        }
    }
}

void SpinBoxStyleHelper::drawFrameBox(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget){
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    setupPainterForFrameBox(option, painter, widget);
    //QRect rect = qApp->style()->proxy()->subControlRect(QStyle::CC_SpinBox, option, QStyle::SC_SpinBoxFrame, widget);
    //qDebug() << "Frame Box : " << rect.width();
    painter->drawRoundedRect(QRectF(option->rect).adjusted(0.5, 0.5, -0.5, -0.5), radius, radius);
    painter->restore();
}

void SpinBoxStyleHelper::drawEditFieldBox(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget){
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    setupPainterForEditFieldBox(option, painter, widget);
    QRectF rect = qApp->style()->proxy()->subControlRect(QStyle::CC_SpinBox, option, QStyle::SC_SpinBoxEditField, widget);
    //qDebug() << "Field Box : " << rect.width();
    painter->drawRoundedRect(rect.adjusted(0.5, 0.5, -0.5, -0.5), sub_radius, sub_radius);
    painter->restore();
}

#include <QApplication>

void SpinBoxStyleHelper::drawFrameUp(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget){
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    QRectF rect = qApp->style()->proxy()->subControlRect(QStyle::CC_SpinBox, option, QStyle::SC_SpinBoxUp, widget);
    //qDebug() << "Frame Upx : " << rect.width();
    setupPainterForBoxUp(option, painter, rect, widget);
    //qApp->style()->drawPrimitive(QStyle::PE_IndicatorSpinUp, option, painter, nullptr);
    painter->drawRoundedRect(rect.adjusted(0.5, 0.5, -0.5, -0.5), sub_radius, sub_radius);
    painter->restore();
}

void SpinBoxStyleHelper::drawFrameDown(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget){
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    QRectF rect = qApp->style()->proxy()->subControlRect(QStyle::CC_SpinBox, option, QStyle::SC_SpinBoxDown, widget);
    //qDebug() << "Frame Dwn : " << rect.width();
    setupPainterForBoxDown(option, painter, rect, widget);
    //painter->drawRoundedRect(QRectF(option->rect).adjusted(0.5, 0.5, -0.5, -0.5), radius, radius);
    //painter->drawRoundedRect(rect.adjusted(0.5, 0.5, -0.5, -0.5), sub_radius, sub_radius);
    painter->drawRoundedRect(rect.adjusted(0.5, 0.5, -0.5, -0.5), sub_radius, sub_radius);
    painter->restore();
}
