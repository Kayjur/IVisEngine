#include "../../Public/Container/CellArray.h"
#include <algorithm>

// ============================================================================
// 构造函数和析构函数
// ============================================================================

FCellArray::FCellArray()
    : ReservedCapacity(0)
    , bEnableMemoryReuse(true)
{
    // 预分配初始容量
    Reserve(1024);
}

FCellArray::FCellArray(uint32 InitialCapacity)
    : ReservedCapacity(0)
    , bEnableMemoryReuse(true)
{
    Reserve(InitialCapacity);
}

FCellArray::FCellArray(const FCellArray& Other)
    : VertexIndices(Other.VertexIndices)
    , CellOffsets(Other.CellOffsets)
    , CellTypes(Other.CellTypes)
    , ReservedCapacity(Other.ReservedCapacity)
    , bEnableMemoryReuse(Other.bEnableMemoryReuse)
{}

FCellArray::FCellArray(FCellArray&& Other) noexcept
    : VertexIndices(std::move(Other.VertexIndices))
    , CellOffsets(std::move(Other.CellOffsets))
    , CellTypes(std::move(Other.CellTypes))
    , ReservedCapacity(Other.ReservedCapacity)
    , bEnableMemoryReuse(Other.bEnableMemoryReuse)
{
    Other.ReservedCapacity = 0;
}

FCellArray::~FCellArray() = default;

FCellArray& FCellArray::operator=(const FCellArray& Other)
{
    if (this != &Other)
    {
        VertexIndices = Other.VertexIndices;
        CellOffsets = Other.CellOffsets;
        CellTypes = Other.CellTypes;
        ReservedCapacity = Other.ReservedCapacity;
        bEnableMemoryReuse = Other.bEnableMemoryReuse;
    }
    return *this;
}

FCellArray& FCellArray::operator=(FCellArray&& Other) noexcept
{
    if (this != &Other)
    {
        VertexIndices = std::move(Other.VertexIndices);
        CellOffsets = std::move(Other.CellOffsets);
        CellTypes = std::move(Other.CellTypes);
        ReservedCapacity = Other.ReservedCapacity;
        bEnableMemoryReuse = Other.bEnableMemoryReuse;
        Other.ReservedCapacity = 0;
    }
    return *this;
}

// ============================================================================
// 容量和大小
// ============================================================================

uint32 FCellArray::GetCellCount() const
{
    return static_cast<uint32>(CellTypes.Num());
}

uint32 FCellArray::GetVertexIndexCount() const
{
    return static_cast<uint32>(VertexIndices.Num());
}

bool FCellArray::IsEmpty() const
{
    return CellTypes.IsEmpty();
}

void FCellArray::Reserve(uint32 Capacity)
{
    // 估算每个单元平均4个顶点
    VertexIndices.Reserve(Capacity * 4);
    CellOffsets.Reserve(Capacity + 1);  // +1 for end offset
    CellTypes.Reserve(Capacity);
    ReservedCapacity = Capacity;
}

void FCellArray::Shrink()
{
    VertexIndices.Shrink();
    CellOffsets.Shrink();
    CellTypes.Shrink();
}

// ============================================================================
// 添加单元
// ============================================================================

void FCellArray::AddCell(ECellType CellType, const TArray<VertexIndexType>& InVertexIndices)
{
    if (InVertexIndices.IsEmpty())
    {
        return;
    }
    
    // 记录当前单元的起始位置
    CellOffsets.Add(static_cast<uint32>(VertexIndices.Num()));
    
    // 添加顶点索引
    for (uint32 i = 0; i < InVertexIndices.Num(); ++i)
    {
        VertexIndices.Add(InVertexIndices[i]);
    }
    
    // 记录单元类型
    CellTypes.Add(CellType);
}

void FCellArray::AddCell(ECellType CellType, const VertexIndexType* InVertexIndices, uint32 VertexCount)
{
    if (VertexCount == 0 || InVertexIndices == nullptr)
    {
        return;
    }
    
    // 记录当前单元的起始位置
    CellOffsets.Add(static_cast<uint32>(VertexIndices.Num()));

    for (uint32 i = 0; i < VertexCount; ++i)
    {
        VertexIndices.Add(InVertexIndices[i]);
    }

    // 记录单元类型
    CellTypes.Add(CellType);
}

void FCellArray::AddCells(const TArray<FCellInfo>& Cells)
{
    Reserve(GetCellCount() + Cells.Num());
    
    for (const auto& Cell : Cells)
    {
        AddCell(Cell.CellType, Cell.VertexIndices);
    }
}

