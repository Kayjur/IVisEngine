#include <iostream>
#include <thread>
#include <chrono>
#include "../Framework/Public/Threading/FrameworkThread.h"
#include "../Renderer/Public/Threading/RendererThread.h"
#include "Components/StaticMeshMapping.h"

int main() {
    std::cout << "=== IVisEngine 启动 ===" << std::endl;

    // 创建Framework线程
    IFrameworkThread FrameworkThread;
    
    // 创建Renderer线程
    IRendererThread RendererThread;

    // 启动两个线程
    std::cout << "启动Framework线程..." << std::endl;
    if (!FrameworkThread.Start())
    {
        std::cerr << "启动Framework线程失败" << std::endl;
        return 1;
    }

    std::cout << "启动Renderer线程..." << std::endl;
    if (!RendererThread.Start())
    {
        std::cerr << "启动Renderer线程失败" << std::endl;
        FrameworkThread.Stop();
        FrameworkThread.WaitForCompletion();
        return 1;
    }

    // 等待线程初始化
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 在Framework线程中创建Component（模拟游戏线程）
    std::cout << "\n=== 创建Component示例 ===" << std::endl;
    
    // 创建网格组件
    IStaticMeshMapping* MeshComponent1 = new IStaticMeshMapping("Cube");
    IStaticMeshMapping* MeshComponent2 = new IStaticMeshMapping("Sphere");
    
    std::cout << "[GameThread] 创建Component: " << MeshComponent1->GetMeshName() 
              << " (ID: " << MeshComponent1->GetComponentId() << ")" << std::endl;
    std::cout << "[GameThread] 创建Component: " << MeshComponent2->GetMeshName() 
              << " (ID: " << MeshComponent2->GetComponentId() << ")" << std::endl;
    
    // 注册Component（会通过ENQUEUE_RENDER_COMMAND提交到渲染线程）
    MeshComponent1->RegisterComponent();
    MeshComponent2->RegisterComponent();
    
    std::cout << "[GameThread] Component已注册，渲染命令已提交到渲染线程" << std::endl;
    
    // 等待渲染线程处理命令
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    // 修改Component数据（模拟游戏逻辑更新）
    std::cout << "\n=== 更新Component数据 ===" << std::endl;
    MeshComponent1->SetMeshName("Cube_Updated");
    std::cout << "[GameThread] 更新Component数据，标记为脏" << std::endl;
    
    std::cout << "\n两个线程运行中..." << std::endl;
    std::cout << "按Enter键停止所有线程..." << std::endl;

    // 主线程等待用户输入
    std::cin.get();

    // 停止线程
    std::cout << "\n正在停止线程..." << std::endl;
    FrameworkThread.Stop();
    RendererThread.Stop();

    // 等待线程结束
    FrameworkThread.WaitForCompletion(5000);
    RendererThread.WaitForCompletion(5000);

    // 清理Component
    delete MeshComponent1;
    delete MeshComponent2;

    std::cout << "\n=== IVisEngine 关闭 ===" << std::endl;
    std::cout << "Framework线程总帧数: " << FrameworkThread.GetFrameCount() << std::endl;
    std::cout << "Renderer线程总渲染帧数: " << RendererThread.GetRenderFrameCount() << std::endl;

    return 0;
}
