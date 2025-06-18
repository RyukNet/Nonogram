#pragma once

#include <QProxyStyle>

#include "PushButtonStyleHelper.h"
#include "SpinBoxStyleHelper.h"

class AppStyle : public QProxyStyle
{
public:
    //AppStyle();
    AppStyle();
    //int styleHint(StyleHint hint, const QStyleOption *option = nullptr, const QWidget *widget = nullptr, QStyleHintReturn *returnData = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    void drawPrimitive(PrimitiveElement  control, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = nullptr) const override;
protected:
    bool is_dark = false;

    PushButtonStyleHelper* m_pushButtonStyleHelper = nullptr;
    SpinBoxStyleHelper* m_spinBoxStyleHelper = nullptr;
};
