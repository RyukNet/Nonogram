#include "AppStyle.h"

#include <QStyleOptionButton>
#include <QStyleOptionSpinBox>
#include <QPalette>
#include <QPainter>
#include <QLineF>
#include <QPointF>
#include <QSpinBox>

#include "ColorRepository.h"

AppStyle::AppStyle()
    : QProxyStyle()
    , m_pushButtonStyleHelper(new PushButtonStyleHelper())
    , m_spinBoxStyleHelper(new SpinBoxStyleHelper())
{

}

void AppStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const{
    switch(element){
    case CE_PushButton:
        QCommonStyle::drawControl(element, option, painter, widget);
        return;
    case CE_PushButtonBevel:
        if(const auto* optionButton = qstyleoption_cast<const QStyleOptionButton *>(option)){
            m_pushButtonStyleHelper->drawButtonShape(optionButton, painter, widget);
        }
        return;
    case CE_PushButtonLabel:
        if (const auto *optionButton = qstyleoption_cast<const QStyleOptionButton *>(option)) {
            QStyleOptionButton copy = *optionButton;
            m_pushButtonStyleHelper->adjustTextPalette(&copy);
            QCommonStyle::drawControl(element, &copy, painter, widget);
        }
        return;
    default:
        QProxyStyle::drawControl(element, option, painter, widget);
        break;
    }
}

void AppStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const{
    switch(control){
    case CC_SpinBox:
    {
        const QStyleOptionSpinBox* spinButton = qstyleoption_cast<const QStyleOptionSpinBox*>(option);
        if(!spinButton){
            return;
        }
        m_spinBoxStyleHelper->drawSpinBox(spinButton, painter, widget);
        proxy()->drawPrimitive(QStyle::PE_IndicatorSpinUp, option, painter, widget);
        proxy()->drawPrimitive(QStyle::PE_IndicatorSpinDown, option, painter, widget);
    }
    break;
    default:
        QProxyStyle::drawComplexControl(control, option, painter, widget);
        break;
    }
}
#include <QDebug>
void AppStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    if(element == PE_IndicatorSpinUp){
        const QStyleOptionSpinBox* spinBoxOption = qstyleoption_cast<const QStyleOptionSpinBox*>(option);
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        QRectF buttonRect = proxy()->subControlRect(QStyle::CC_SpinBox, spinBoxOption, QStyle::SC_SpinBoxUp, widget);
        buttonRect.adjust(1, 1, -1, -1);
        qreal minDimension = qMin(buttonRect.width(), buttonRect.height());
        qreal midX = buttonRect.x() + (buttonRect.width() / 2);
        qreal midY = buttonRect.y() + (buttonRect.height() / 2);

        QPointF topPoint(midX, midY - minDimension / 4);
        QPointF leftPoint(midX - minDimension / 2, midY + minDimension / 4);
        QPointF rightPoint(midX + minDimension / 2, midY + minDimension / 4);
        QLineF slash(leftPoint, topPoint);
        QLineF antiSlash(topPoint, rightPoint);

        QVector<QLineF> lines({slash, antiSlash});


        if(!(spinBoxOption->state & QStyle::State_Enabled) || !(spinBoxOption->stepEnabled & QSpinBox::StepUpEnabled)){
            painter->setPen(QPen(ColorRepository::disabledTextColor(), 1.2));
        }else{
            painter->setPen(QPen(ColorRepository::textColor(), 1.2));
        }
        //painter->setBrush(ColorRepository::textColor());
        painter->drawLines(lines);

        painter->restore();
    }else if(element == PE_IndicatorSpinDown){
        const QStyleOptionSpinBox* spinBoxOption = qstyleoption_cast<const QStyleOptionSpinBox*>(option);
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        QRectF buttonRect = proxy()->subControlRect(QStyle::CC_SpinBox, spinBoxOption, QStyle::SC_SpinBoxDown, widget);
        buttonRect.adjust(1, 1, -1, -1);
        qreal minDimension = qMin(buttonRect.width(), buttonRect.height());
        qreal midX = buttonRect.x() + (buttonRect.width() / 2);
        qreal midY = buttonRect.y() + (buttonRect.height() / 2);

        QPointF bottomPoint(midX, midY + minDimension / 4);
        QPointF leftPoint(midX - minDimension / 2, midY - minDimension / 4);
        QPointF rightPoint(midX + minDimension / 2, midY - minDimension / 4);
        QLineF slash(leftPoint, bottomPoint);
        QLineF antiSlash(bottomPoint, rightPoint);

        QVector<QLineF> lines({slash, antiSlash});


        if(!(spinBoxOption->state & QStyle::State_Enabled) || !(spinBoxOption->stepEnabled & QSpinBox::StepDownEnabled)){
            painter->setPen(QPen(ColorRepository::disabledTextColor(), 1.2));
        }else{
            painter->setPen(QPen(ColorRepository::textColor(), 1.2));
        }
        //painter->setBrush(ColorRepository::textColor());
        painter->drawLines(lines);

        painter->restore();
    }else{
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}
