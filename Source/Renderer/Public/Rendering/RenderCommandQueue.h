#pragma once

#include <functional>
#include <queue>
#include <mutex>
#include <memory>
#include "HAL/Platform.h"
#include "Memory/UniquePtr.h"

/**
 * FRenderCommand - 渲染命令基类
 * 用于将游戏线程的渲染请求提交到渲染线程执行
 */
class FRenderCommand
{
public:
    virtual ~FRenderCommand() = default;
    
    /**
     * 在渲染线程中执行此命令
     */
    virtual void Execute() = 0;
};

/**
 * FRenderCommandQueue - 渲染命令队列
 * 线程安全的命令队列，用于游戏线程向渲染线程提交命令
 */
class FRenderCommandQueue
{
public:

    FRenderCommandQueue(const FRenderCommandQueue&) = delete;
    FRenderCommandQueue& operator=(const FRenderCommandQueue&) = delete;

    /**
     * 获取全局渲染命令队列单例
     * @return 渲染命令队列引用
     */
    static FRenderCommandQueue& Get();

    /**
     * 将渲染命令添加到队列（从游戏线程调用）
     * @param Command 要执行的渲染命令
     */
    void EnqueueCommand(TUniquePtr<FRenderCommand> Command);

    /**
     * 处理队列中的所有命令（在渲染线程中调用）
     * 执行队列中的所有命令并清空队列
     */
    void ProcessCommands();

    /**
     * 获取队列中待处理的命令数量
     * @return 命令数量
     */
    uint32 GetPendingCommandCount() const;

    /**
     * 清空所有待处理的命令
     */
    void Flush();

private:
    FRenderCommandQueue() = default;
    ~FRenderCommandQueue() = default;


    mutable std::mutex QueueMutex;
    std::queue<TUniquePtr<FRenderCommand>> CommandQueue;
};
