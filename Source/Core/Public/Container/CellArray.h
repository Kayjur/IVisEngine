#pragma once

#include "Cell/CellType.h"
#include "Container/Array.h"
#include "HAL/Platform.h"

class FCellArray;

/** 单元信息结构体 */
struct FCellInfo
{
    /** 顶点索引类型 */
    using VertexIndexType = int32;

    /** 单元索引类型 */
    using CellIndexType = int32;

    using SizeType = uint64;

    /** 单元类型 */
    ECellType CellType;

    /** 顶点索引列表 */
    TArray<VertexIndexType> VertexIndices;

    FCellInfo() : CellType(ECellType::None) {}
    FCellInfo(const ECellType InType, const TArray<VertexIndexType>& InIndices)
        : CellType(InType), VertexIndices(InIndices) {}
    FCellInfo(const ECellType InType, TArray<VertexIndexType>&& InIndices)
        : CellType(InType), VertexIndices(std::move(InIndices)) {}

    /** 获取指定索引的元素（不检查边界） */
    VertexIndexType& operator[](const SizeType Index) { return VertexIndices[Index]; }
    const VertexIndexType& operator[](const SizeType Index) const { return VertexIndices[Index]; }

    [[nodiscard]] SizeType Num() const { return VertexIndices.Num(); }
};

/**
 * FCellArray - 单元数组类
 * 
 * 设计特点：
 * 1. 连续存储：所有单元的顶点索引连续存储在内存中，提高访问效率
 * 2. 类型化单元：支持多种单元类型
 * 3. 高效内存管理：使用压缩存储、批量处理、动态内存分配和重用
 * 4. 拓扑与几何分离：只存储拓扑信息（顶点索引），几何数据（顶点坐标）单独存储
 * 
 * 存储结构：
 * - VertexIndices: 连续存储所有单元的顶点索引
 * - CellOffsets: 记录每个单元在VertexIndices中的起始位置
 * - CellTypes: 记录每个单元的类型
 * 
 * 示例：
 *   三角形单元1: [0, 1, 2]
 *   四边形单元2: [3, 4, 5, 6]
 *   三角形单元3: [7, 8, 9]
 * 
 *   VertexIndices = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
 *   CellOffsets = [0, 3, 7, 10]  // 每个单元的起始位置
 *   CellTypes = [Triangle, Quad, Triangle]
 * 
 */
class FCellArray {
public:
    // ============================================================================
    // 类型定义
    // ============================================================================
    /** 顶点索引类型 */
    using VertexIndexType = FCellInfo::VertexIndexType;

    /** 单元索引类型 */
    using CellIndexType = FCellInfo::CellIndexType;


private:
    // ============================================================================
    // 内部存储
    // ============================================================================
    
    /** 所有单元的顶点索引（连续存储） */
    TArray<VertexIndexType> VertexIndices;
    
    /** 每个单元在VertexIndices中的起始偏移位置 */
    TArray<uint32> CellOffsets;
    
    /** 每个单元的类型 */
    TArray<ECellType> CellTypes;
    
    /** 当前容量（用于内存预分配） */
    uint32 ReservedCapacity;
    
    /** 是否启用内存重用 */
    bool bEnableMemoryReuse;

public:
    // ============================================================================
    // 构造函数和析构函数
    // ============================================================================
    
    /** 默认构造函数 */
    FCellArray();
    
    /** 使用初始容量构造 */
    explicit FCellArray(uint32 InitialCapacity);
    
    /** 拷贝构造函数 */
    FCellArray(const FCellArray& Other);
    
    /** 移动构造函数 */
    FCellArray(FCellArray&& Other) noexcept;
    
    /** 析构函数 */
    ~FCellArray();
    
    /** 拷贝赋值 */
    FCellArray& operator=(const FCellArray& Other);
    
    /** 移动赋值 */
    FCellArray& operator=(FCellArray&& Other) noexcept;

    // ============================================================================
    // 容量和大小
    // ============================================================================
    
    /** 获取单元数量 */
    [[nodiscard]] uint32 GetCellCount() const;
    
    /** 获取顶点索引总数 */
    [[nodiscard]] uint32 GetVertexIndexCount() const;
    
    /** 检查是否为空 */
    [[nodiscard]] bool IsEmpty() const;
    
    /** 预留容量（优化内存分配） */
    void Reserve(uint32 Capacity);
    
    /** 收缩容量以匹配实际大小 */
    void Shrink();

    // ============================================================================
    // 添加单元
    // ============================================================================
    
    /**
     * 添加一个单元
     * @param CellType 单元类型
     * @param InVertexIndices 顶点索引数组
     */
    void AddCell(ECellType CellType, const TArray<VertexIndexType>& InVertexIndices);
    
