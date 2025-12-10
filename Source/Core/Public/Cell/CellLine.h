#pragma once

#include "CellType.h"
#include "Math/Math.h"

class ICellLine : public ICellType
{
public:
    ~ICellLine() override = default;

    [[nodiscard]] ECellType GetCellType() const override;

    // 每种 Cell 的顶点数量
    [[nodiscard]] int32 GetVertexCount() const override;

    // 单元的维度
    [[nodiscard]] int32 GetCellDimension() const override;

    // ============================================================================
    // 静态辅助函数
    // ============================================================================

    /**
     * 计算线段长度
     * 
     * 算法：
     * 欧几里得距离：|V1 - V0|
     * 
     * @param V0, V1 线段端点
     * @return 长度
     */
    static float ComputeLength(const FVector& V0, const FVector& V1);
    static double ComputeLength(const FVector3d& V0, const FVector3d& V1);

    /**
     * 计算线段长度的平方（避免开方）
     * 
     * 算法：
     * 欧几里得距离平方：(V1 - V0) . (V1 - V0)
     * 
     * 适用范围：
     * 当只需要比较长度大小时使用，比 ComputeLength 更高效
     * 
     * @param V0, V1 线段端点
     * @return 长度平方
     */
    static float ComputeLengthSquared(const FVector& V0, const FVector& V1);
    static double ComputeLengthSquared(const FVector3d& V0, const FVector3d& V1);

    /**
     * 计算线段中心（中点）
     * 
     * 算法：
     * (V0 + V1) / 2
     * 
     * @param V0, V1 线段端点
     * @return 中心坐标
     */
    static FVector ComputeCenter(const FVector& V0, const FVector& V1);
    static FVector3d ComputeCenter(const FVector3d& V0, const FVector3d& V1);

    /**
     * 计算线段方向（归一化）
     * 
     * 算法：
     * (V1 - V0) / |V1 - V0|
     * 
     * 注意事项：
     * 如果 V0 和 V1 重合（长度为0），结果未定义（通常为零向量）
     * 
     * @param V0 起点
     * @param V1 终点
     * @return 归一化的方向向量
     */
    static FVector ComputeDirection(const FVector& V0, const FVector& V1);
    static FVector3d ComputeDirection(const FVector3d& V0, const FVector3d& V1);
};