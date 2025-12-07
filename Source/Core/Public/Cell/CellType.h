#pragma once

#include <string>
#include "HAL/Platform.h"


/**
 * 单元类型枚举
 */
enum class ECellType : uint8
{
    None = 0,
    Line,           // 线单元
    PolyLine,       // 折线单元
    Triangle,       // 三角形单元
    Quad,           // 四边形单元
    Polygon,        // 多边形单元
    Tetra,          // 四面体单元
    Hex,            // 六面体单元
    Pyramid,        // 金字塔单元
    Prism,          // 三棱柱单元
    Polyhedron,     // 多面体单元
};

class ICellType
{
public:
    virtual ~ICellType() = default;

    [[nodiscard]] virtual ECellType GetCellType() const = 0;

    // 每种 Cell 的顶点数量
    [[nodiscard]] virtual int32 GetVertexCount() const = 0;

    // 单元的维度
    [[nodiscard]] virtual int32 GetCellDimension() const = 0;

    // ============================================================================
    // 静态辅助函数
    // ============================================================================

    /**
     * 获取单元类型的标准顶点数量
     * @param CellType 单元类型
     * @return 标准顶点数量，如果类型未知返回 0
     */
    static uint32 GetStandardVertexCount(ECellType CellType);

    /**
     * 检查单元类型是否有效
     * @param CellType 单元类型
     * @return 是否有效
     */
    static bool IsValidCellType(ECellType CellType);

    /**
     * 获取单元类型的维度
     * @param CellType 单元类型
     * @return 维度（0=点，1=线，2=面，3=体），如果类型未知返回 -1
     */
    static int32 GetCellDimension(ECellType CellType);
};

// ============================================================================
// 全局辅助函数（可选）
// ============================================================================

/**
 * 获取单元类型的标准顶点数量（全局函数版本）
 */
inline uint32 GetCellTypeVertexCount(ECellType CellType)
{
    return ICellType::GetStandardVertexCount(CellType);
}

/**
 * 获取单元类型的维度（全局函数版本）
 */
inline int32 GetCellTypeDimension(ECellType CellType)
{
    return ICellType::GetCellDimension(CellType);
}

inline std::string EnumToString(ECellType Type)
{
    switch (Type)
    {
        case ECellType::None: return "None";
        case ECellType::Line: return "Line";
        case ECellType::PolyLine: return "PolyLine";
        case ECellType::Triangle: return "Triangle";
        case ECellType::Quad: return "Quad";
        case ECellType::Polygon: return "Polygon";
        case ECellType::Tetra: return "Tetra";
        case ECellType::Hex: return "Hex";
        case ECellType::Pyramid: return "Pyramid";
        case ECellType::Prism: return "Prism";
        case ECellType::Polyhedron: return "Polyhedron";
        default: return "Unknown";
    }
}