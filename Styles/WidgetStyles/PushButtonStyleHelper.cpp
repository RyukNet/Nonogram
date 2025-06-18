#include "PushButtonStyleHelper.h"

#include "ColorRepository.h"

static const uint8_t radius = 4;

void PushButtonStyleHelper::setupPainterForShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget){
    Q_UNUSED(widget)
    if(!(option->state & QStyle::State_Enabled)){
        painter->setPen(ColorRepository::disabledOutlineBrush());
        painter->setBrush(ColorRepository::buttonDisabledBackground());
    }else if(option->state & QStyle::State_Sunken){
        painter->setPen(ColorRepository::pressedOutlineColor());
        painter->setBrush(ColorRepository::buttonPressedBackground());
    }else if((option->state & QStyle::State_MouseOver) || (option->state & QStyle::State_HasFocus)){
        painter->setPen(ColorRepository::hoverOutlineBrush());
        painter->setBrush(ColorRepository::buttonHoveredBackground());
    }else{
        painter->setPen(ColorRepository::buttonOutlineColor());
        painter->setBrush(ColorRepository::buttonBackground());
    }
}

void PushButtonStyleHelper::drawButtonShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget){
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    setupPainterForShape(option, painter, widget);
    painter->drawRoundedRect(QRectF(option->rect).adjusted(0.5, 0.5, -0.5, -0.5), radius, radius);
    painter->restore();
}

QSize PushButtonStyleHelper::sizeFromContents(const QStyleOptionButton *option, QSize contentsSize, const QWidget *widget) const{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    const int margin = 6; // usually this comes from PM_ButtonMargin
    const int frameWidth = 2; // due to pen width 1 in drawButtonBevel, on each side
    return QSize(qMax(60, contentsSize.width() + margin + frameWidth), contentsSize.height() + margin + frameWidth);
}

void PushButtonStyleHelper::adjustTextPalette(QStyleOptionButton *option) const{
    QColor textColor;
    if(!(option->state & QStyle::State_Enabled)){
        textColor = ColorRepository::disabledTextColor();
    }else if(option->state & QStyle::State_Sunken){
        textColor = ColorRepository::pressedTextColor();
    }else{
        textColor = ColorRepository::textColor();
    }
    option->palette.setColor(QPalette::ButtonText, textColor);
}
