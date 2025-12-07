#include "../../Public/Exception/Exception.h"
#include <sstream>

// ============================================================================
// FException 实现
// ============================================================================

FException::FException(
    const std::string& InMessage,
    EExceptionCode InErrorCode,
    const std::string& InFileName,
    int32 InLineNumber,
    const std::string& InFunctionName
)
    : Message(InMessage)
    , ErrorCode(InErrorCode)
    , FileName(InFileName)
    , LineNumber(InLineNumber)
    , FunctionName(InFunctionName)
    , NestedException(nullptr)
{
}

FException::FException(
    const std::string& InMessage,
    const FException& InNestedException,
    EExceptionCode InErrorCode
)
    : Message(InMessage)
    , ErrorCode(InErrorCode)
    , FileName("")
    , LineNumber(0)
    , FunctionName("")
    , NestedException(&InNestedException)
{
}

FException::FException(const FException& Other)
    : Message(Other.Message)
    , ErrorCode(Other.ErrorCode)
    , FileName(Other.FileName)
    , LineNumber(Other.LineNumber)
    , FunctionName(Other.FunctionName)
    , NestedException(Other.NestedException)
{
}

FException::FException(FException&& Other) noexcept
    : Message(std::move(Other.Message))
    , ErrorCode(Other.ErrorCode)
    , FileName(std::move(Other.FileName))
    , LineNumber(Other.LineNumber)
    , FunctionName(std::move(Other.FunctionName))
    , NestedException(Other.NestedException)
{
    Other.NestedException = nullptr;
}

FException& FException::operator=(const FException& Other)
{
    if (this != &Other)
    {
        Message = Other.Message;
        ErrorCode = Other.ErrorCode;
        FileName = Other.FileName;
        LineNumber = Other.LineNumber;
        FunctionName = Other.FunctionName;
        NestedException = Other.NestedException;
    }
    return *this;
}

FException& FException::operator=(FException&& Other) noexcept
{
    if (this != &Other)
    {
        Message = std::move(Other.Message);
        ErrorCode = Other.ErrorCode;
        FileName = std::move(Other.FileName);
        LineNumber = Other.LineNumber;
        FunctionName = std::move(Other.FunctionName);
        NestedException = Other.NestedException;
        Other.NestedException = nullptr;
    }
    return *this;
}

const char* FException::what() const noexcept
{
    return Message.c_str();
}

const std::string& FException::GetMessage() const
{
    return Message;
}

EExceptionCode FException::GetErrorCode() const
{
    return ErrorCode;
}

const std::string& FException::GetFileName() const
{
    return FileName;
}

int32 FException::GetLineNumber() const
{
    return LineNumber;
}

const std::string& FException::GetFunctionName() const
{
    return FunctionName;
}

const FException* FException::GetNestedException() const
{
    return NestedException;
}

std::string FException::GetFullMessage() const
{
    std::ostringstream Oss;
    
    Oss << "[" << ErrorCodeToString(ErrorCode) << "] " << Message;
    
    if (!FileName.empty())
    {
        Oss << "\n  文件: " << FileName;
        if (LineNumber > 0)
        {
            Oss << ":" << LineNumber;
        }
    }
    
    if (!FunctionName.empty())
    {
        Oss << "\n  函数: " << FunctionName;
    }
    
    if (NestedException != nullptr)
    {
        Oss << "\n  嵌套异常: " << NestedException->GetFullMessage();
    }
    
    return Oss.str();
}

std::string FException::ErrorCodeToString(EExceptionCode Code)
{
    switch (Code)
    {
        case EExceptionCode::Unknown:
            return "Unknown";
        case EExceptionCode::InvalidArgument:
            return "InvalidArgument";
        case EExceptionCode::OutOfRange:
            return "OutOfRange";
        case EExceptionCode::NullPointer:
            return "NullPointer";
        case EExceptionCode::InvalidOperation:
            return "InvalidOperation";
        case EExceptionCode::MemoryAllocation:
            return "MemoryAllocation";
        case EExceptionCode::FileIO:
            return "FileIO";
        case EExceptionCode::Network:
            return "Network";
        case EExceptionCode::ResourceNotFound:
            return "ResourceNotFound";
        case EExceptionCode::AlreadyExists:
            return "AlreadyExists";
        case EExceptionCode::InvalidState:
            return "InvalidState";
        case EExceptionCode::Timeout:
            return "Timeout";
        case EExceptionCode::NotSupported:
            return "NotSupported";
        default:
            return "Unknown";
    }
}

// ============================================================================
// 派生异常类实现
// ============================================================================

FInvalidArgumentException::FInvalidArgumentException(
    const std::string& InMessage,
    const std::string& InFileName,
    int32 InLineNumber,
    const std::string& InFunctionName
)
    : FException(InMessage, EExceptionCode::InvalidArgument, InFileName, InLineNumber, InFunctionName)
{
}

FOutOfRangeException::FOutOfRangeException(
    const std::string& InMessage,
    const std::string& InFileName,
    int32 InLineNumber,
    const std::string& InFunctionName
)
    : FException(InMessage, EExceptionCode::OutOfRange, InFileName, InLineNumber, InFunctionName)
{
}

FNullPointerException::FNullPointerException(
    const std::string& InMessage,
    const std::string& InFileName,
    int32 InLineNumber,
    const std::string& InFunctionName
)
    : FException(InMessage, EExceptionCode::NullPointer, InFileName, InLineNumber, InFunctionName)
{
}

FInvalidOperationException::FInvalidOperationException(
    const std::string& InMessage,
    const std::string& InFileName,
    int32 InLineNumber,
    const std::string& InFunctionName
)
    : FException(InMessage, EExceptionCode::InvalidOperation, InFileName, InLineNumber, InFunctionName)
{
}

FMemoryAllocationException::FMemoryAllocationException(
    const std::string& InMessage,
    const std::string& InFileName,
    int32 InLineNumber,
    const std::string& InFunctionName
)
    : FException(InMessage, EExceptionCode::MemoryAllocation, InFileName, InLineNumber, InFunctionName)
{
}

FFileIOException::FFileIOException(
    const std::string& InMessage,
    const std::string& InFileName,
    int32 InLineNumber,
    const std::string& InFunctionName
)
    : FException(InMessage, EExceptionCode::FileIO, InFileName, InLineNumber, InFunctionName)
{
}

FResourceNotFoundException::FResourceNotFoundException(
    const std::string& InResourceName,
    const std::string& InFileName,
    int32 InLineNumber,
    const std::string& InFunctionName
)
    : FException("资源未找到: " + InResourceName, EExceptionCode::ResourceNotFound, InFileName, InLineNumber, InFunctionName)
{
}

FInvalidStateException::FInvalidStateException(
    const std::string& InMessage,
    const std::string& InFileName,
    int32 InLineNumber,
    const std::string& InFunctionName
)
    : FException(InMessage, EExceptionCode::InvalidState, InFileName, InLineNumber, InFunctionName)
{
}

FNotSupportedException::FNotSupportedException(
    const std::string& InMessage,
    const std::string& InFileName,
    int32 InLineNumber,
    const std::string& InFunctionName
)
    : FException(InMessage, EExceptionCode::NotSupported, InFileName, InLineNumber, InFunctionName)
{
}

