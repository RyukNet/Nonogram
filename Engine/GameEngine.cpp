#include "GameEngine.h"

#include <iostream>

#include "MatrixGenerator.h"


void printGrid(const std::vector<std::vector<bool>>& grid){
    for(int i = 0; i < grid.size(); i++){
        for(int j = 0; j < grid[i].size(); j++){
            std::cout << ((grid[i][j]) ? "X" : " ") << "|";
        }
        std::cout << std::endl;
    }
}

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

quint32 GameEngine::validateGrid(std::vector<std::vector<bool>> matrix){
    assert(matrix.size() == m_matrix.size());
    assert(matrix[0].size() == m_matrix[0].size());

    printGrid(m_matrix);
    std::cout << "_________________________________________" << std::endl;
    printGrid(matrix);
    std::cout << "_________________________________________" << std::endl;

    quint32 errors = 0;
    for(quint8 x = 0; x < m_matrix.size(); x++){
        for(quint8 y = 0; y < m_matrix[x].size(); y++){
            if(m_matrix[x][y] != matrix[x][y]){
                errors++;
            }
        }
    }
    if(errors == 0){
        emit valid();
    }
    return errors;
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
            if(m_matrix[h][w]){
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
            if(m_matrix[h][w]){
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
