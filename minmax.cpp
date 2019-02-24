#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <numeric>
#include <limits>
#include <functional>

using Graph = std::vector<std::vector<int>>;

enum class Move {
    Mouse,
    Cat,
};

struct State {
    int mouse;
    int cat;
    Move move;

    bool is_terminal() const noexcept
    {
        return (mouse == 0) || (mouse == cat);
    }

    int value() const noexcept
    {
        if (mouse == 0) {
            return 1;
        }
        if (mouse == cat) {
            return -1;
        }

        std::abort();
    }
};

bool operator==(const State& lhs, const State& rhs)
{
    return (lhs.mouse == rhs.mouse) && (lhs.cat == rhs.cat) && (lhs.move == rhs.move);
};

std::ostream& operator<<(std::ostream& os, const State& state)
{
    return os
        << state.mouse << " " << state.cat << " " << static_cast<int>(state.move);
}

namespace std {
    template<>
    struct hash<State> {
        using argument_type = State;
        using result_type = std::size_t;

        result_type operator()(const argument_type& s) const noexcept
        {
            const result_type h1 (std::hash<int>{}(s.mouse));
            const result_type h2 (std::hash<int>{}(s.cat));
            return h1 ^ (h2 << 1);
        }
    };
};

std::unordered_set<State> states;
std::unordered_map<State, int> transposition;

int min_value(State state, const Graph& graph);

int max_value(State state, const Graph& graph)
{
    auto it = transposition.find(state);
    if (it != transposition.end()) {
        return it->second;
    }

    if (state.is_terminal()) {
        return state.value();
    }

    auto kv = states.insert(state);
    if (!kv.second) {
        return 0;
    }

    int value = std::numeric_limits<int>::min();
    for (int a : graph[state.mouse]) {
        value = std::max<int>(
            value,
            min_value(State{a, state.cat, Move::Cat}, graph)
        );

    }

    states.erase(state);
    transposition[state] = value;

    return value;
}

int min_value(State state, const Graph& graph)
{
    auto it = transposition.find(state);
    if (it != transposition.end()) {
        return it->second;
    }

    if (state.is_terminal()) {
        return state.value();
    }

    auto kv = states.insert(state);
    if (!kv.second) {
        return 0;
    }

    int value = std::numeric_limits<int>::max();
    for (int a : graph[state.cat]) {

        if (a == 0) {
            continue;
        }

        value = std::min<int>(
            value,
            max_value(State{state.mouse, a, Move::Mouse}, graph)
        );
    }

    states.erase(state);
    transposition[state] = value;

    return value;
}

int play(State state, const Graph& graph)
{
    const int r = max_value(state, graph);
    switch(r) {
    case 0:
        return 0;
    case 1:
        return 1;
    case -1:
        return 2;
    }
    std::abort();
}

int main()
{
    // 1. draw
    //std::vector<std::vector<int>> graph = {{2,5},{3},{0,4,5},{1,4,5},{2,3},{0,2,3}};
    // 2. mouse
    // std::vector<std::vector<int>> graph = {{1},{0,2},{1}};
    // 3. cat
    // std::vector<std::vector<int>> graph = {{2},{2},{0,1}};
    // 4. mouse
    std::vector<std::vector<int>> graph = {{6},{4},{9},{5},{1,5},{3,4,6},{0,5,10},{8,9,10},{7},{2,7},{6,7}};
    // 5.
    //std::vector<std::vector<int>> graph = {
    //    {2,4,7,8},{2,3,4,5,7,8,9},{0,1,3,4,5,7,8,9},{1,2,5},{0,1,2,5,6,7,9},{1,2,3,4,6,7},{4,5,8,9},{0,1,2,4,5,9},{0,1,2,6,9},{1,2,4,6,7,8}
    //};
    // 6.
    //std::vector<std::vector<int>> graph = {
    //    {2,9,14},{2,5,7},{0,1,3,8,9,11,14},{2,12},{5,11,18},{1,4,18},{9,17,19},{1,11,12,13,14,17,19},{2,16,17},{0,2,6,12,14,18},{14},{2,4,7},{3,7,9,13},{7,12,14},{0,2,7,9,10,13,17},{18},{8,19},{6,7,8,14,19},{4,5,9,15},{6,7,16,17}
    //};

    const int r = play(State{1, 2, Move::Mouse}, graph);
    std::cout << r << std::endl;

    return 0;
}