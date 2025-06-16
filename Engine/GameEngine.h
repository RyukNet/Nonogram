#pragma once

#include <vector>

#include <QtGlobal>
#include <QObject>

class GameEngine : public QObject
{
    Q_OBJECT
public:
    explicit GameEngine(quint8 columns, quint8 rows, QObject *parent = nullptr);

    quint8 columns() const;
    quint8 rows() const;

    std::vector<std::vector<quint8>> columnsTasks() const;
    std::vector<std::vector<quint8>> rowsTasks() const;

    quint32 validateGrid(std::vector<std::vector<bool>> matrix);
public slots:
    void setSize(quint8 columns, quint8 rows);
signals:
    void resized();
    void columnsChanged(quint8 c);
    void rowsChanged(quint8 r);
    void valid();


protected:
    quint8 m_columns;
    quint8 m_rows;
    std::vector<std::vector<bool>> m_matrix;
};
