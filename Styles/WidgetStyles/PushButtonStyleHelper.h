#pragma once

#include <QSize>

#include <QPainter>
#include <QPushButton>
#include <QStyleOptionButton>
#include <QWidget>

class PushButtonStyleHelper
{
public:
    void setupPainterForShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget);
    void drawButtonShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget);
    QSize sizeFromContents(const QStyleOptionButton *option, QSize contentsSize, const QWidget *widget) const;
    void adjustTextPalette(QStyleOptionButton *option) const;
};
