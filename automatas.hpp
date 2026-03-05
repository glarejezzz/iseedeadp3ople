#include <map>
#include <unordered_set>
#include <queue>
#include <deque>

using states_t = std::unordered_set<int>;

struct DFA {
    int initial_state;
    int initial_numm;
    std::unordered_set<int> accept_states;
    std::map<std::pair<int, char>, int> transitions;
};

struct NFA {
    int num_state;
    std::unordered_set<int> accept_states, initial_state;
    std::map<std::pair<int, char>, std::unordered_set<int>> transitions;
    std::map<int, std::unordered_set<int>> epsilon_transitions;
};

class RabinScottConverter {
private: 
states_t epsilon_closure(const NFA& nfa, const states_t &states){
    states_t  result{};
    std::queue<int> q{};
    for (const auto &state : states)
    q.push(state); 
   while(!q.empty())
   {
        int state = q.front(); q.pop();      
        auto it_transitions = nfa.epsilon_transitions.find(state);
        if(it_transitions != nfa.epsilon_transitions.end())
        result.insert(state);
            for(const auto &target_state : it_transitions->second){
                if(result.find(target_state) != result.end()){
                result.insert(target_state);
                q.push(target_state);
            }
        }
    }
    return result;


}
states_t move_by_char(const NFA &nfa, const states_t &states, char letter){
    states_t result{};
    for(const auto &state : states){
        std::pair<int,char> key = {state, letter};
        auto it_trans = nfa.transitions.find(key);
        if(nfa.transitions.find(key) != nfa.transitions.end())
            for (int target_state : it_trans->second)
                result.insert(target_state);
    }
    return result;

}
bool is_contains_accept_state(const NFA &nfa, const states_t &states){
    for(int state : states)
        if(nfa.accept_states.find(state) != nfa.accept_states.end()) return true;
    return false;
}
public:
    DFA operator()(const NFA& nfa, std::unordered_set<char> alphabet) {
        DFA dfa; 
        std::map<states_t, int> state_mapped;
        std::queue<states_t> q{};
        q.push(epsilon_closure(nfa, nfa.initial_state));
    }
}; 