#pragma once

#include <QColor>
#include <QBrush>

namespace ColorRepository
{
QColor windowBackground();
QColor baseBackground();

QPalette palette();
void setDarkMode(bool check);

QColor textColor();
QColor pressedTextColor();
QColor disabledTextColor();

QColor buttonOutlineColor();
QColor pressedOutlineColor();
QColor hoverOutlineBrush();
QColor disabledOutlineBrush();

QColor buttonOutlineColor2();

QColor buttonBackground();
QColor buttonPressedBackground();
QColor buttonHoveredBackground();
QColor buttonDisabledBackground();


}
