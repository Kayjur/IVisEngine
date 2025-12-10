#include "Cell/CellPyramid.h"
#include "Cell/CellTetra.h"

ECellType ICellPyramid::GetCellType() const
{
    return ECellType::Pyramid;
}

int32 ICellPyramid::GetVertexCount() const
{
    return 5;
}

int32 ICellPyramid::GetCellDimension() const
{
    return 3;
}

// ============================================================================
// 静态辅助函数
// ============================================================================

float ICellPyramid::ComputeVolume(const TArray<FVector>& Vertices)
{
    if (Vertices.Num() < 5) return 0.0f;

    // 分割为两个四面体: (0,1,2,4) 和 (0,2,3,4)
    // 假设底面是 0-1-2-3，顶点是 4
    return ICellTetra::ComputeVolume(Vertices[0], Vertices[1], Vertices[2], Vertices[4]) +
           ICellTetra::ComputeVolume(Vertices[0], Vertices[2], Vertices[3], Vertices[4]);
}

double ICellPyramid::ComputeVolume(const TArray<FVector3d>& Vertices)
{
    if (Vertices.Num() < 5) return 0.0;

    return ICellTetra::ComputeVolume(Vertices[0], Vertices[1], Vertices[2], Vertices[4]) +
           ICellTetra::ComputeVolume(Vertices[0], Vertices[2], Vertices[3], Vertices[4]);
}

FVector ICellPyramid::ComputeCenter(const TArray<FVector>& Vertices)
{
    int32 Count = Vertices.Num();
    if (Count == 0) return FVector::ZeroVector();

    FVector Sum = FVector::ZeroVector();
    for (const auto& V : Vertices)
    {
        Sum += V;
    }
    return Sum / static_cast<float>(Count);
}

FVector3d ICellPyramid::ComputeCenter(const TArray<FVector3d>& Vertices)
{
    int32 Count = Vertices.Num();
    if (Count == 0) return FVector3d::ZeroVector();

    FVector3d Sum = FVector3d::ZeroVector();
    for (const auto& V : Vertices)
    {
        Sum += V;
    }
    return Sum / static_cast<double>(Count);
}