// ============================================================================
// 获取单元
// ============================================================================

bool FCellArray::GetCell(CellIndexType CellIndex, FCellInfo& OutCellInfo) const
{
    if (!IsValidCellIndex(CellIndex))
    {
        return false;
    }
    
    OutCellInfo.CellType = CellTypes[CellIndex];
    
    // 获取该单元的顶点索引范围
    uint32 StartOffset = CellOffsets[CellIndex];
    uint32 EndOffset = (CellIndex + 1 < CellOffsets.Num()) 
        ? CellOffsets[CellIndex + 1] 
        : static_cast<uint32>(VertexIndices.Num());
    
    // 提取顶点索引
    OutCellInfo.VertexIndices.Reset();
    OutCellInfo.VertexIndices.Reserve(EndOffset - StartOffset);
    for (uint32 i = StartOffset; i < EndOffset; ++i)
    {
        OutCellInfo.VertexIndices.Add(VertexIndices[i]);
    }
    
    return true;
}

ECellType FCellArray::GetCellType(CellIndexType CellIndex) const
{
    if (!IsValidCellIndex(CellIndex))
    {
        return ECellType::None;
    }
    return CellTypes[CellIndex];
}

bool FCellArray::GetCellVertexIndices(CellIndexType CellIndex, TArray<VertexIndexType>& OutVertexIndices) const
{
    if (!IsValidCellIndex(CellIndex))
    {
        return false;
    }
    
    // 获取该单元的顶点索引范围
    uint32 StartOffset = CellOffsets[CellIndex];
    uint32 EndOffset = (CellIndex + 1 < CellOffsets.Num()) 
        ? CellOffsets[CellIndex + 1] 
        : static_cast<uint32>(VertexIndices.Num());
    
    // 提取顶点索引
    OutVertexIndices.Reset();
    OutVertexIndices.Reserve(EndOffset - StartOffset);
    for (uint32 i = StartOffset; i < EndOffset; ++i)
    {
        OutVertexIndices.Add(VertexIndices[i]);
    }
    
    return true;
}

uint32 FCellArray::GetCellVertexCount(CellIndexType CellIndex) const
{
    if (!IsValidCellIndex(CellIndex))
    {
        return 0;
    }
    
    uint32 StartOffset = CellOffsets[CellIndex];
    uint32 EndOffset = (CellIndex + 1 < CellOffsets.Num()) 
        ? CellOffsets[CellIndex + 1] 
        : static_cast<uint32>(VertexIndices.Num());
    
    return EndOffset - StartOffset;
}

const FCellArray::VertexIndexType* FCellArray::GetCellVertexIndicesPtr(CellIndexType CellIndex, uint32& OutVertexCount) const
{
    if (!IsValidCellIndex(CellIndex))
    {
        OutVertexCount = 0;
        return nullptr;
    }
    
    uint32 StartOffset = CellOffsets[CellIndex];
    uint32 EndOffset = (CellIndex + 1 < CellOffsets.Num()) 
        ? CellOffsets[CellIndex + 1] 
        : static_cast<uint32>(VertexIndices.Num());
    
    OutVertexCount = EndOffset - StartOffset;
    return VertexIndices.GetData() + StartOffset;
}

// ============================================================================
// 删除单元
// ============================================================================

bool FCellArray::RemoveCell(CellIndexType CellIndex)
{
    if (!IsValidCellIndex(CellIndex))
    {
        return false;
    }
    
    // 获取要删除的单元的顶点数量
    uint32 StartOffset = CellOffsets[CellIndex];
    uint32 EndOffset = (CellIndex + 1 < CellOffsets.Num()) 
        ? CellOffsets[CellIndex + 1] 
        : static_cast<uint32>(VertexIndices.Num());
    uint32 VertexCount = EndOffset - StartOffset;
    
    // 删除顶点索引
    VertexIndices.RemoveAt(StartOffset, VertexCount);
    
    // 删除单元偏移和类型
    CellOffsets.RemoveAt(CellIndex);
    CellTypes.RemoveAt(CellIndex);
    
    // 更新后续单元的偏移量
    uint32 OffsetDelta = static_cast<uint32>(VertexCount);
    for (uint32 i = CellIndex; i < CellOffsets.Num(); ++i)
    {
        CellOffsets[i] -= OffsetDelta;
    }
    
    return true;
}

