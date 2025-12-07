#pragma once

#include <exception>
#include <string>
#include "HAL/Platform.h"

/**
 * 异常错误码
 */
enum class EExceptionCode : int32
{
    Unknown = 0,
    InvalidArgument,     // 无效参数
    OutOfRange,          // 超出范围
    NullPointer,         // 空指针
    InvalidOperation,    // 无效操作
    MemoryAllocation,    // 内存分配失败
    FileIO,              // 文件IO错误
    Network,             // 网络错误
    ResourceNotFound,    // 资源未找到
    AlreadyExists,       // 资源已存在
    InvalidState,        // 无效状态
    Timeout,             // 超时
    NotSupported         // 不支持的操作
};

/**
 * FException - 自定义异常基类
 * 
 * 设计特点：
 * 1. 继承自 std::exception，兼容标准异常处理
 * 2. 支持错误码、错误消息、文件位置信息
 * 3. 支持异常链（嵌套异常）
 */
class FException : public std::exception
{
private:
    /** 错误消息 */
    std::string Message;
    
    /** 错误码 */
    EExceptionCode ErrorCode;
    
    /** 文件名（可选） */
    std::string FileName;
    
    /** 行号（可选） */
    int32 LineNumber;
    
    /** 函数名（可选） */
    std::string FunctionName;
    
    /** 嵌套异常（异常链） */
    const FException* NestedException;

public:
    /**
     * 构造函数
     * @param InMessage 错误消息
     * @param InErrorCode 错误码（默认 Unknown）
     * @param InFileName 文件名（可选）
     * @param InLineNumber 行号（可选）
     * @param InFunctionName 函数名（可选）
     */
    FException(
        const std::string& InMessage,
        EExceptionCode InErrorCode = EExceptionCode::Unknown,
        const std::string& InFileName = "",
        int32 InLineNumber = 0,
        const std::string& InFunctionName = ""
    );
    
    /**
     * 带嵌套异常的构造函数
     * @param InMessage 错误消息
     * @param InNestedException 嵌套异常
     * @param InErrorCode 错误码
     */
    FException(
        const std::string& InMessage,
        const FException& InNestedException,
        EExceptionCode InErrorCode = EExceptionCode::Unknown
    );
    
    /**
     * 拷贝构造函数
     */
    FException(const FException& Other);
    
    /**
     * 移动构造函数
     */
    FException(FException&& Other) noexcept;
    
    /**
     * 析构函数
     */
    ~FException() noexcept override = default;
    
    /**
     * 拷贝赋值
     */
    FException& operator=(const FException& Other);
    
    /**
     * 移动赋值
     */
    FException& operator=(FException&& Other) noexcept;
    
    /**
     * 获取错误消息（std::exception 接口）
     */
    const char* what() const noexcept override;
    
    /**
     * 获取错误消息（字符串版本）
     */
    const std::string& GetMessage() const;
    
    /**
     * 获取错误码
     */
    EExceptionCode GetErrorCode() const;
    
    /**
     * 获取文件名
     */
    const std::string& GetFileName() const;
    
    /**
     * 获取行号
     */
    int32 GetLineNumber() const;
    
    /**
     * 获取函数名
     */
    const std::string& GetFunctionName() const;
    
    /**
     * 获取嵌套异常
     */
    const FException* GetNestedException() const;
    
    /**
     * 获取完整的错误信息（包含位置信息）
     */
    std::string GetFullMessage() const;
    
    /**
     * 获取错误码的字符串表示
     */
    static std::string ErrorCodeToString(EExceptionCode Code);
};

/**
 * FInvalidArgumentException - 无效参数异常
 */
class FInvalidArgumentException : public FException
{
public:
    FInvalidArgumentException(
        const std::string& InMessage,
        const std::string& InFileName = "",
        int32 InLineNumber = 0,
        const std::string& InFunctionName = ""
    );
};

/**
 * FOutOfRangeException - 超出范围异常
 */
class FOutOfRangeException : public FException
{
public:
    FOutOfRangeException(
        const std::string& InMessage,
        const std::string& InFileName = "",
        int32 InLineNumber = 0,
        const std::string& InFunctionName = ""
    );
};

/**
 * FNullPointerException - 空指针异常
 */
class FNullPointerException : public FException
{
public:
    FNullPointerException(
        const std::string& InMessage,
        const std::string& InFileName = "",
        int32 InLineNumber = 0,
        const std::string& InFunctionName = ""
    );
};

/**
 * FInvalidOperationException - 无效操作异常
 */
class FInvalidOperationException : public FException
{
public:
    FInvalidOperationException(
        const std::string& InMessage,
        const std::string& InFileName = "",
        int32 InLineNumber = 0,
        const std::string& InFunctionName = ""
    );
};

/**
 * FMemoryAllocationException - 内存分配异常
 */
class FMemoryAllocationException : public FException
{
public:
    FMemoryAllocationException(
        const std::string& InMessage = "内存分配失败",
        const std::string& InFileName = "",
        int32 InLineNumber = 0,
        const std::string& InFunctionName = ""
    );
};

/**
 * FFileIOException - 文件IO异常
 */
class FFileIOException : public FException
{
public:
    FFileIOException(
        const std::string& InMessage,
        const std::string& InFileName = "",
        int32 InLineNumber = 0,
        const std::string& InFunctionName = ""
    );
};

/**
 * FResourceNotFoundException - 资源未找到异常
 */
class FResourceNotFoundException : public FException
{
public:
    FResourceNotFoundException(
        const std::string& InResourceName,
        const std::string& InFileName = "",
        int32 InLineNumber = 0,
        const std::string& InFunctionName = ""
    );
};

/**
 * FInvalidStateException - 无效状态异常
 */
class FInvalidStateException : public FException
{
public:
    FInvalidStateException(
        const std::string& InMessage,
        const std::string& InFileName = "",
        int32 InLineNumber = 0,
        const std::string& InFunctionName = ""
    );
};

/**
 * FNotSupportedException - 不支持的操作异常
 */
class FNotSupportedException : public FException
{
public:
    FNotSupportedException(
        const std::string& InMessage,
        const std::string& InFileName = "",
        int32 InLineNumber = 0,
        const std::string& InFunctionName = ""
    );
};

// ============================================================================
// 辅助宏
// ============================================================================

/**
 * 抛出异常（自动包含文件、行号、函数名信息）
 */
#define THROW_EXCEPTION(ExceptionType, Message) \
    throw ExceptionType(Message, __FILE__, __LINE__, __FUNCTION__)

/**
 * 抛出异常（带错误码）
 */
#define THROW_EXCEPTION_WITH_CODE(ExceptionType, Message, Code) \
    throw ExceptionType(Message, Code, __FILE__, __LINE__, __FUNCTION__)

/**
 * 检查条件，如果失败则抛出异常
 */
#define CHECK_OR_THROW(Condition, ExceptionType, Message) \
    do { \
        if (!(Condition)) { \
            THROW_EXCEPTION(ExceptionType, Message); \
        } \
    } while(0)

/**
 * 检查指针，如果为空则抛出异常
 */
#define CHECK_POINTER_OR_THROW(Ptr, Message) \
    CHECK_OR_THROW((Ptr) != nullptr, FNullPointerException, Message)

/**
 * 检查索引范围，如果超出范围则抛出异常
 */
#define CHECK_INDEX_OR_THROW(Index, Size, Message) \
    do { \
        if ((Index) >= (Size)) { \
            THROW_EXCEPTION(FOutOfRangeException, Message); \
        } \
    } while(0)

