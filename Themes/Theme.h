#pragma once

#include <QPalette>
#include <QColor>

namespace Theme
{
QPalette palette();

void setDarkMode(bool dark);

QColor windowBackground();
QColor baseBackground();
QColor text();

QColor pressedTextColor();
QColor hoverTextColor();

QColor buttonOutlineColor();
QColor pressedOutlineColor();
QBrush hoverOutlineBrush(const QRect &rect);

 QBrush progressBarOutlineBrush(const QRect &rect);

}
