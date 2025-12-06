#pragma once

#include "Rendering/RenderCommandQueue.h"
#include "Memory/UniquePtr.h"
#include <utility>


/**
 * 模板渲染命令类
 * 用于将Lambda或函数对象封装为渲染命令
 */
template<typename LambdaType>
class TLambdaRenderCommand : public FRenderCommand {
public:
    explicit TLambdaRenderCommand(LambdaType&& InLambda)
        : Lambda(std::forward<LambdaType>(InLambda))
    {
    }

    virtual void Execute() override
    {
        Lambda();
    }

private:
    LambdaType Lambda;
};

/**
 * ENQUEUE_RENDER_COMMAND - 将命令提交到渲染线程的宏
 * 
 * 使用示例：
 *   ENQUEUE_RENDER_COMMAND(UpdateMeshCommand)(
 *       [MeshProxy]() {
 *           // 在渲染线程中执行的代码
 *           MeshProxy->UpdateData();
 *       }
 *   );
 * 
 * 注意：Lambda中捕获的变量必须是线程安全的，或者确保在渲染线程执行时仍然有效
 */
#define ENQUEUE_RENDER_COMMAND(CommandName) \
    [](auto&& Lambda) { \
        auto Command = MakeUnique<TLambdaRenderCommand<std::decay_t<decltype(Lambda)>>>(std::forward<decltype(Lambda)>(Lambda)); \
        FRenderCommandQueue::Get().EnqueueCommand(std::move(Command)); \
    }
