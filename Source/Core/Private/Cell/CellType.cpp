#include "../../Public/Cell/CellType.h"


// ============================================================================
// 静态辅助函数
// ============================================================================

uint32 ICellType::GetStandardVertexCount(ECellType CellType)
{
    switch (CellType)
    {
        case ECellType::None:
            return 0;
        case ECellType::Vertex:
            return 1;
        case ECellType::Line:
            return 2;
        case ECellType::PolyLine:
            return 0; // 可变顶点数量
        case ECellType::Triangle:
            return 3;
        case ECellType::Quad:
            return 4;
        case ECellType::Polygon:
            return 0; // 可变顶点数量
        case ECellType::Tetra:
            return 4;
        case ECellType::Hex:
            return 8;
        case ECellType::Pyramid:
            return 5;
        case ECellType::Prism:
            return 6;
        case ECellType::Polyhedron:
            return 0;
        default:
            return 0;
    }
}

bool ICellType::IsValidCellType(ECellType CellType)
{
    return CellType != ECellType::None;
}

int32 ICellType::GetCellDimension(ECellType CellType)
{
    switch (CellType)
    {
        case ECellType::None:
        case ECellType::Vertex:
            return 0;
        case ECellType::Line:
        case ECellType::PolyLine:
            return 1;
        case ECellType::Triangle:
        case ECellType::Quad:
        case ECellType::Polygon:
            return 2;
        case ECellType::Tetra:
        case ECellType::Hex:
        case ECellType::Pyramid:
        case ECellType::Prism:
        case ECellType::Polyhedron:
            return 3;
        default:
            return 0;
    }
}