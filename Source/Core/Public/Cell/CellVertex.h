#pragma once

#include "CellType.h"

/**
 * ICellVertex - 点单元类型实现
 */
class ICellVertex : public ICellType
{
public:
    ECellType GetCellType() const override { return ECellType::Vertex; }

    // 每种 Cell 的顶点数量
    int32 GetVertexCount() const override { return 1; }

    // 单元的维度
    int32 GetCellDimension() const override { return 1; }
};