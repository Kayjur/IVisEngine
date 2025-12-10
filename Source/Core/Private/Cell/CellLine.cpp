#include "Cell/CellLine.h"

ECellType ICellLine::GetCellType() const
{
    return ECellType::Line;
}

int32 ICellLine::GetVertexCount() const
{
    return 2;
}

int32 ICellLine::GetCellDimension() const
{
    return 1;
}

// ============================================================================
// 静态辅助函数
// ============================================================================

float ICellLine::ComputeLength(const FVector& V0, const FVector& V1)
{
    return (V1 - V0).Size();
}

double ICellLine::ComputeLength(const FVector3d& V0, const FVector3d& V1)
{
    return (V1 - V0).Size();
}

float ICellLine::ComputeLengthSquared(const FVector& V0, const FVector& V1)
{
    return (V1 - V0).SizeSquared();
}

double ICellLine::ComputeLengthSquared(const FVector3d& V0, const FVector3d& V1)
{
    return (V1 - V0).SizeSquared();
}

FVector ICellLine::ComputeCenter(const FVector& V0, const FVector& V1)
{
    return (V0 + V1) * 0.5f;
}

FVector3d ICellLine::ComputeCenter(const FVector3d& V0, const FVector3d& V1)
{
    return (V0 + V1) * 0.5;
}

FVector ICellLine::ComputeDirection(const FVector& V0, const FVector& V1)
{
    return (V1 - V0).GetSafeNormal();
}

FVector3d ICellLine::ComputeDirection(const FVector3d& V0, const FVector3d& V1)
{
    return (V1 - V0).GetSafeNormal();
}
