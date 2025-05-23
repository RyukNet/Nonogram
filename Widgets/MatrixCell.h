#pragma once

#include <QAbstractButton>
#include <QColor>
#include <QEvent>
#include <QPaintEvent>

class MatrixCell : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(QColor neutralColor READ neutralColor WRITE setNeutralColor DESIGNABLE true)
    Q_PROPERTY(QColor neutralEnteredColor READ neutralEnteredColor WRITE setNeutralEnteredColor DESIGNABLE true)
    Q_PROPERTY(QColor neutralClickedColor READ neutralClickedColor WRITE setNeutralClickedColor DESIGNABLE true)

    Q_PROPERTY(QColor checkedColor READ checkedColor WRITE setCheckedColor DESIGNABLE true)
    Q_PROPERTY(QColor checkedEnteredColor READ checkedEnteredColor WRITE setCheckedEnteredColor DESIGNABLE true)
    Q_PROPERTY(QColor checkedClickedColor READ checkedClickedColor WRITE setCheckedClickedColor DESIGNABLE true)

    Q_PROPERTY(QColor crossColor READ crossColor WRITE setCrossColor DESIGNABLE true)
    Q_PROPERTY(QColor crossEnteredColor READ crossEnteredColor WRITE setCrossEnteredColor DESIGNABLE true)
    Q_PROPERTY(QColor crossClickedColor READ crossClickedColor WRITE setCrossClickedColor DESIGNABLE true)
public:
    enum State{
        NEUTRAL,
        CHECKED,
        CROSS,
    };
    MatrixCell(QWidget *parent = nullptr);

    bool event(QEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


    // Neutral colors -------------------

    QColor neutralColor() const;
    void setNeutralColor(QColor color);

    QColor neutralEnteredColor() const;
    void setNeutralEnteredColor(QColor color);

    QColor neutralClickedColor() const;
    void setNeutralClickedColor(QColor color);

    // Checked colors -------------------

    QColor checkedColor() const;
    void setCheckedColor(QColor color);

    QColor checkedEnteredColor() const;
    void setCheckedEnteredColor(QColor color);

    QColor checkedClickedColor() const;
    void setCheckedClickedColor(QColor color);

    // Crossed colors -------------------

    QColor crossColor() const;
    void setCrossColor(QColor color);

    QColor crossEnteredColor() const;
    void setCrossEnteredColor(QColor color);

    QColor crossClickedColor() const;
    void setCrossClickedColor(QColor color);

protected:
    QColor m_neutralColor;
    QColor m_neutralEnteredColor;
    QColor m_neutralClickedColor;

    QColor m_checkedColor;
    QColor m_checkedEnteredColor;
    QColor m_checkedClickedColor;

    QColor m_crossColor;
    QColor m_crossEnteredColor;
    QColor m_crossClickedColor;


    State m_state;
    QColor m_currentColor;
};
