#pragma once

#include <QtGlobal>
#include <vector>

namespace MatrixGenerator
{
void GenerateNext(quint8 columns,quint8 rows, std::vector<std::vector<bool>>& matrix);
}
