#include "Theme.h"

#include <QToolTip>
#include <QApplication>

static bool is_dark = false;

QPalette Theme::palette(){
    QPalette pal;
    pal.setColor(QPalette::Window, windowBackground());
    pal.setColor(QPalette::Base, baseBackground());
    pal.setColor(QPalette::Text, text());
    pal.setColor(QPalette::WindowText, text());

    pal.setColor(QPalette::ButtonText, text());

    pal.setColor(QPalette::ToolTipText, text());

    QToolTip::setPalette(pal);

    return pal;
}

void Theme::setDarkMode(bool dark){
    is_dark = dark;
    qApp->setPalette(palette());
}

QColor Theme::windowBackground(){
    if(is_dark){
        return QColor(0x29, 0x29, 0x29);
    }else{
        return QColor(0xf3, 0xf3, 0xf3);
    }
}

QColor Theme::baseBackground(){
    if(is_dark){
        return QColor(0x3d, 0x3d, 0x3d);
    }else{
        return QColor(0xff, 0xff, 0xff);
    }
}

QColor Theme::text(){
    if(is_dark){
        return QColor(0xff, 0xff, 0xff);
    }else{
        return QColor(0x24, 0x24, 0x24);
    }
}

QColor Theme::pressedTextColor(){
    if(is_dark){
        return QColor(0xff, 0xff, 0xff);
    }else{
        return QColor(0x24, 0x24, 0x24);
    }
}

QColor Theme::hoverTextColor(){
    if(is_dark){
        return QColor(0xff, 0xff, 0xff);
    }else{
        return QColor(0x24, 0x24, 0x24);
    }
}

QColor Theme::buttonOutlineColor(){
    return Theme::text();
}

QColor Theme::pressedOutlineColor(){
    return Theme::text();
}

QBrush Theme::hoverOutlineBrush(const QRect &rect){
    const qreal w = rect.width();
    const qreal h = rect.height();
    const qreal xmid = w * 0.54;
    const qreal xoffset = (h * h) / (2 * w); // Proportionality: xoffset / (h/2) = h / w
    const qreal x0 = xmid - xoffset;
    const qreal x1 = xmid + xoffset;

    QLinearGradient gradient(x0, h, x1, 0);
    gradient.setColorAt(0.0, QColor(0x53, 0x94, 0x9f));
    gradient.setColorAt(1.0, QColor(0x75, 0x55, 0x79));
    return QBrush(gradient);
}

QBrush Theme::progressBarOutlineBrush(const QRect &rect){
    QLinearGradient gradient(0, rect.height(), rect.width(), 0);
    gradient.setColorAt(0.0, QColor(0x11, 0xc2, 0xe1));
    gradient.setColorAt(1.0, QColor(0x89, 0x3a, 0x94));
    return QBrush(gradient);
}
