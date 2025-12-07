#pragma once

#include "Math/MathType.h"
#include "HAL/Platform.h"
#include <string>

// 前向声明，减少编译依赖
class FCellArray;
class FField;

/**
 * IMeshBase - 网格基类接口
 * 
 * 设计特点：
 * 1. 定义统一的网格访问接口
 * 2. 支持多态访问不同类型的网格
 * 3. 提供几何、拓扑、场数据的统一访问方式
 * 4. 作为所有网格类型的基类
 * 5. 使用前向声明减少编译依赖
 * 
 * 核心概念：
 * - 几何数据：顶点坐标（TArray<FVector>）
 * - 拓扑数据：单元连接关系（FCellArray*）
 * - 场数据：绑定到顶点或单元的属性数据（FField*）
 */
class IMeshBase
{
public:
    // ============================================================================
    // 构造函数和析构函数
    // ============================================================================
    
    /** 默认构造函数 */
    IMeshBase() = default;
    
    /** 虚析构函数，支持多态删除 */
    virtual ~IMeshBase() = default;
    
    // ============================================================================
    // 几何数据访问接口
    // ============================================================================
    
    /**
     * 获取顶点数量
     * @return 顶点数量
     */
    [[nodiscard]] virtual uint32 GetVertexCount() const = 0;
    
    /**
     * 获取指定索引的顶点坐标
     * @param Index 顶点索引
     * @return 顶点坐标
     */
    [[nodiscard]] virtual FVector GetVertex(uint32 Index) const = 0;
    
    /**
     * 获取顶点坐标数组的原始指针（用于GPU上传等）
     * @return 顶点坐标数组指针，如果无效返回 nullptr
     */
    [[nodiscard]] virtual const FVector* GetVerticesPtr() const = 0;
    
    /**
     * 检查顶点索引是否有效
     * @param Index 顶点索引
     * @return 是否有效
     */
    [[nodiscard]] virtual bool IsValidVertexIndex(uint32 Index) const = 0;
    
    // ============================================================================
    // 拓扑数据访问接口
    // ============================================================================
    
    /**
     * 获取单元数量
     * @return 单元数量
     */
    [[nodiscard]] virtual uint32 GetCellCount() const = 0;
    
    /**
     * 获取单元数组的引用（用于访问单元拓扑）
     * @return 单元数组引用
     */
    [[nodiscard]] virtual FCellArray& GetCells() = 0;
    
    /**
     * 获取单元数组的常量引用
     * @return 单元数组常量引用
     */
    [[nodiscard]] virtual const FCellArray& GetCells() const = 0;
    
    /**
     * 检查单元索引是否有效
     * @param Index 单元索引
     * @return 是否有效
     */
    [[nodiscard]] virtual bool IsValidCellIndex(uint32 Index) const = 0;
    
    // ============================================================================
    // 场数据访问接口
    // ============================================================================
    
    /**
     * 获取指定名称的场数据（自动查找顶点场和单元场）
     * @param FieldName 场名称
     * @return 场数据指针，如果不存在返回 nullptr
     */
    [[nodiscard]] virtual FField* GetField(const std::string& FieldName) = 0;
    
    /**
     * 获取指定名称的场数据（常量版本）
     * @param FieldName 场名称
     * @return 场数据常量指针，如果不存在返回 nullptr
     */
    [[nodiscard]] virtual const FField* GetField(const std::string& FieldName) const = 0;
    
    /**
     * 检查是否存在指定名称的场数据
     * @param FieldName 场名称
     * @return 是否存在
     */
    [[nodiscard]] virtual bool HasField(const std::string& FieldName) const = 0;
    
    /**
     * 获取指定名称的顶点场数据
     * @param FieldName 场名称
     * @return 顶点场数据指针，如果不存在返回 nullptr
     */
    [[nodiscard]] virtual FField* GetVertexField(const std::string& FieldName) = 0;
    
    /**
     * 获取指定名称的顶点场数据（常量版本）
     * @param FieldName 场名称
     * @return 顶点场数据常量指针，如果不存在返回 nullptr
     */
    [[nodiscard]] virtual const FField* GetVertexField(const std::string& FieldName) const = 0;
    
    /**
     * 获取指定名称的单元场数据
     * @param FieldName 场名称
     * @return 单元场数据指针，如果不存在返回 nullptr
     */
    [[nodiscard]] virtual FField* GetCellField(const std::string& FieldName) = 0;
    
    /**
     * 获取指定名称的单元场数据（常量版本）
     * @param FieldName 场名称
     * @return 单元场数据常量指针，如果不存在返回 nullptr
     */
    [[nodiscard]] virtual const FField* GetCellField(const std::string& FieldName) const = 0;
    
    /**
     * 检查是否存在指定名称的顶点场数据
     * @param FieldName 场名称
     * @return 是否存在
     */
    [[nodiscard]] virtual bool HasVertexField(const std::string& FieldName) const = 0;
    
    /**
     * 检查是否存在指定名称的单元场数据
     * @param FieldName 场名称
     * @return 是否存在
     */
    [[nodiscard]] virtual bool HasCellField(const std::string& FieldName) const = 0;
    
    // ============================================================================
    // 元数据访问接口
    // ============================================================================
    
    /**
     * 获取网格名称
     * @return 网格名称
     */
    [[nodiscard]] virtual const std::string& GetMeshName() const = 0;

    /**
     * 检查网格是否有效
     * @return 是否有效
     */
    [[nodiscard]] virtual bool IsValid() const = 0;
    
    // ============================================================================
    // 数据验证接口
    // ============================================================================
    
    /**
     * 验证网格数据的完整性和一致性
     * @return 是否有效
     */
    [[nodiscard]] virtual bool Validate() const = 0;
    
    // ============================================================================
    // 清空和重置接口
    // ============================================================================
    
    /**
     * 清空所有数据
     */
    virtual void Clear() = 0;
    
    /**
     * 重置网格到初始状态
     */
    virtual void Reset() = 0;
};

