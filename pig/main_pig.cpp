#include "LayoutEmbedding/EmbeddingInput.hh"
#include "LayoutEmbedding/Embedding.hh"
#include "LayoutEmbedding/Greedy.hh"
#include "LayoutEmbedding/BranchAndBound.hh"
#include "LayoutEmbedding/PathSmoothing.hh"

// 只有在需要可视化时才包含 GLFW
#ifdef USE_VISUALIZATION
#include <glow-extras/glfw/GlfwContext.hh>
#endif

#include <filesystem>
#include <iostream>
#include <exception>

using namespace LayoutEmbedding;
namespace fs = std::filesystem;

int main()
{
    try {
        std::cout << "Starting layout embedding process..." << std::endl;

#ifdef USE_VISUALIZATION
        // 只有在需要可视化时才初始化 GLFW 上下文
        glow::glfw::GlfwContext ctx;
        std::cout << "GLFW context initialized." << std::endl;
#endif

        // 1. 检查数据路径是否存在
        fs::path data_path(LE_DATA_PATH);
        fs::path layout_path = data_path / "models/layouts/teaser_pig.obj";
        fs::path target_path = data_path / "models/target-meshes/pig/pig_union.obj";
        
        if (!fs::exists(layout_path)) {
            std::cerr << "Layout file not found: " << layout_path << std::endl;
            return -1;
        }
        
        if (!fs::exists(target_path)) {
            std::cerr << "Target mesh file not found: " << target_path << std::endl;
            return -1;
        }

        std::cout << "Loading input files..." << std::endl;
        std::cout << "Layout: " << layout_path << std::endl;
        std::cout << "Target: " << target_path << std::endl;

        // 2. 读取 layout 和目标 mesh
        EmbeddingInput input;
        input.load(layout_path, target_path);
        
        std::cout << "Input loaded successfully." << std::endl;

        // 3. 只进行 Branch and Bound 优化
        std::cout << "Creating embedding..." << std::endl;
        Embedding em(input);
        
        std::cout << "Running branch and bound optimization (without greedy init)..." << std::endl;
        BranchAndBoundSettings settings;
        settings.use_greedy_init = false;  // 不使用贪心算法初始化
        settings.time_limit = 60.0;        // 60秒时间限制
        branch_and_bound(em, settings);

        // 4. 平滑路径
        std::cout << "Smoothing paths..." << std::endl;
        em = smooth_paths(em);

        // 5. 保存结果
        std::cout << "Saving results..." << std::endl;
        fs::create_directories("output");
        em.save("output/pig_embedding");
        
        std::cout << "Process completed successfully!" << std::endl;
        std::cout << "Results saved to: output/pig_embedding" << std::endl;
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        return -1;
    }
}