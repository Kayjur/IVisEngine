#pragma once

#include "Mesh/MeshBase.h"
#include "Container/Array.h"
#include "Container/Map.h"
#include "Memory/UniquePtr.h"
#include <string>

// 前向声明
class FCellArray;
class FField;

/**
 * IMesh - 基础网格类
 * 
 * 设计特点：
 * 1. 存储几何数据（顶点坐标）- 使用值类型TArray
 * 2. 存储拓扑数据（单元连接关系）- 使用UniquePtr减少编译依赖
 * 3. 存储场数据（顶点场和单元场分开存储）- 使用指针减少编译依赖
 * 4. 提供统一的数据访问接口
 * 5. 使用前向声明和指针减少编译时间
 * 
 * 核心概念：
 * - 几何与拓扑分离：顶点坐标和单元拓扑分开存储
 * - 场数据分离：顶点场和单元场分开管理
 */
class IMesh : public IMeshBase
{
private:
    // ============================================================================
    // 几何数据（值类型，直接使用）
    // ============================================================================
    
    /** 顶点坐标数组 */
    TArray<FVector> Vertices;
    
    // ============================================================================
    // 拓扑数据（使用UniquePtr）
    // ============================================================================
    
    /** 单元数组（存储拓扑信息） */
    TUniquePtr<FCellArray> Cells;
    
    // ============================================================================
    // 场数据
    // ============================================================================
    
    /** 顶点场数据容器（键为场名称，值为场数据指针） */
    TMap<std::string, TUniquePtr<FField>> VertexFields;
    
    /** 单元场数据容器（键为场名称，值为场数据指针） */
    TMap<std::string, TUniquePtr<FField>> CellFields;
    
    // ============================================================================
    // 元数据
    // ============================================================================
    
    /** 网格名称 */
    std::string MeshName;
    
    /** 是否有效 */
    bool bIsValid;

public:
    // ============================================================================
    // 构造函数和析构函数
    // ============================================================================
    
    /** 默认构造函数 */
    IMesh();
    
    /**
     * 使用网格名称构造
     * @param InMeshName 网格名称
     */
    explicit IMesh(const std::string& InMeshName);
    
    /** 拷贝构造函数 */
    IMesh(const IMesh& Other);
    
    /** 移动构造函数 */
    IMesh(IMesh&& Other) noexcept;
    
    /** 析构函数 */
    ~IMesh() override = default;
    
    /** 拷贝赋值 */
    IMesh& operator=(const IMesh& Other);
    
    /** 移动赋值 */
    IMesh& operator=(IMesh&& Other) noexcept;
    
    // ============================================================================
    // 几何数据操作（实现IMeshBase接口）
    // ============================================================================
    
    /** 获取顶点数量 */
    [[nodiscard]] uint32 GetVertexCount() const override;
    
    /** 获取指定索引的顶点坐标 */
    [[nodiscard]] FVector GetVertex(uint32 Index) const override;
    
    /** 获取顶点坐标数组的原始指针 */
    [[nodiscard]] const FVector* GetVerticesPtr() const override;
    
    /** 检查顶点索引是否有效 */
    [[nodiscard]] bool IsValidVertexIndex(uint32 Index) const override;
    
    // ============================================================================
    // 几何数据操作（扩展接口）
    // ============================================================================
    
    /**
     * 添加顶点
     * @param Vertex 顶点坐标
     */
    void AddVertex(const FVector& Vertex);
    void AddVertex(float X, float Y, float Z);

    /**
     * 批量添加顶点
     * @param InVertices 顶点数组
     */
    void AddVertices(const TArray<FVector>& InVertices);
    
    /**
     * 批量添加顶点（移动语义）
     * @param InVertices 顶点数组
     */
    void AddVertices(TArray<FVector>&& InVertices);
    
    /**
     * 设置指定索引的顶点坐标
     * @param Index 顶点索引
     * @param Vertex 顶点坐标
     */
    void SetVertex(uint32 Index, const FVector& Vertex);
    void SetVertex(uint32 Index, float X, float Y, float Z);

    /**
     * 获取所有顶点（常量引用）
     * @return 顶点数组常量引用
     */
    [[nodiscard]] const TArray<FVector>& GetVertices() const;
    
    // ============================================================================
    // 拓扑数据操作（实现IMeshBase接口）
    // ============================================================================
    
    /** 获取单元数量 */
    [[nodiscard]] uint32 GetCellCount() const override;
    
    /** 获取单元数组的引用 */
    [[nodiscard]] FCellArray& GetCells() override;
    
    /** 获取单元数组的常量引用 */
    [[nodiscard]] const FCellArray& GetCells() const override;
    
