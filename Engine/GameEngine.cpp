#include "GameEngine.h"

#include <iostream>

#include "MatrixGenerator.h"

GameEngine::GameEngine(quint8 width, quint8 height, QObject *parent)
    : QObject{parent}
    ,m_columns(width)
    ,m_rows(height)
    ,m_matrix(m_rows, std::vector<bool>(m_columns, false))
{
    for(quint8 h = 0; h < m_rows; h++){
        for(quint8 w = 0; w < m_columns; w++){
            m_matrix[h][w] = MatrixGenerator::GenerateNext();
        }
    }
    /*for(quint8 h = 0; h < height; h++){
        for(quint8 w = 0; w < width; w++){
            std::cout << ((m_matrix[h][w])? "X" : " ") << "|";
        }
        std::cout << std::endl;
    }*/
    /*std::vector<std::vector<quint8>> widthNums = this->columnsTasks();
    for(int w = 0; w < widthNums.size(); w++){
        for(int i = 0; i < widthNums[w].size(); i++){
            std::cout << (int)widthNums[w][i] << " | ";
        }
        std::cout << std::endl;
    }*/
    /*std::cout << "---------------" << std::endl;
    std::vector<std::vector<quint8>> heightNums = this->rowsTasks();
    for(int h = 0; h < heightNums.size(); h++){
        for(int i = 0; i < heightNums[h].size(); i++){
            std::cout << (int)heightNums[h][i] << " | ";
        }
        std::cout << std::endl;
    }*/
}

void GameEngine::setSize(quint8 columns, quint8 rows){
    m_columns = columns;
    m_rows = rows;
    emit columnsChanged(m_columns);
    emit rowsChanged(m_rows);
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
