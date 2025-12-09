#pragma once

#include <map>
#include <algorithm>
#include <stdexcept>

/**
 * TMultiMap - 键值对多重映射容器类
 * 使用 UE 风格的接口命名，底层使用 std::multimap 实现
 * 允许一个键对应多个值
 *
 * @tparam KeyType 键类型
 * @tparam ValueType 值类型
 */
template<typename KeyType, typename ValueType>
class TMultiMap
{
public:
    // ============================================================================
    // 类型定义
    // ============================================================================
    using KeyTypeAlias = KeyType;
    using ValueTypeAlias = ValueType;
    using MultiMapType = std::multimap<KeyType, ValueType>;
    using SizeType = typename MultiMapType::size_type;
    using Iterator = typename MultiMapType::iterator;
    using ConstIterator = typename MultiMapType::const_iterator;
    using PairType = std::pair<const KeyType, ValueType>;
    using RangeType = std::pair<Iterator, Iterator>;
    using ConstRangeType = std::pair<ConstIterator, ConstIterator>;

    // ============================================================================
    // 构造函数和析构函数
    // ============================================================================

    /** 默认构造函数 */
    TMultiMap() = default;

    /** 使用初始化列表构造 */
    TMultiMap(std::initializer_list<PairType> InitList) : Data(InitList) {}

    /** 拷贝构造函数 */
    TMultiMap(const TMultiMap& Other) : Data(Other.Data) {}

    /** 移动构造函数 */
    TMultiMap(TMultiMap&& Other) noexcept : Data(std::move(Other.Data)) {}

    /** 析构函数 */
    ~TMultiMap() = default;

    // ============================================================================
    // 赋值操作符
    // ============================================================================

    /** 拷贝赋值 */
    TMultiMap& operator=(const TMultiMap& Other)
    {
        if (this != &Other)
        {
            Data = Other.Data;
        }
        return *this;
    }

    /** 移动赋值 */
    TMultiMap& operator=(TMultiMap&& Other) noexcept
    {
        if (this != &Other)
        {
            Data = std::move(Other.Data);
        }
        return *this;
    }

    // ============================================================================
    // 元素访问
    // ============================================================================

    /** 获取指定键的值（检查存在性，返回第一个匹配的值，不存在则抛出异常） */
    ValueType& At(const KeyType& Key)
    {
        auto It = Data.find(Key);
        if (It != Data.end())
        {
            return It->second;
        }
        throw std::out_of_range("TMultiMap::At: Key not found");
    }

    /** 获取指定键的值（常量版本） */
    const ValueType& At(const KeyType& Key) const
    {
        auto It = Data.find(Key);
        if (It != Data.end())
        {
            return It->second;
        }
        throw std::out_of_range("TMultiMap::At: Key not found");
    }

    /** 查找元素，返回第一个匹配值的指针，未找到返回 nullptr */
    ValueType* Find(const KeyType& Key)
    {
        auto It = Data.find(Key);
        if (It != Data.end())
        {
            return &It->second;
        }
        return nullptr;
    }

    /** 查找元素，返回第一个匹配值的常量指针，未找到返回 nullptr */
    const ValueType* Find(const KeyType& Key) const
    {
        auto It = Data.find(Key);
        if (It != Data.end())
        {
            return &It->second;
        }
        return nullptr;
    }

    /** 查找所有匹配指定键的元素，返回迭代器范围 */
    RangeType FindAll(const KeyType& Key)
    {
        return Data.equal_range(Key);
    }

    /** 查找所有匹配指定键的元素，返回常量迭代器范围 */
    ConstRangeType FindAll(const KeyType& Key) const
    {
        return Data.equal_range(Key);
    }

    /** 查找第一个匹配的元素，返回迭代器 */
    Iterator FindFirst(const KeyType& Key)
    {
        return Data.find(Key);
    }

    /** 查找第一个匹配的元素，返回常量迭代器 */
    ConstIterator FindFirst(const KeyType& Key) const
    {
        return Data.find(Key);
    }

    // ============================================================================
    // 容量相关
    // ============================================================================

    /** 获取元素数量 */
    SizeType Num() const { return Data.size(); }

    /** 获取元素数量 */
    SizeType Size() const { return Data.size(); }

    /** 检查是否为空 */
    [[nodiscard]] bool IsEmpty() const { return Data.empty(); }

    /** 获取指定键的元素数量 */
    SizeType Count(const KeyType& Key) const
    {
        return Data.count(Key);
    }

    // ============================================================================
    // 添加元素
    // ============================================================================

