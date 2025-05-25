#pragma once

#include <LayoutEmbedding/Embedding.hh>
#include <LayoutEmbedding/InsertionSequence.hh>

namespace LayoutEmbedding {

// Settings for controlling greedy embedding behavior
struct GreedySettings
{
    enum class InsertionOrder
    {
        BestFirst,  // Embed best-fitting edge first (default)
        Arbitrary   // Embed edges in arbitrary order
    };
    InsertionOrder insertion_order = InsertionOrder::BestFirst;

    // Whether to penalize "swirled" paths to improve layout quality
    bool use_swirl_detection = false;
    double swirl_penalty_factor = 2.0;

    // Enable tracing paths using harmonic fields repelling layout vertices
    bool use_vertex_repulsive_tracing = false;

    // Use the edge blocking rule to avoid overlapping paths
    bool use_blocking_condition = false;

    // Prefer starting with extremal vertices (farthest in local neighborhood)
    bool prefer_extremal_vertices = false;
    double extremal_vertex_ratio = 0.25;
};

// Result from a greedy embedding algorithm
struct GreedyResult
{
    GreedyResult() = default;

    GreedyResult(const std::string& _algorithm, const GreedySettings& _settings)
        : algorithm(_algorithm), settings(_settings) {}

    std::string algorithm;             // Algorithm name
    GreedySettings settings;           // Settings used
    InsertionSequence insertion_sequence;
    double cost = std::numeric_limits<double>::infinity();
};

// Apply a single greedy embedding method
GreedyResult embed_greedy(
    Embedding& _em,
    const GreedySettings& _settings = GreedySettings(),
    const std::string& _name = "greedy");

// Specific variants of greedy embedding
GreedyResult embed_praun(
    Embedding& _em,
    const GreedySettings& _settings = GreedySettings());

GreedyResult embed_kraevoy(
    Embedding& _em,
    const GreedySettings& _settings = GreedySettings());

GreedyResult embed_schreiner(
    Embedding& _em,
    const GreedySettings& _settings = GreedySettings());

// Run multiple variants of greedy embedding and return all results
std::vector<GreedyResult> embed_greedy(
    Embedding& _em,
    const std::vector<GreedySettings>& _all_settings);

// Run a standard set of known greedy methods as competitors
std::vector<GreedyResult> embed_competitors(
    Embedding& _em,
    const GreedySettings& _settings = GreedySettings());

// Utility to retrieve the best result
const GreedyResult& best(const std::vector<GreedyResult>& _results);
const GreedyResult& best(const std::vector<GreedyResult>& _results, int& best_idx);

} // namespace LayoutEmbedding
