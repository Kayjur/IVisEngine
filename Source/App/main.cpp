#include <iostream>
#include <thread>
#include <chrono>
#include "FrameworkThread.h"
#include "RendererThread.h"

int main() {
    std::cout << "=== IVisEngine 启动 ===" << std::endl;

    // 创建Framework线程
    ivis::framework::FFrameworkThread FrameworkThread;
    
    // 创建Renderer线程
    ivis::renderer::FRendererThread RendererThread;

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

    std::cout << "两个线程已启动，主线程等待中..." << std::endl;
    std::cout << "按Enter键停止所有线程..." << std::endl;

    // 主线程等待用户输入
    std::cin.get();

    // 停止线程
    std::cout << "正在停止线程..." << std::endl;
    FrameworkThread.Stop();
    RendererThread.Stop();

    // 等待线程结束
    FrameworkThread.WaitForCompletion(5000);
    RendererThread.WaitForCompletion(5000);

    std::cout << "=== IVisEngine 关闭 ===" << std::endl;
    std::cout << "Framework线程总帧数: " << FrameworkThread.GetFrameCount() << std::endl;
    std::cout << "Renderer线程总渲染帧数: " << RendererThread.GetRenderFrameCount() << std::endl;

    return 0;
}
