#include "AppStyle.h"

#include <QStyleOptionButton>
#include <QStyleOptionSpinBox>
#include <QPalette>
#include <QPainter>
#include <QLineF>
#include <QPointF>

#include "ColorRepository.h"

AppStyle::AppStyle()
    : QProxyStyle()
    , m_pushButtonStyleHelper(new PushButtonStyleHelper())
    , m_spinBoxStyleHelper(new SpinBoxStyleHelper())
{

}

/*int AppStyle::styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const{
    if(hint == SH_UnderlineShortcut){

    }
}*/

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

        QPointF topPoint(midX, midY);// - minDimension / 2);
        QPointF leftPoint(midX - minDimension / 2, midY + minDimension / 2);
        QPointF rightPoint(midX + minDimension / 2, midY + minDimension / 2);
        QLineF slash(leftPoint, topPoint);
        QLineF antiSlash(topPoint, rightPoint);

        QVector<QLineF> lines({slash, antiSlash});


        if(spinBoxOption->state & QStyle::State_Enabled){
            painter->setPen(QPen(ColorRepository::textColor(), 1.2));
        }else{
            painter->setPen(QPen(ColorRepository::disabledTextColor(), 1.2));
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

        QPointF bottomPoint(midX, midY);
        QPointF leftPoint(midX - minDimension / 2, midY - minDimension / 2);
        QPointF rightPoint(midX + minDimension / 2, midY - minDimension / 2);
        QLineF slash(leftPoint, bottomPoint);
        QLineF antiSlash(bottomPoint, rightPoint);

        QVector<QLineF> lines({slash, antiSlash});


        if(spinBoxOption->state & QStyle::State_Enabled){
            painter->setPen(QPen(ColorRepository::textColor(), 1.2));
        }else{
            painter->setPen(QPen(ColorRepository::disabledTextColor(), 1.2));
        }
        //painter->setBrush(ColorRepository::textColor());
        painter->drawLines(lines);

        painter->restore();
    }else{
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
    /*if(element == PE_IndicatorSpinUp || element == PE_IndicatorSpinDown){
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        //painter->setPen(ColorRepository::baseBackground());
        //painter->setBrush(ColorRepository::baseBackground());
        //proxy()->subControlRect(CC_SpinBox, option, SC_SpinBoxUp, widget);
        //painter->setPen(Qt::red);
        //painter->setBrush(Qt::red);
        //painter->drawRect(option->rect);
        qDebug() << "drawPrimitive : " << option->rect.width();
        int rectWidth = option->rect.width();
        int rectHeight = option->rect.height();
        int minDimension = qMin(rectWidth, rectHeight);
        minDimension = minDimension;

        int rectXMid = option->rect.x() + (option->rect.width() / 2);
        int rectYMid = option->rect.y() + (option->rect.height() / 2);

        QLineF slash;
        QLineF antiSlash;
        if(element == PE_IndicatorSpinUp){
            QPointF topPoint(rectXMid, rectYMid - 1);
            QPoint leftPoint(rectXMid - minDimension + 2, rectYMid + minDimension - 2);
            QPoint rightPoint(rectXMid + minDimension - 2, rectYMid + minDimension - 2);

            slash.setP1(topPoint);
            slash.setP2(leftPoint);
            antiSlash.setP1(topPoint);
            antiSlash.setP2(rightPoint);
        }else{
            QPointF bottomPoint(rectXMid, rectYMid + 1);
            QPoint leftPoint(rectXMid - minDimension + 2, rectYMid - minDimension + 2);
            QPoint rightPoint(rectXMid + minDimension - 2, rectYMid - minDimension + 2);

            slash.setP1(bottomPoint);
            slash.setP2(leftPoint);
            antiSlash.setP1(bottomPoint);
            antiSlash.setP2(rightPoint);
        }

        if(option->state & State_Enabled){
            QPen pen(ColorRepository::textColor(), 1.2);
            painter->setPen(pen);
        }else if(option->state & State_Sunken){
            QPen pen(ColorRepository::pressedTextColor(), 1.2);
            painter->setPen(pen);
        }else{
            QPen pen(ColorRepository::disabledTextColor(), 1.2);
            painter->setPen(pen);
        }
        QVector<QLineF> lines({antiSlash, slash});
        painter->drawLines(lines);
        painter->restore();
    }else{
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }*/
}
