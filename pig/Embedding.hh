#pragma once

#include <LayoutEmbedding/EmbeddingInput.hh>
#include <LayoutEmbedding/LayoutGeneration.hh>
#include <LayoutEmbedding/VirtualVertex.hh>
#include <LayoutEmbedding/VirtualPath.hh>

#include <polymesh/formats/obj.hh>
#include <Eigen/Dense>
#include <optional>

namespace LayoutEmbedding {

struct Snake;

/// 表示一次嵌入状态，包括 layout mesh 与 target mesh 的映射关系
class Embedding {
public:
    explicit Embedding(EmbeddingInput& _input);
    Embedding(const Embedding& _em);
    Embedding& operator=(const Embedding& _em);

    /// 获取某 layout halfedge 在 target 上对应路径起点的 halfedge
    pm::halfedge_handle get_embedded_target_halfedge(const pm::halfedge_handle& _l_he) const;

    /// 判断某 layout 边或半边是否已经嵌入
    bool is_embedded(const pm::halfedge_handle& _l_he) const;
    bool is_embedded(const pm::edge_handle& _l_e) const;
    bool is_embedded(const pm::edge_index& _l_e) const;

    /// 获取 layout halfedge 可嵌入的目标区域（sector）
    pm::halfedge_handle get_embeddable_sector(const pm::halfedge_handle& _l_he) const;

    /// 判断某 target 元素是否被占用
    bool is_blocked(const pm::edge_handle& _t_e) const;
    bool is_blocked(const pm::vertex_handle& _t_v) const;
    bool is_blocked(const VirtualVertex& _t_vv) const;

    /// 获取 target 元素的位置
    tg::pos3 element_pos(const pm::edge_handle& _t_e) const;
    tg::pos3 element_pos(const pm::vertex_handle& _t_v) const;
    tg::pos3 element_pos(const VirtualVertex& _t_vv) const;

    enum class ShortestPathMetric {
        Geodesic,
        VertexRepulsive,
    };

    /// 路径计算接口（layout 到 target）
    VirtualPath find_shortest_path(const pm::halfedge_handle& _t_h_start,
                                   const pm::halfedge_handle& _t_h_end,
                                   ShortestPathMetric _metric = ShortestPathMetric::Geodesic) const;

    VirtualPath find_shortest_path(const pm::halfedge_handle& _l_he,
                                   ShortestPathMetric _metric = ShortestPathMetric::Geodesic) const;

    VirtualPath find_shortest_path(const pm::edge_handle& _l_e,
                                   ShortestPathMetric _metric = ShortestPathMetric::Geodesic) const;

    double path_length(const VirtualPath& _path) const;

    /// 执行嵌入 / 撤销嵌入
    void embed_path(const pm::halfedge_handle& _l_he, const VirtualPath& _path);
    void embed_path(const pm::halfedge_handle& _l_he, const Snake& _snake);
    void unembed_path(const pm::halfedge_handle& _l_he);
    void unembed_path(const pm::edge_handle& _l_e);

    /// 获取 layout halfedge 对应的嵌入路径
    std::vector<pm::vertex_handle> get_embedded_path(const pm::halfedge_handle& _l_he) const;
    std::vector<pm::face_handle> get_patch(const pm::face_handle& _l_f) const;

    double embedded_path_length(const pm::halfedge_handle& _l_he) const;
    double embedded_path_length(const pm::edge_handle& _l_e) const;
    double total_embedded_path_length() const;
    bool is_complete() const;

    /// 序列化接口
    bool save(std::string filename, bool write_target_mesh = true,
              bool write_layout_mesh = true, bool write_target_input_mesh = true) const;
    bool load(std::string filename);

    // === Getters === //
    const pm::Mesh& layout_mesh() const;
    pm::Mesh& layout_mesh();
    const pm::vertex_attribute<tg::pos3>& layout_pos() const;
    pm::vertex_attribute<tg::pos3>& layout_pos();

    const pm::Mesh& target_mesh() const;
    pm::Mesh& target_mesh();
    const pm::vertex_attribute<tg::pos3>& target_pos() const;
    pm::vertex_attribute<tg::pos3>& target_pos();

    const pm::vertex_handle& matching_target_vertex(const pm::vertex_handle& _l_v) const;
    pm::vertex_handle& matching_target_vertex(const pm::vertex_handle& _l_v);

    const pm::vertex_handle& matching_layout_vertex(const pm::vertex_handle& _t_v) const;
    pm::vertex_handle& matching_layout_vertex(const pm::vertex_handle& _t_v);

    const pm::halfedge_handle& matching_layout_halfedge(const pm::halfedge_handle& _t_h) const;
    pm::halfedge_handle& matching_layout_halfedge(const pm::halfedge_handle& _t_h);

    /// 顶点排斥项（用于路径避障）
    double get_vertex_repulsive_energy(const pm::vertex_handle& _t_v,
                                       const pm::vertex_handle& _l_v) const;

    double get_vertex_repulsive_energy(const VirtualVertex& _t_vv,
                                       const pm::vertex_handle& _l_v) const;

private:
    EmbeddingInput* input;
    pm::Mesh t_m; ///< 拷贝后的 target mesh
    pm::vertex_attribute<tg::pos3> t_pos;

    pm::vertex_attribute<pm::vertex_handle> l_matching_vertex;
    pm::vertex_attribute<pm::vertex_handle> t_matching_vertex;
    pm::halfedge_attribute<pm::halfedge_handle> t_matching_halfedge;

    // 用于 vertex repulsion 路径优化
    mutable std::optional<pm::vertex_attribute<Eigen::VectorXd>> vertex_repulsive_energy;
};

} // namespace LayoutEmbedding
