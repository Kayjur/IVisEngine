#pragma once

#include "CellType.h"
#include "CellTriangle.h"
#include "Math/Math.h"
#include "Container/Array.h"

class ICellPolygon : public ICellType
{
public:
    ~ICellPolygon() override = default;

    [[nodiscard]] ECellType GetCellType() const override;

    // 每种 Cell 的顶点数量
    // 对于 Polygon，此方法返回 -1，表示动态顶点数
    [[nodiscard]] int32 GetVertexCount() const override;

    // 单元的维度
    [[nodiscard]] int32 GetCellDimension() const override;

    // ============================================================================
    // 静态辅助函数
    // ============================================================================

    /**
     * 计算多边形的平均法线
     * 适用于一般多边形（包括非凸、非平面）
     * 计算多边形中心，然后计算中心与每条边构成的三角形法线之和（Newell方法的变体）
     * @param Vertices 多边形顶点列表
     * @return 归一化的法线向量
     */
    static FVector ComputeNormal(const TArray<FVector>& Vertices);
    static FVector3d ComputeNormal(const TArray<FVector3d>& Vertices);

    /**
     * 计算多边形面积
     * 将多边形分割为三角形扇计算
     * 注意：仅适用于平面或近似平面的多边形。对于非平面多边形，这是投影面积的近似。
     * @param Vertices 多边形顶点列表
     * @return 面积
     */
    static float ComputeArea(const TArray<FVector>& Vertices);
    static double ComputeArea(const TArray<FVector3d>& Vertices);

    /**
     * 计算多边形中心（几何中心）
     * 即所有顶点的平均值
     * @param Vertices 多边形顶点列表
     * @return 中心坐标
     */
    static FVector ComputeCenter(const TArray<FVector>& Vertices);
    static FVector3d ComputeCenter(const TArray<FVector3d>& Vertices);

    /**
     * 检查多边形是否为凸多边形
     * 通过检查所有相邻边的叉积方向是否一致来判断
     * 假设多边形是平面的
     * @param Vertices 多边形顶点列表
     * @param Normal 多边形法线（可选，如果为空则自动计算）
     * @return 是否为凸
     */
    static bool IsConvex(const TArray<FVector>& Vertices, const FVector& Normal = FVector::ZeroVector());
    static bool IsConvex(const TArray<FVector3d>& Vertices, const FVector3d& Normal = FVector3d::ZeroVector());
};
