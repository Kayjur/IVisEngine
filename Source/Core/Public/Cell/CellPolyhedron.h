#pragma once

#include "CellType.h"
#include "Math/Math.h"
#include "Container/Array.h"

class ICellPolyhedron : public ICellType
{
public:
    ~ICellPolyhedron() override = default;

    [[nodiscard]] ECellType GetCellType() const override;
    
    // 顶点数量：-1 (动态)
    [[nodiscard]] int32 GetVertexCount() const override;

    // 维度：3
    [[nodiscard]] int32 GetCellDimension() const override;

    // ============================================================================
    // 静态辅助函数
    // ============================================================================

    /**
     * 计算多面体中心（几何中心）
     * 
     * 算法：
     * 所有顶点的算术平均值：Sum(Vi) / N
     * 
     * 注意事项：
     * 1. 对于多面体，通常需要面信息来计算精确的体积或质心。
     * 2. 这里仅计算几何中心（顶点平均值）。
     * 
     * @param Vertices 多面体的顶点列表
     * @return 中心坐标
     */
    static FVector ComputeCenter(const TArray<FVector>& Vertices);
    static FVector3d ComputeCenter(const TArray<FVector3d>& Vertices);
};