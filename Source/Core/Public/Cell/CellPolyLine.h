#pragma once

#include "CellType.h"
#include "Container/Array.h"

class ICellPolyLine : public ICellType
{
public:
    ~ICellPolyLine() override = default;

    [[nodiscard]] ECellType GetCellType() const override;

    // 每种 Cell 的顶点数量
    // 对于 PolyLine，此方法返回 -1，表示动态顶点数
    [[nodiscard]] int32 GetVertexCount() const override;

    // 单元的维度
    [[nodiscard]] int32 GetCellDimension() const override;

    // ============================================================================
    // 静态辅助函数
    // ============================================================================

    /**
     * 计算折线总长度
     * 
     * 算法：
     * 累加每一段线段的欧几里得长度：Sum(|Vi+1 - Vi|)
     * 
     * 适用范围：
     * 任意折线
     * 
     * @param Vertices 折线顶点列表
     * @return 总长度
     */
    static float ComputeLength(const TArray<FVector>& Vertices);
    static double ComputeLength(const TArray<FVector3d>& Vertices);

    /**
     * 计算折线中心（几何中心）
     * 
     * 算法：
     * 所有顶点的算术平均值：Sum(Vi) / N
     * 
     * 注意事项：
     * 这与折线的"质心"不同（质心通常基于线段长度加权）。
     * 这里仅仅是顶点的平均位置。
     * 
     * @param Vertices 折线顶点列表
     * @return 中心坐标
     */
    static FVector ComputeCenter(const TArray<FVector>& Vertices);
    static FVector3d ComputeCenter(const TArray<FVector3d>& Vertices);
};