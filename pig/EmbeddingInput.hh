#pragma once

#include <LayoutEmbedding/Util/Assert.hh>
#include <polymesh/pm.hh>
#include <typed-geometry/tg.hh>

#include <filesystem>
#include <fstream>
#include <sstream>

namespace LayoutEmbedding {

namespace fs = std::filesystem;

/// 可接受的 landmark 格式
enum class LandmarkFormat {
    id_x_y_z,  ///< 包含顶点 ID 和 3D 坐标
    id         ///< 仅包含顶点 ID
};

/// 表示一个 layout embedding 输入，包括 layout mesh、target mesh 及匹配关系
struct EmbeddingInput {
    // 网格数据
    pm::Mesh l_m;  ///< layout mesh
    pm::Mesh t_m;  ///< target mesh

    // Layout 顶点属性
    pm::vertex_attribute<tg::pos3> l_pos;                  ///< layout 顶点坐标
    pm::vertex_attribute<pm::vertex_handle> l_matching_vertex;  ///< layout 顶点匹配的 target 顶点

    // Target 顶点属性
    pm::vertex_attribute<tg::pos3> t_pos;  ///< target 顶点坐标

    // 构造函数
    EmbeddingInput();
    EmbeddingInput(const EmbeddingInput& other);
    EmbeddingInput& operator=(const EmbeddingInput& other);

    /// 保存输入到 `.inp` 文件（可选同时保存 layout/target mesh 的 obj）
    /// @param filename 基础文件名（不含后缀）
    /// @param write_layout_mesh 是否写入 layout obj
    /// @param write_target_input_mesh 是否写入 target obj
    bool save(const std::string& filename,
              bool write_layout_mesh = true,
              bool write_target_input_mesh = true) const;

    /// 从 `.inp` 文件中加载 layout/target 路径和匹配信息
    bool load(const std::string& _path_prefix);

    /// 直接从 layout、target 文件路径加载
    bool load(const fs::path& _layout_path, const fs::path& _target_path);

    /// 同时加载匹配 landmarks
    bool load(const fs::path& _layout_path,
              const fs::path& _target_path,
              const fs::path& _landmarks_path,
              const LandmarkFormat& _format);

    /// 归一化 target 表面面积到 1.0
    void normalize_surface_area();

    /// 将 target mesh 移动到原点中心
    void center_translation();

    /// 翻转 layout mesh 的面方向
    /// 注意：该操作会重排边、半边索引，清除 edge/face 属性
    void invert_layout();
};

/// 加载 landmark 列表（用于顶点匹配）
/// @param _file_path landmark 文件路径
/// @param _format 格式（id / id + 坐标）
std::vector<int> load_landmarks(const fs::path& _file_path,
                                const LandmarkFormat& _format);

} // namespace LayoutEmbedding
