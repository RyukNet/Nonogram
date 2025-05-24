#include "GameEngine.h"

#include "MatrixGenerator.h"

GameEngine::GameEngine(quint8 width, quint8 height, QObject *parent)
    : QObject{parent}
    ,m_width(width)
    ,m_height(height)
    ,m_matrix(m_height, std::vector<bool>(m_width, false))
{
    for(quint8 h = 0; h < m_height; h++){
        for(quint8 w = 0; w < m_width; w++){
            m_matrix[h][w] = MatrixGenerator::GenerateNext();
        }
    }
    /*for(quint8 h = 0; h < height; h++){
        for(quint8 w = 0; w < width; w++){
            std::cout << ((m_matrix[h][w])? "X" : " ") << "|";
        }
        std::cout << std::endl;
    }*/
}

void GameEngine::setSize(quint8 width, quint8 height){
    m_width = width;
    m_height = height;
}

quint8 GameEngine::width() const{
    return m_width;
}
quint8 GameEngine::height() const{
    return m_height;
}