    /**
     * 添加一个单元（使用C风格数组）
     * @param CellType 单元类型
     * @param InVertexIndices 顶点索引数组指针
     * @param VertexCount 顶点数量
     */
    void AddCell(ECellType CellType, const VertexIndexType* InVertexIndices, uint32 VertexCount);
    
    /**
     * 批量添加单元
     * @param Cells 单元信息数组
     */
    void AddCells(const TArray<FCellInfo>& Cells);

    // ============================================================================
    // 获取单元
    // ============================================================================
    
    /**
     * 获取指定索引的单元信息
     * @param CellIndex 单元索引
     * @param OutCellInfo 输出的单元信息
     * @return 是否成功获取
     */
    bool GetCell(CellIndexType CellIndex, FCellInfo& OutCellInfo) const;
    
    /**
     * 获取指定索引的单元类型
     * @param CellIndex 单元索引
     * @return 单元类型，如果索引无效返回 Unknown
     */
    [[nodiscard]] ECellType GetCellType(CellIndexType CellIndex) const;
    
    /**
     * 获取指定索引的单元的顶点索引列表
     * @param CellIndex 单元索引
     * @param OutVertexIndices 输出的顶点索引列表
     * @return 是否成功获取
     */
    bool GetCellVertexIndices(CellIndexType CellIndex, TArray<VertexIndexType>& OutVertexIndices) const;
    
    /**
     * 获取指定索引的单元的顶点数量
     * @param CellIndex 单元索引
     * @return 顶点数量，如果索引无效返回 0
     */
    [[nodiscard]] uint32 GetCellVertexCount(CellIndexType CellIndex) const;
    
    /**
     * 获取指定索引的单元的顶点索引（通过指针访问，更高效）
     * @param CellIndex 单元索引
     * @param OutVertexCount 输出的顶点数量
     * @return 顶点索引数组的指针，如果索引无效返回 nullptr
     */
    const VertexIndexType* GetCellVertexIndicesPtr(CellIndexType CellIndex, uint32& OutVertexCount) const;

    // ============================================================================
    // 删除单元
    // ============================================================================
    
    /**
     * 删除指定索引的单元
     * @param CellIndex 单元索引
     * @return 是否成功删除
     */
    bool RemoveCell(CellIndexType CellIndex);
    
    /**
     * 删除指定范围的单元
     * @param StartIndex 起始索引
     * @param Count 要删除的数量
     * @return 实际删除的数量
     */
    uint32 RemoveCells(CellIndexType StartIndex, uint32 Count);
    
    /**
     * 删除指定类型的所有单元
     * @param CellType 要删除的单元类型
     * @return 删除的单元数量
     */
    uint32 RemoveCellsByType(ECellType CellType);
    
    /**
     * 清空所有单元
     */
    void Clear();
    
    /**
     * 清空所有单元（别名）
     */
    void Reset();
    void Empty();

    // ============================================================================
    // 查询和统计
    // ============================================================================
    
    /**
     * 检查单元索引是否有效
     * @param CellIndex 单元索引
     * @return 是否有效
     */
    [[nodiscard]] bool IsValidCellIndex(CellIndexType CellIndex) const;
    
    /**
     * 获取指定类型的单元数量
     * @param CellType 单元类型
     * @return 该类型的单元数量
     */
    [[nodiscard]] uint32 GetCellCountByType(ECellType CellType) const;
    
    /**
     * 获取所有单元类型（去重）
     * @param OutTypes 输出的类型列表
     */
    void GetCellTypes(TArray<ECellType>& OutTypes) const;
    
    /**
     * 查找包含指定顶点的所有单元
     * @param VertexIndex 顶点索引
     * @param OutCellIndices 输出的单元索引列表
     * @return 找到的单元数量
     */
    uint32 FindCellsContainingVertex(VertexIndexType VertexIndex, TArray<CellIndexType>& OutCellIndices) const;

    // ============================================================================
    // 内存管理
    // ============================================================================
    
    /**
     * 启用或禁用内存重用
     * @param bEnable 是否启用
     */
    void SetMemoryReuseEnabled(bool bEnable);
    
    /**
     * 检查是否启用内存重用
     * @return 是否启用
     */
    [[nodiscard]] bool IsMemoryReuseEnabled() const;
    
    /**
     * 压缩存储（移除未使用的内存）
     */
    void Compact();
    
    /**
     * 获取当前内存使用情况（估算）
     * @return 内存使用量（字节）
     */
    [[nodiscard]] uint64 GetMemoryUsage() const;
};
