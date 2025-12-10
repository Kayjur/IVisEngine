#pragma once

#include "CellType.h"

class ICellQuad : public ICellType
{
public:
    ~ICellQuad() override = default;

    [[nodiscard]] ECellType GetCellType() const override;

    // 每种 Cell 的顶点数量
    [[nodiscard]] int32 GetVertexCount() const override;

    // 单元的维度
    [[nodiscard]] int32 GetCellDimension() const override;

    /** 四边形的边索引定义 {{0, 1}, {1, 2}, {2, 3}, {3, 0}} */
    static constexpr int32 Edges[4][2] = { { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 } };

    // ============================================================================
    // 静态辅助函数
    // ============================================================================

    /**
     * 计算四边形的平均法线
     * 将四边形分割为四个三角形（中心点与每条边），计算这四个三角形法线的平均值。
     * 这种方法比简单的叉积（只用三个点）更健壮，特别是对于非平面（翘曲）四边形。
     * @param V0, V1, V2, V3 四个顶点
     * @return 归一化的法线向量
     */
    static FVector ComputeNormal(const FVector& V0, const FVector& V1, const FVector& V2, const FVector& V3);
    static FVector3d ComputeNormal(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2, const FVector3d& V3);

    /**
     * 计算四边形面积
     * 将四边形分割为两个三角形 (V0,V1,V2) 和 (V0,V2,V3) 计算总面积。
     * 注意：对于非凸或自相交四边形，这种简单的分割可能会导致错误的结果。
     * @param V0, V1, V2, V3 四个顶点
     * @return 面积
     */
    static float ComputeArea(const FVector& V0, const FVector& V1, const FVector& V2, const FVector& V3);
    static double ComputeArea(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2, const FVector3d& V3);

    /**
     * 计算四边形中心（几何中心）
     * 即四个顶点的平均值
     * @param V0, V1, V2, V3 四个顶点
     * @return 中心坐标
     */
    static FVector ComputeCenter(const FVector& V0, const FVector& V1, const FVector& V2, const FVector& V3);
    static FVector3d ComputeCenter(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2, const FVector3d& V3);

    /**
     * 检查四边形是否为凸多边形
     * 通过检查所有相邻边的叉积方向是否一致来判断。
     * 假设四边形是平面或近似平面的。
     * @param V0, V1, V2, V3 四个顶点
     * @return 是否为凸
     */
    static bool IsConvex(const FVector& V0, const FVector& V1, const FVector& V2, const FVector& V3);
    static bool IsConvex(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2, const FVector3d& V3);
};
