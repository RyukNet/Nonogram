#include "ToggleSwitch.h"

#include <QMouseEvent>
#include <QPainter>

#include "ColorRepository.h"

static const int s_height = 26;
static const int s_innerMargin = 4;
static const int s_handleSize = s_height - s_innerMargin * 2;
static const int s_width = s_handleSize * 2 + s_innerMargin * 2;

ToggleSwitch::ToggleSwitch(QWidget *parent)
    : QWidget{parent}
{
    setSizePolicy({QSizePolicy::Fixed, QSizePolicy::Fixed}); // sizeHint is the only acceptable size
    setFocusPolicy(Qt::TabFocus); // can tab into the widget
    setAttribute(Qt::WA_Hover); // repaint on mouse-enter/mouse-exit
}

void ToggleSwitch::setChecked(bool checked)
{
    if (m_checked == checked)
        return;
    m_checked = checked;
    emit toggled(checked);
    update();
}

bool ToggleSwitch::isChecked() const
{
    return m_checked;
}

void ToggleSwitch::toggle()
{
    setChecked(!m_checked);
}

QSize ToggleSwitch::sizeHint() const
{
    return QSize(s_width, s_height);
}

void ToggleSwitch::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Another advantage of QStyle: custom widgets can share code
    // with it (here just the color repository, but we could share more code
    // if needed).
    // With QSS, we can't use QSS to style our custom widget.

    // Similar colors as for pushbuttons (but the shape is different)
    if (!isEnabled()) {
        painter.setPen(QPen(ColorRepository::disabledOutlineBrush(), 1.2));
        //painter.setOpacity(0.5);
    } /*else if (m_mouseDown) // Sunken
        //painter.setPen(ColorRepository::pressedOutlineColor());
    else if (underMouse() || hasFocus()){
        //painter.setPen(QPen(ColorRepository::hoverOutlineBrush(rect()), 1));
        painter.setPen(ColorRepository::buttonOutlineColor2());
    }*/
    else
        painter.setPen(QPen(ColorRepository::buttonOutlineColor2(), 1.2));

    if (m_checked)
        painter.setBrush(ColorRepository::windowBackground());
    const qreal radius = height() / 2;
    painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), radius, radius);

    // Now draw the handle

    QRect valueRect = rect().adjusted(s_innerMargin, s_innerMargin, -s_innerMargin, -s_innerMargin);
    valueRect.setWidth(valueRect.height()); // must be a square

    if (m_checked) {
        valueRect.moveLeft(width() - valueRect.width() - s_innerMargin);
        //painter.setPen(QPen(Theme::progressBarOutlineBrush(valueRect), 1));
        painter.setPen(ColorRepository::buttonOutlineColor2());
        painter.setBrush(Qt::NoBrush);
    } else {
        painter.setBrush(ColorRepository::windowBackground());
    }
    painter.drawEllipse(valueRect);
}

void ToggleSwitch::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_mouseDown = true;
    } else {
        event->ignore();
    }
}

void ToggleSwitch::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_mouseDown) {
        m_mouseDown = false;
        toggle();
        emit checked(m_checked);
    } else {
        event->ignore();
    }
}

void ToggleSwitch::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        toggle();
        emit checked(m_checked);
    } else {
        event->ignore(); // let it propagate to the parent (e.g. so that Return closes dialogs)
    }
}
