#pragma once

#include "CellType.h"
#include "Container/Array.h"

class ICellPyramid : public ICellType
{
public:
    ~ICellPyramid() override = default;

    [[nodiscard]] ECellType GetCellType() const override;
    
    // 顶点数量：5
    [[nodiscard]] int32 GetVertexCount() const override;

    // 维度：3
    [[nodiscard]] int32 GetCellDimension() const override;

    // ============================================================================
    // 静态辅助函数
    // ============================================================================

    /**
     * 计算金字塔体积
     * 
     * 算法：
     * 将金字塔分割为两个四面体计算：(0,1,2,4) 和 (0,2,3,4)
     * 假设底面 (0,1,2,3) 为四边形，顶点 4 为金字塔顶点
     * 
     * 适用范围：
     * 底面近似平面的金字塔单元
     * 
     * @param Vertices 金字塔的5个顶点
     * @return 体积
     */
    static float ComputeVolume(const TArray<FVector>& Vertices);
    static double ComputeVolume(const TArray<FVector3d>& Vertices);

    /**
     * 计算金字塔中心（几何中心）
     * 
     * 算法：
     * 所有顶点的算术平均值：Sum(Vi) / 5
     * 
     * 适用范围：
     * 任意金字塔单元
     * 
     * @param Vertices 金字塔的5个顶点
     * @return 中心坐标
     */
    static FVector ComputeCenter(const TArray<FVector>& Vertices);
    static FVector3d ComputeCenter(const TArray<FVector3d>& Vertices);
};