    /** 检查单元索引是否有效 */
    [[nodiscard]] bool IsValidCellIndex(uint32 Index) const override;
    
    // ============================================================================
    // 场数据操作（实现IMeshBase接口）
    // ============================================================================
    
    /** 获取指定名称的场数据（自动查找顶点场和单元场） */
    [[nodiscard]] FField* GetField(const std::string& FieldName) override;
    
    /** 获取指定名称的场数据（常量版本） */
    [[nodiscard]] const FField* GetField(const std::string& FieldName) const override;
    
    /** 检查是否存在指定名称的场数据 */
    [[nodiscard]] bool HasField(const std::string& FieldName) const override;
    
    /** 获取指定名称的顶点场数据 */
    [[nodiscard]] FField* GetVertexField(const std::string& FieldName) override;
    
    /** 获取指定名称的顶点场数据（常量版本） */
    [[nodiscard]] const FField* GetVertexField(const std::string& FieldName) const override;
    
    /** 获取指定名称的单元场数据 */
    [[nodiscard]] FField* GetCellField(const std::string& FieldName) override;
    
    /** 获取指定名称的单元场数据（常量版本） */
    [[nodiscard]] const FField* GetCellField(const std::string& FieldName) const override;
    
    /** 检查是否存在指定名称的顶点场数据 */
    [[nodiscard]] bool HasVertexField(const std::string& FieldName) const override;
    
    /** 检查是否存在指定名称的单元场数据 */
    [[nodiscard]] bool HasCellField(const std::string& FieldName) const override;
    
    // ============================================================================
    // 场数据操作（扩展接口）
    // ============================================================================
    
    /**
     * 添加场数据（自动判断类型）
     * @param Field 场数据指针（移动所有权）
     * @return 是否成功添加（如果已存在同名场则返回false）
     */
    bool AddField(TUniquePtr<FField> Field);
    
    /**
     * 添加或替换场数据
     * @param Field 场数据指针（移动所有权）
     */
    void SetField(TUniquePtr<FField> Field);
    
    /**
     * 删除指定名称的场数据
     * @param FieldName 场名称
     * @return 是否成功删除
     */
    bool RemoveField(const std::string& FieldName);
    
    /**
     * 删除指定名称的顶点场数据
     * @param FieldName 场名称
     * @return 是否成功删除
     */
    bool RemoveVertexField(const std::string& FieldName);
    
    /**
     * 删除指定名称的单元场数据
     * @param FieldName 场名称
     * @return 是否成功删除
     */
    bool RemoveCellField(const std::string& FieldName);
    
    /**
     * 获取所有顶点场名称
     * @param OutFieldNames 输出的场名称数组
     */
    void GetVertexFieldNames(TArray<std::string>& OutFieldNames) const;
    
    /**
     * 获取所有单元场名称
     * @param OutFieldNames 输出的场名称数组
     */
    void GetCellFieldNames(TArray<std::string>& OutFieldNames) const;
    
    /**
     * 获取所有场名称（包括顶点场和单元场）
     * @param OutFieldNames 输出的场名称数组
     */
    void GetAllFieldNames(TArray<std::string>& OutFieldNames) const;
    
    // ============================================================================
    // 元数据操作（实现IMeshBase接口）
    // ============================================================================
    
    /** 获取网格名称 */
    [[nodiscard]] const std::string& GetMeshName() const override;
    
    /** 检查网格是否有效 */
    [[nodiscard]] bool IsValid() const override;
    
    // ============================================================================
    // 元数据操作（扩展接口）
    // ============================================================================
    
    /**
     * 设置网格名称
     * @param InName 网格名称
     */
    void SetMeshName(const std::string& InName);
    
    // ============================================================================
    // 数据验证（实现IMeshBase接口）
    // ============================================================================
    
    /**
     * 验证网格数据的完整性和一致性
     * 检查：
     * - 顶点数量是否匹配
     * - 单元索引是否有效
     * - 场数据数量是否匹配顶点数/单元数
     * @return 是否有效
     */
    [[nodiscard]] bool Validate() const override;
    
    // ============================================================================
    // 清空和重置（实现IMeshBase接口）
    // ============================================================================
    
    /** 清空所有数据 */
    void Clear() override;
    
    /** 重置网格到初始状态 */
    void Reset() override;
    
    // ============================================================================
    // 内存管理
    // ============================================================================

    /**
     * 预留顶点容量
     * @param Capacity 容量
     */
    void ReserveVertices(uint32 Capacity);

    /**
     * 预留单元容量
     * @param Capacity 容量
     */
    void ReserveCells(uint32 Capacity);
    
    /**
     * 收缩内存以匹配实际大小
     */
    void Shrink();
};

