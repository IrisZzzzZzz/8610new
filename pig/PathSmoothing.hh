#pragma once

#include <LayoutEmbedding/Embedding.hh>

namespace LayoutEmbedding
{

/**
 * \brief Perform loop subdivision on the target mesh.
 *
 * This function returns a new Embedding object with the
 * target mesh subdivided using loop subdivision.
 *
 * \param _em_orig     Original embedding
 * \param _n_iters     Number of subdivision iterations (default = 1)
 *
 * \return             New Embedding with subdivided target mesh
 */
Embedding subdivide(
    const Embedding& _em_orig,
    const int _n_iters = 1);

/**
 * \brief Smooth all embedded paths in the embedding using harmonic parametrization.
 *
 * This method uses harmonic mapping to generate smooth paths on the target mesh,
 * following the approach described in [Praun2001].
 *
 * \param _em_orig                 Original embedding
 * \param _n_iters                 Number of harmonic smoothing iterations (default = 1)
 * \param _quad_flap_to_rectangle Whether to convert quad flaps into rectangles (default = true)
 *
 * \return                         New Embedding with smoothed paths
 */
Embedding smooth_paths(
    const Embedding& _em_orig,
    const int _n_iters = 1,
    const bool _quad_flap_to_rectangle = true);

/**
 * \brief Smooth only selected layout edges.
 *
 * This overload allows smoothing of specific layout edges only.
 *
 * \param _em_orig                 Original embedding
 * \param _l_edges                 Vector of layout edges to smooth
 * \param _n_iters                 Number of harmonic smoothing iterations (default = 1)
 * \param _quad_flap_to_rectangle Whether to convert quad flaps into rectangles (default = true)
 *
 * \return                         New Embedding with smoothed paths for selected edges
 */
Embedding smooth_paths(
    const Embedding& _em_orig,
    const std::vector<pm::edge_handle>& _l_edges,
    const int _n_iters = 1,
    const bool _quad_flap_to_rectangle = true);

} // namespace LayoutEmbedding
