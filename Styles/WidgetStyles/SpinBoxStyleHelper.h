#pragma once

#include <QStyle>

#include <QPainter>
#include <QRectf>
#include <QPushButton>
#include <QStyleOptionSpinBox>
#include <QWidget>

class SpinBoxStyleHelper
{
public:
    //void setupPainterForShape(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget);
    void drawSpinBox(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget);

private:
    void setupPainterForFrameBox(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget);
    void setupPainterForEditFieldBox(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget);
    void setupPainterForBoxUp(const QStyleOptionSpinBox *option, QPainter *painter, const QRectF subRect, const QWidget *widget);
    void setupPainterForBoxDown(const QStyleOptionSpinBox *option, QPainter *painter, const QRectF subRect, const QWidget *widget);

    void drawFrameBox(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget);
    void drawEditFieldBox(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget);
    void drawFrameUp(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget);
    void drawFrameDown(const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget);
};
