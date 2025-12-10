#pragma once

#include "CellType.h"
#include "Math/Math.h"

class ICellTetra : public ICellType
{
public:
    ~ICellTetra() override = default;

    [[nodiscard]] ECellType GetCellType() const override;
    
    // 顶点数量：4
    [[nodiscard]] int32 GetVertexCount() const override;

    // 维度：3
    [[nodiscard]] int32 GetCellDimension() const override;

    // ============================================================================
    // 静态辅助函数
    // ============================================================================

    /**
     * 计算四面体体积
     * 
     * 算法：
     * 使用标量三重积公式：Volume = |(V1-V0) . ((V2-V0) x (V3-V0))| / 6
     * 
     * 适用范围：
     * 任意四面体
     * 
     * @param V0, V1, V2, V3 四面体的四个顶点
     * @return 体积（始终为正值）
     */
    static float ComputeVolume(const FVector& V0, const FVector& V1, const FVector& V2, const FVector& V3);
    static double ComputeVolume(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2, const FVector3d& V3);

    /**
     * 计算四面体中心（几何中心）
     * 
     * 算法：
     * 所有顶点的算术平均值：(V0 + V1 + V2 + V3) / 4
     * 
     * 适用范围：
     * 任意四面体
     * 
     * @param V0, V1, V2, V3 四面体的四个顶点
     * @return 中心坐标
     */
    static FVector ComputeCenter(const FVector& V0, const FVector& V1, const FVector& V2, const FVector& V3);
    static FVector3d ComputeCenter(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2, const FVector3d& V3);
};