uint32 FCellArray::RemoveCells(CellIndexType StartIndex, uint32 Count)
{
    if (!IsValidCellIndex(StartIndex))
    {
        return 0;
    }
    
    uint32 EndIndex = std::min(StartIndex + Count, GetCellCount());
    uint32 ActualCount = EndIndex - StartIndex;
    
    if (ActualCount == 0)
    {
        return 0;
    }
    
    // 计算要删除的顶点索引总数
    uint32 StartOffset = CellOffsets[StartIndex];
    uint32 EndOffset = (EndIndex < CellOffsets.Num()) 
        ? CellOffsets[EndIndex] 
        : static_cast<uint32>(VertexIndices.Num());
    uint32 TotalVertexCount = EndOffset - StartOffset;
    
    // 删除顶点索引
    VertexIndices.RemoveAt(StartOffset, TotalVertexCount);
    
    // 删除单元偏移和类型
    CellOffsets.RemoveAt(StartIndex, ActualCount);
    CellTypes.RemoveAt(StartIndex, ActualCount);
    
    // 更新后续单元的偏移量
    for (uint32 i = StartIndex; i < CellOffsets.Num(); ++i)
    {
        CellOffsets[i] -= TotalVertexCount;
    }
    
    return ActualCount;
}

uint32 FCellArray::RemoveCellsByType(ECellType CellType)
{
    if (IsEmpty())
    {
        return 0;
    }
    
    // 收集要删除的单元索引（从后往前，避免索引变化）
    TArray<CellIndexType> IndicesToRemove;
    for (int32 i = static_cast<int32>(GetCellCount()) - 1; i >= 0; --i)
    {
        if (CellTypes[i] == CellType)
        {
            IndicesToRemove.Add(static_cast<CellIndexType>(i));
        }
    }
    
    // 删除单元
    uint32 RemovedCount = 0;
    for (CellIndexType Index : IndicesToRemove)
    {
        if (RemoveCell(Index))
        {
            ++RemovedCount;
        }
    }
    
    return RemovedCount;
}

void FCellArray::Clear()
{
    VertexIndices.Reset();
    CellOffsets.Reset();
    CellTypes.Reset();
}

void FCellArray::Reset()
{
    Clear();
}

void FCellArray::Empty()
{
    Clear();
}

// ============================================================================
// 查询和统计
// ============================================================================

bool FCellArray::IsValidCellIndex(CellIndexType CellIndex) const
{
    return CellIndex < GetCellCount();
}

uint32 FCellArray::GetCellCountByType(ECellType CellType) const
{
    uint32 Count = 0;
    for (const auto& Type : CellTypes)
    {
        if (Type == CellType)
        {
            ++Count;
        }
    }
    return Count;
}

void FCellArray::GetCellTypes(TArray<ECellType>& OutTypes) const
{
    OutTypes.Reset();
    for (const auto& Type : CellTypes)
    {
        if (!OutTypes.Contains(Type))
        {
            OutTypes.Add(Type);
        }
    }
}

uint32 FCellArray::FindCellsContainingVertex(VertexIndexType VertexIndex, TArray<CellIndexType>& OutCellIndices) const
{
    OutCellIndices.Reset();
    
    for (CellIndexType i = 0; i < GetCellCount(); ++i)
    {
        uint32 VertexCount = 0;
        const VertexIndexType* Indices = GetCellVertexIndicesPtr(i, VertexCount);
        
        if (Indices != nullptr)
        {
            for (uint32 j = 0; j < VertexCount; ++j)
            {
                if (Indices[j] == VertexIndex)
                {
                    OutCellIndices.Add(i);
                    break;
                }
            }
        }
    }
    
    return static_cast<uint32>(OutCellIndices.Num());
}

// ============================================================================
// 内存管理
// ============================================================================

void FCellArray::SetMemoryReuseEnabled(bool bEnable)
{
    bEnableMemoryReuse = bEnable;
}

bool FCellArray::IsMemoryReuseEnabled() const
{
    return bEnableMemoryReuse;
}

void FCellArray::Compact()
{
    Shrink();
}

uint64 FCellArray::GetMemoryUsage() const
{
    uint64 Usage = 0;
    Usage += VertexIndices.Capacity() * sizeof(VertexIndexType);
    Usage += CellOffsets.Capacity() * sizeof(uint32);
    Usage += CellTypes.Capacity() * sizeof(ECellType);
    return Usage;
}
