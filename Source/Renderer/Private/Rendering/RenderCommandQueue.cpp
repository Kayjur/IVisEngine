#include "Rendering/RenderCommandQueue.h"

FRenderCommandQueue& FRenderCommandQueue::Get()
{
    static FRenderCommandQueue Instance;
    return Instance;
}

void FRenderCommandQueue::EnqueueCommand(TUniquePtr<FRenderCommand> Command)
{
    std::lock_guard<std::mutex> Lock(QueueMutex);
    CommandQueue.push(std::move(Command));
}

void FRenderCommandQueue::ProcessCommands()
{
    std::queue<TUniquePtr<FRenderCommand>> CommandsToProcess;
    
    // 快速交换，减少锁持有时间
    {
        std::lock_guard<std::mutex> Lock(QueueMutex);
        CommandsToProcess.swap(CommandQueue);
    }
    
    // 执行所有命令
    while (!CommandsToProcess.empty())
    {
        auto Command = std::move(CommandsToProcess.front());
        CommandsToProcess.pop();
        if (Command)
        {
            Command->Execute();
        }
    }
}

uint32 FRenderCommandQueue::GetPendingCommandCount() const
{
    std::lock_guard<std::mutex> Lock(QueueMutex);
    return static_cast<uint32>(CommandQueue.size());
}

void FRenderCommandQueue::Flush()
{
    std::lock_guard<std::mutex> Lock(QueueMutex);
    while (!CommandQueue.empty())
    {
        CommandQueue.pop();
    }
}

