#ifndef SEARCH_ALGORITHMS_LAZY_SEARCH_H
#define SEARCH_ALGORITHMS_LAZY_SEARCH_H

#include "../evaluation_context.h"
#include "../evaluator.h"
#include "../open_list.h"
#include "../operator_id.h"
#include "../search_algorithm.h"
#include "../search_progress.h"
#include "../search_space.h"

#include "../utils/rng.h"



// PR2: Override the successor generator to use the PR2 deadend aware one.
#include "../pr2/fd_integration/pr2_search_algorithm.h"



#include <memory>
#include <vector>

class OpenListFactory;

namespace lazy_search {
class LazySearch : public SearchAlgorithm {
protected:
    std::unique_ptr<EdgeOpenList> open_list;

    // Search behavior parameters
    bool reopen_closed_nodes; // whether to reopen closed nodes upon finding lower g paths
    bool randomize_successors;
    bool preferred_successors_first;
    std::shared_ptr<utils::RandomNumberGenerator> rng;

    std::vector<Evaluator *> path_dependent_evaluators;
    std::vector<std::shared_ptr<Evaluator>> preferred_operator_evaluators;

    State current_state;
    StateID current_predecessor_id;
    OperatorID current_operator_id;
    int current_g;
    int current_real_g;
    EvaluationContext current_eval_context;

    int state_count; // Used to limit the search when replanning for a local goal
    bool was_initialized;

    virtual void initialize() override;
    virtual SearchStatus step() override;

    void generate_successors();
    SearchStatus fetch_next_state();

    void reward_progress();

    std::vector<OperatorID> get_successor_operators(
        const ordered_set::OrderedSet<OperatorID> &preferred_operators) const;
    


    // PR2: Override the successor generator to use the PR2 deadend aware one.
    DeadendAwareSuccessorGenerator *pr2_successor_generator;



public:
    LazySearch(
        const std::shared_ptr<OpenListFactory> &open,
        bool reopen_closed,
        const std::vector<std::shared_ptr<Evaluator>> &evaluators,
        bool randomize_successors, bool preferred_successors_first,
        int random_seed, OperatorCost cost_type, int bound,
        double max_time, const std::string &description,
        utils::Verbosity verbosity);
    



    // PR2: Modified to allow for specific task
    LazySearch(
        const std::shared_ptr<OpenListFactory> &open,
        bool reopen_closed,
        const std::vector<std::shared_ptr<Evaluator>> &evaluators,
        bool randomize_successors, bool preferred_successors_first,
        int random_seed, OperatorCost cost_type, int bound,
        double max_time, const std::string &description,
        utils::Verbosity verbosity,
        const std::shared_ptr<AbstractTask> &newtask,
        DeadendAwareSuccessorGenerator *pr2_successor_generator);


    


    virtual void print_statistics() const override;
};
}

#endif
