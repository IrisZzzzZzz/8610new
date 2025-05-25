#pragma once

#include <LayoutEmbedding/Embedding.hh>
#include <LayoutEmbedding/InsertionSequence.hh>

namespace LayoutEmbedding {

// Configuration options for Branch-and-Bound optimization
struct BranchAndBoundSettings
{
    double optimality_gap = 0.01;                          // Stop when (UB - LB) / UB < gap
    double time_limit = 1 * 60 * 60;                       // Max time in seconds (1 hour); ≤0 means no limit
    bool extend_time_limit_to_ensure_solution = true;      // If true, extend until at least one full solution found

    bool record_upper_bound_events = true;                // Track UB over time
    bool record_lower_bound_events = false;               // Track LB over time

    enum class Priority
    {
        LowerBoundNonConflicting,  // Use: lower_bound * #non_conflicting_paths
        LowerBound                 // Use: lower_bound only
    };
    Priority priority = Priority::LowerBoundNonConflicting;

    bool use_state_hashing = true;                        // Avoid revisiting identical states
    bool use_proactive_pruning = true;                    // Use heuristic-based pruning
    bool use_candidate_paths_for_lower_bounds = true;     // Improve lower bound estimation

    bool print_current_insertion_sequence = true;         // Log sequence at each step
    bool print_memory_footprint_estimate = true;          // Show memory estimate of state tree

    bool use_greedy_init = true;                          // Use greedy embedding to initialize
};

// Result container for Branch-and-Bound run
struct BranchAndBoundResult
{
    BranchAndBoundResult() = default;

    BranchAndBoundResult(const std::string& _algorithm, const BranchAndBoundSettings& _settings)
        : algorithm(_algorithm), settings(_settings) {}

    std::string algorithm;
    BranchAndBoundSettings settings;
    InsertionSequence insertion_sequence;

    double cost = std::numeric_limits<double>::infinity();       // Best solution found (UB)
    double lower_bound = std::numeric_limits<double>::infinity(); // Best lower bound
    double gap = 1.0;                                              // Optimality gap

    struct UpperBoundEvent {
        double t;              // Time
        double upper_bound;    // UB at time t
    };
    std::vector<UpperBoundEvent> upper_bound_events;

    struct LowerBoundEvent {
        double t;              // Time
        double lower_bound;    // LB at time t
    };
    std::vector<LowerBoundEvent> lower_bound_events;

    double max_state_tree_memory_estimate = 0.0; // Peak memory estimate in bytes
    int num_iters = 0;                           // Total iterations (nodes explored)
};

// Core API: Perform BnB optimization on embedding
BranchAndBoundResult branch_and_bound(
    Embedding& _em,
    const BranchAndBoundSettings& _settings = BranchAndBoundSettings(),
    const std::string& _name = "bnb"
);

} // namespace LayoutEmbedding
