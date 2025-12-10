#pragma once

#include "CellType.h"

class ICellTriangle : public ICellType
{
public:
    ~ICellTriangle() override = default;

    [[nodiscard]] ECellType GetCellType() const override;

    // 每种 Cell 的顶点数量
    [[nodiscard]] int32 GetVertexCount() const override;

    // 单元的维度
    [[nodiscard]] int32 GetCellDimension() const override;

    /** 三角形的边索引定义 {{0, 1}, {1, 2}, {2, 0}} */
    static constexpr int32 Edges[3][2] = { { 0, 1 }, { 1, 2 }, { 2, 0 } };

    // ============================================================================
    // 静态辅助函数
    // ============================================================================

    /**
     * 计算三角形法线
     * @param V0 第一个顶点
     * @param V1 第二个顶点
     * @param V2 第三个顶点
     * @return 归一化的法线向量
     */
    static FVector ComputeNormal(const FVector& V0, const FVector& V1, const FVector& V2);
    static FVector3d ComputeNormal(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2);

    /**
     * 计算三角形面积
     * @param V0 第一个顶点
     * @param V1 第二个顶点
     * @param V2 第三个顶点
     * @return 面积
     */
    static float ComputeArea(const FVector& V0, const FVector& V1, const FVector& V2);
    static double ComputeArea(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2);

    /**
     * 计算三角形重心
     * @param V0 第一个顶点
     * @param V1 第二个顶点
     * @param V2 第三个顶点
     * @return 重心坐标
     */
    static FVector ComputeCenter(const FVector& V0, const FVector& V1, const FVector& V2);
    static FVector3d ComputeCenter(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2);

    /**
     * 检查点是否在三角形内
     * @param Point 待检查的点
     * @param V0 第一个顶点
     * @param V1 第二个顶点
     * @param V2 第三个顶点
     * @return 是否在三角形内
     */
    static bool IsPointInTriangle(const FVector& Point, const FVector& V0, const FVector& V1, const FVector& V2);
    static bool IsPointInTriangle(const FVector3d& Point, const FVector3d& V0, const FVector3d& V1, const FVector3d& V2);

    /**
     * 计算点的重心坐标 (Barycentric Coordinates)
     * P = u*V0 + v*V1 + w*V2
     * @param Point 目标点
     * @param V0, V1, V2 三角形顶点
     * @param OutU, OutV, OutW 输出的重心坐标
     */
    static void ComputeBarycentricCoordinates(const FVector& Point, const FVector& V0, const FVector& V1, const FVector& V2, float& OutU, float& OutV, float& OutW);
    static void ComputeBarycentricCoordinates(const FVector3d& Point, const FVector3d& V0, const FVector3d& V1, const FVector3d& V2, double& OutU, double& OutV, double& OutW);

    /**
     * 计算点在三角形平面上的投影
     * @param Point 目标点
     * @param V0, V1, V2 三角形顶点
     * @return 投影点
     */
    static FVector ProjectPointOnPlane(const FVector& Point, const FVector& V0, const FVector& V1, const FVector& V2);
    static FVector3d ProjectPointOnPlane(const FVector3d& Point, const FVector3d& V0, const FVector3d& V1, const FVector3d& V2);

    /**
     * 计算点到三角形的最近点
     * @param Point 目标点
     * @param V0, V1, V2 三角形顶点
     * @return 三角形上最近的点
     */
    static FVector ClosestPointOnTriangle(const FVector& Point, const FVector& V0, const FVector& V1, const FVector& V2);
    static FVector3d ClosestPointOnTriangle(const FVector3d& Point, const FVector3d& V0, const FVector3d& V1, const FVector3d& V2);

    /**
     * 检查三角形是否退化 (面积接近0)
     * @param V0, V1, V2 三角形顶点
     * @return 是否退化
     */
    static bool IsDegenerate(const FVector& V0, const FVector& V1, const FVector& V2);
    static bool IsDegenerate(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2);
};