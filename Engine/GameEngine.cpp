#include "GameEngine.h"

#include <iostream>

#include "MatrixGenerator.h"

GameEngine::GameEngine(quint8 width, quint8 height, QObject *parent)
    : QObject{parent}
    ,m_columns(width)
    ,m_rows(height)
//,m_matrix(m_rows, std::vector<bool>(m_columns, false))
{
    MatrixGenerator::GenerateNext(m_columns, m_rows, m_matrix);

}

void GameEngine::setSize(quint8 columns, quint8 rows){
    m_columns = columns;
    m_rows = rows;
    MatrixGenerator::GenerateNext(m_columns, m_rows, m_matrix);
    emit columnsChanged(m_columns);
    emit rowsChanged(m_rows);
    emit resized();
}

quint8 GameEngine::columns() const{
    return m_columns;
}
quint8 GameEngine::rows() const{
    return m_rows;
}

std::vector<std::vector<quint8>> GameEngine::rowsTasks() const{
    std::vector<std::vector<quint8>> result(0);
    for(quint8 h = 0; h < m_rows; h++){
        quint8 value = 0;
        std::vector<quint8> values;
        for(quint8 w = 0; w < m_columns; w++){
            if(m_matrix[w][h]){
                value++;
            }else{
                if(value > 0){
                    values.push_back(value);
                    value = 0;
                }
            }
        }
        if(value > 0){
            values.push_back(value);
            value = 0;
        }
        if(values.empty()){
            values.push_back(0);
        }
        result.push_back(values);
    }
    return result;
}

std::vector<std::vector<quint8>> GameEngine::columnsTasks() const{
    std::vector<std::vector<quint8>> result(0);
    for(quint8 w = 0; w < m_columns; w++){
        quint8 value = 0;
        std::vector<quint8> values;
        for(quint8 h = 0; h < m_rows; h++){
            if(m_matrix[w][h]){
                value++;
            }else{
                if(value > 0){
                    values.push_back(value);
                    value = 0;
                }
            }
        }
        if(value > 0){
            values.push_back(value);
            value = 0;
        }
        if(values.empty()){
            values.push_back(0);
        }
        result.push_back(values);
    }
    return result;
}
