#pragma once

#include "CellType.h"
#include "Container/Array.h"

class ICellPrism : public ICellType
{
public:
    ~ICellPrism() override = default;

    [[nodiscard]] ECellType GetCellType() const override;
    
    // 顶点数量：6
    [[nodiscard]] int32 GetVertexCount() const override;

    // 维度：3
    [[nodiscard]] int32 GetCellDimension() const override;

    // ============================================================================
    // 静态辅助函数
    // ============================================================================

    /**
     * 计算三棱柱体积
     * 
     * 算法：
     * 将三棱柱分割为三个四面体计算：(0,1,2,3), (1,2,3,4), (2,3,4,5)
     * 假设底面 (0,1,2)，顶面 (3,4,5)
     * 
     * 适用范围：
     * 任意三棱柱单元
     * 
     * @param Vertices 三棱柱的6个顶点
     * @return 体积
     */
    static float ComputeVolume(const TArray<FVector>& Vertices);
    static double ComputeVolume(const TArray<FVector3d>& Vertices);

    /**
     * 计算三棱柱中心（几何中心）
     * 
     * 算法：
     * 所有顶点的算术平均值：Sum(Vi) / 6
     * 
     * 适用范围：
     * 任意三棱柱单元
     * 
     * @param Vertices 三棱柱的6个顶点
     * @return 中心坐标
     */
    static FVector ComputeCenter(const TArray<FVector>& Vertices);
    static FVector3d ComputeCenter(const TArray<FVector3d>& Vertices);
};