#include "player.hpp"

State s[500];

string Training::get_action(vector<string> hole, vector<string> table, vector<string> all_cards, vector<string> legal_actions)
{
    int hs = b.hand_strength(all_cards);
    int u = b.usefulness(hole, table);
    State cur = s[ttoi(hs, u, b.current_round)];


}

void Training::train()
{
    ofstream fout("probabilities.out");

    
}
