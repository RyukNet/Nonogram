#pragma once

#include <vector>

#include <QtGlobal>
#include <QObject>

class GameEngine : public QObject
{
    Q_OBJECT
public:
    explicit GameEngine(quint8 width, quint8 height, QObject *parent = nullptr);

    quint8 width() const;
    quint8 height() const;

public slots:
    void setSize(quint8 width, quint8 height);
signals:

protected:
    quint8 m_width;
    quint8 m_height;
    std::vector<std::vector<bool>> m_matrix;
};
