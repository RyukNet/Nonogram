#include "ColorRepository.h"

#include <QApplication>
#include <QPalette>
#include <QToolTip>

static bool is_dark = false;

QPalette ColorRepository::palette(){
    QPalette pal;
    pal.setColor(QPalette::Window, windowBackground());
    pal.setColor(QPalette::Base, baseBackground());
    pal.setColor(QPalette::WindowText, textColor());
    pal.setColor(QPalette::Text, textColor());

    pal.setColor(QPalette::ButtonText, textColor());

    pal.setColor(QPalette::ToolTipText, textColor());

    QToolTip::setPalette(pal);

    return pal;
}

void ColorRepository::setDarkMode(bool check){
    is_dark = check;
    qApp->setPalette(palette());
}

QColor ColorRepository::windowBackground(){
    if(is_dark){
        return QColor(0x20, 0x20, 0x20);
    }else{
        return QColor(0xf3, 0xf3, 0xf3);
    }
}

QColor ColorRepository::baseBackground(){
    if(is_dark){
        return QColor(0x3d, 0x3d, 0x3d);
    }else{
        return QColor(0xff, 0xff, 0xff);
    }
}


QColor ColorRepository::textColor(){
    if(is_dark){
        return QColor(0xFF, 0xFF, 0xFF);
    }else{
        return QColor(0x1B, 0x1B, 0x1B);
    }
}

QColor ColorRepository::pressedTextColor(){
    if(is_dark){
        return QColor(0xCE, 0xCE, 0xCE);
    }else{
        return QColor(0x5D, 0x5D, 0x5D);
    }
}


QColor ColorRepository::disabledTextColor(){
    if(is_dark){
        return QColor(0x78, 0x78, 0x78);
    }else{
        return QColor(0x9D, 0x9D, 0x9D);
    }
}

QColor ColorRepository::buttonOutlineColor(){
    if(is_dark){
        return QColor(0x30, 0x30, 0x30);
    }else{
        return QColor(0xE5, 0xE5, 0xE5);
    }
}

QColor ColorRepository::buttonOutlineColor2(){
    if(is_dark){
        return QColor(0xCC, 0xCC, 0xCC);
    }else{
        return QColor(0x5A, 0x5A, 0x5A);
    }
}

QColor ColorRepository::pressedOutlineColor(){
    return buttonOutlineColor();
}

QColor ColorRepository::hoverOutlineBrush(){
    return buttonOutlineColor();
}

QColor ColorRepository::disabledOutlineBrush(){
    if(is_dark){
        return QColor(0x30, 0x30, 0x30);
    }else{
        return QColor(0xE5, 0xE5, 0xE5);
    }
}

QColor ColorRepository::buttonBackground(){
    if(is_dark){
        return QColor(0x2D, 0x2D, 0x2D);
    }else{
        return QColor(0xFB, 0xFB, 0xFB);
    }
}

QColor ColorRepository::buttonPressedBackground(){
    if(is_dark){
        return QColor(0x27, 0x27, 0x27);
    }else{
        return QColor(0xF5, 0xF5, 0xF5);
    }
}


QColor ColorRepository::buttonHoveredBackground(){
    if(is_dark){
        return QColor(0x32, 0x32, 0x32);
    }else{
        return QColor(0xF6, 0xF6, 0xF6);
    }
}

QColor ColorRepository::buttonDisabledBackground(){
    if(is_dark){
        return QColor(0x2A, 0x2A, 0x2A);
    }else{
        return QColor(0xF5, 0xF5, 0xF5);
    }
}