    /** 添加键值对（multimap 允许多个相同键） */
    Iterator Add(const KeyType& Key, const ValueType& Value)
    {
        return Data.insert(std::make_pair(Key, Value));
    }

    /** 添加键值对（移动语义） */
    Iterator Add(const KeyType& Key, ValueType&& Value)
    {
        return Data.insert(std::make_pair(Key, std::move(Value)));
    }

    /** 添加键值对（使用 pair） */
    Iterator Add(const PairType& Pair)
    {
        return Data.insert(Pair);
    }

    /** 添加键值对（使用 pair，移动语义） */
    Iterator Add(PairType&& Pair)
    {
        return Data.insert(std::move(Pair));
    }

    /** 原地构造添加元素 */
    template<typename... Args>
    Iterator Emplace(const KeyType& Key, Args&&... InArgs)
    {
        return Data.emplace(Key, std::forward<Args>(InArgs)...);
    }

    // ============================================================================
    // 删除元素
    // ============================================================================

    /** 删除指定键的所有元素，返回删除的数量 */
    SizeType Remove(const KeyType& Key)
    {
        return Data.erase(Key);
    }

    /** 删除指定迭代器位置的元素 */
    Iterator Remove(Iterator It)
    {
        return Data.erase(It);
    }

    /** 删除指定范围的元素 */
    Iterator Remove(Iterator First, Iterator Last)
    {
        return Data.erase(First, Last);
    }

    /** 删除指定键的第一个匹配元素，返回是否成功 */
    bool RemoveFirst(const KeyType& Key)
    {
        auto It = Data.find(Key);
        if (It != Data.end())
        {
            Data.erase(It);
            return true;
        }
        return false;
    }

    /** 删除指定键的元素并复制值（如果存在） */
    bool RemoveAndCopyValue(const KeyType& Key, ValueType& OutValue)
    {
        auto It = Data.find(Key);
        if (It != Data.end())
        {
            OutValue = It->second;
            Data.erase(It);
            return true;
        }
        return false;
    }

    /** 清空所有元素 */
    void Reset() { Data.clear(); }
    void Empty() { Reset(); }
    void Clear() { Reset(); }

    // ============================================================================
    // 查找操作
    // ============================================================================

    /** 检查是否包含指定键 */
    bool Contains(const KeyType& Key) const
    {
        return Data.find(Key) != Data.end();
    }

    /** 查找元素，返回迭代器 */
    Iterator FindIterator(const KeyType& Key)
    {
        return Data.find(Key);
    }

    /** 查找元素，返回常量迭代器 */
    ConstIterator FindIterator(const KeyType& Key) const
    {
        return Data.find(Key);
    }

    /** 获取指定键的下界迭代器（第一个 >= Key 的元素） */
    Iterator LowerBound(const KeyType& Key)
    {
        return Data.lower_bound(Key);
    }

    /** 获取指定键的下界迭代器（常量版本） */
    ConstIterator LowerBound(const KeyType& Key) const
    {
        return Data.lower_bound(Key);
    }

    /** 获取指定键的上界迭代器（第一个 > Key 的元素） */
    Iterator UpperBound(const KeyType& Key)
    {
        return Data.upper_bound(Key);
    }

    /** 获取指定键的上界迭代器（常量版本） */
    ConstIterator UpperBound(const KeyType& Key) const
    {
        return Data.upper_bound(Key);
    }

    // ============================================================================
    // 迭代器
    // ============================================================================

    // UE 风格的大写方法
    Iterator Begin() { return Data.begin(); }
    ConstIterator Begin() const { return Data.begin(); }
    ConstIterator cBegin() const { return Data.cbegin(); }

    Iterator End() { return Data.end(); }
    ConstIterator End() const { return Data.end(); }
    ConstIterator cEnd() const { return Data.cend(); }

    // STL 风格的小写方法（用于范围 for 循环和 STL 算法）
    Iterator begin() { return Data.begin(); }
    ConstIterator begin() const { return Data.begin(); }
    ConstIterator cbegin() const { return Data.cbegin(); }

    Iterator end() { return Data.end(); }
    ConstIterator end() const { return Data.end(); }
    ConstIterator cend() const { return Data.cend(); }

    // ============================================================================
    // 其他操作
    // ============================================================================

    /** 交换两个多重映射 */
    void Swap(TMultiMap& Other) noexcept
    {
        Data.swap(Other.Data);
    }

    /** 获取底层 multimap 的引用（用于高级操作） */
    MultiMapType& GetData() { return Data; }
    const MultiMapType& GetData() const { return Data; }

private:
    // 底层使用 std::multimap 存储数据
    MultiMapType Data;
};