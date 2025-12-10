#pragma once

#include "CellType.h"
#include "Math/Math.h"
#include "Container/Array.h"

class ICellHex : public ICellType
{
public:
    ~ICellHex() override = default;

    [[nodiscard]] ECellType GetCellType() const override;
    
    // 顶点数量：8
    [[nodiscard]] int32 GetVertexCount() const override;

    // 维度：3
    [[nodiscard]] int32 GetCellDimension() const override;

    // ============================================================================
    // 静态辅助函数
    // ============================================================================

    /**
     * 计算六面体中心（几何中心）
     * 
     * 算法：
     * 所有顶点的算术平均值：Sum(Vi) / 8
     * 
     * 适用范围：
     * 任意六面体
     * 
     * @param Vertices 六面体的8个顶点
     * @return 中心坐标
     */
    static FVector ComputeCenter(const TArray<FVector>& Vertices);
    static FVector3d ComputeCenter(const TArray<FVector3d>& Vertices);
};