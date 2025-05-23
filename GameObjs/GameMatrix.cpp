#include "GameMatrix.h"

#include "MatrixGenerator.h"

GameMatrix::GameMatrix(quint8 width, quint8 height, QWidget *parent)
    : QWidget{parent}, m_matrix(height, std::vector<bool>(width, false))
{
    m_gridLay = new QGridLayout(this);
    this->setLayout(m_gridLay);

    for(quint8 w = 0; w < width; w++){
        for(quint8 h = 0; h < height; h++){
            m_matrix[h][w] = MatrixGenerator::GenerateNext();

        }
    }
}
