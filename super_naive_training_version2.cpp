#include "player.hpp"

#define LOW_THRESHOLD 15
#define NUM_STATES 60
#define VOLATILE 0.005

int rounds_in_preflop;
bool discard_flop, discard_turn;
bool cantfold;

// this is terrible code
string Training::get_action(vector<string> hole, vector<string> table, vector<string> legal_actions)
{
    // check if we are at preflop
    if(table.size() != 0) preflop_cnt = 0;
    else preflop_cnt++;

    // updating allowed_to_discard
    if(preflop_cnt = 1) { // beggining of a new game, we can discard (for now) both at the flop and at the turn, and we can also fold
        discsard_flop = discard_turn = true; 
        cantfold = false;
    }
    
    vector<string> all_cards = table;
    all_cards.push_back(hole[0]);
    all_cards.push_back(hole[1]);

    int hs = b.hand_strength(all_cards);
    int u = b.usefulness(hole, table);
    State cur = s[ttoi(hs, u, b.current_round)];

    string bet_string = "X";
    // find string that says "BET:min:max" or "RAISE:min:max"
    for(int i = 0; i < legal_actions.size(); ++i) {
        if(legal_actions[i][0] == 'B' || legal_actions[i][0] == 'R') {
            bet_string = legal_actions[i];
            break;
        }
    }

    string pre = ""; // will either say "BET:" or "RAISE:"
    if(bet_string[0] == 'B') {
        pre = bet_string.substr(0, 4);
        bet_string = bet_string.substr(4);
    }
    else if(bet_string[0] == 'R') {
        pre = bet_string.substr(0, 6);
        bet_string = bet_string.substr(6);
    }
    else {
        // 0 = CHECK
        b.actions.push_back(fttoi(0, hs, u, b.current_round));
        return "CHECK";
    }

    stringstream ss(bet_string);
    int i1, i2;
    ss >> i1;
    if(ss.peek() == ':') ss.ignore();
    ss >> i2;

    // ==============================
    int action = rand() % 100; // 100 is just any number
    int bluffing = 0, check = 0, discard = 0;
    
    for(int i = 0; i < legal_actions.size(); ++i) {
        if(legal_actions[i][0] == 'D') discard = 1;
        if(legal_actions[i][0] == 'C') check = 1;
    }
    
    // assuming that when we are able to discard our only options are DISCARD/CHECK
    if(discard) {
        if(table.size() == 3) {
            if(!discard_flop) return "CHECK";
            else {
                if(u == 0) return "CHECK";
                else if(u == 1 && rand() % 2 == 0) { // discarding with probability 50% (very naive)
                    vector<string> one = table, two = table; // one is the table with card 0, two is the table with card 1 
                    one.push_back(hole[0]);
                    two.push_back(hole[1]);
                    int sone = hand_strength4(one);
                    int stwo = hand_strength4(two);
                    if(sone == stwo) {
                        if(number(hole[0]) > number(hole[1])) return "DISCARD:" + hole[1];
                        else if(number(hole[0]) < number(hole[1])) return "DISCARD:" + hole[0];
                        else return "CHECK"; // dont discard; might need to change that
                    } 
                    else if(sone > stwo) return "DISCARD:" + hole[1];
                    else return "DISCARD:" + hole[0];
                } else if(u == 2) {
                    vector<string> one = table, two = table; // one is the table with card 0, two is the table with card 1 
                    one.push_back(hole[0]);
                    two.push_back(hole[1]);
                    int sone = hand_strength4(one);
                    int stwo = hand_strength4(two);
                    if(sone == stwo) {
                        if(number(hole[0]) > number(hole[1])) return "DISCARD:" + hole[1];
                        else return "DISCARD:" + hole[0];
                        // always discard at usefulness 2
                        // else if(number(hole[0]) < number(hole[1])) return /* DISCARD card 0 */;
                        // else return "CHECK"; // dont discard; might need to change that
                    } 
                    else if(sone > stwo) return "DISCARD:" + hole[1];
                    else return "DISCARD:" + hole[0];
                }
            }
        } // if(table.size() == 3)
        else {
            if(!discard_turn) return "CHECK";
            else {
                if(u == 0) return "CHECK";
                else if(u == 1 && rand() % 2 == 0) { // discarding with probability 50% (very naive)
                    vector<string> one = table, two = table; // one is the table with card 0, two is the table with card 1 
                    one.push_back(hole[0]);
                    two.push_back(hole[1]);
                    int sone = hand_strength5(one, 0);
                    int stwo = hand_strength5(two, 0);
                    if(sone == stwo) {
                        if(number(hole[0]) > number(hole[1])) return "DISCARD:" + hole[1];
                        else if(number(hole[0]) < number(hole[1])) return "DISCARD:" + hole[0];
                        else return "CHECK"; // dont discard; might need to change that
                    } 
                    else if(sone > stwo) return "DISCARD:" + hole[1];
                    else return "DISCARD:" + hole[0];
                } else if(u == 2) {
                    vector<string> one = table, two = table; // one is the table with card 0, two is the table with card 1 
                    one.push_back(hole[0]);
                    two.push_back(hole[1]);
                    int sone = hand_strength5(one, 0);
                    int stwo = hand_strength5(two, 0);
                    if(sone == stwo) {
                        if(number(hole[0]) > number(hole[1])) return "DISCARD:" + hole[1];
                        else return "DISCARD:" + hole[0];
                        // always discard at usefulness 2
                        // else if(number(hole[0]) < number(hole[1])) return /* DISCARD card 0 */;
                        // else return "CHECK"; // dont discard; might need to change that
                    } 
                    else if(sone > stwo) return "DISCARD:" + hole[1];
                    else return "DISCARD:" + hole[0];
                }
            }
        }
    } // if(discard)
    else {
        if(action >= cur.checkfold * 100 && action < cur.bethigh * 100) { // we are betting high
            cantfold = true; // we just bet high; folding is no longer an option
            if(table.empty()) discard_flop = discard_turn = false; // we started betting high on preflop, so we are pretending we have a high pair; discarding would give us away
            else discard_flop = false; // we cannot discard in flop, we can potentially discard in turn
        }
        
        if(action < cur.checkfold * 100) {
            if(check) {
                // 0 = CHECK
                b.actions.push_back(fttoi(0, hs, u, b.current_round));
                return "CHECK";
            } else if(!cantfold) return "FOLD";
        }
        if(action < cur.bethigh * 100) {
            cantfold = true; // we just bet high; folding is no longer an option
            int lower = int(double(i2) * 0.3); // can tweak 0.5
            int actual = rand() % (100-lower+1) + lower;
    
            stringstream ss2;
            ss2 << max(i1, actual);
    
            // 2 = BETHIGH
            b.actions.push_back(fttoi(2, hs, u, b.current_round));
            return pre + ss2.str();
        }
        else {
            if(i1 > LOW_THRESHOLD && !cantfold) { // <------- maybe we should change that, as a safety measure in case the opponent goes all-in
                // 0 = FOLD
                b.actions.push_back(fttoi(0, hs, u, b.current_round));
                return "FOLD";
            } else {
                int call_vs_raise = rand() % 10;


                if(pre == "BET:") {
                    int upper = int(double(i1) * 10); // can tweak 10 -- this just says we're betting between 2 and 20
                    int actual = rand() % (upper-i1+1) + i1;
        
                    stringstream ss2;
                    ss2 << min(i2, min(actual, LOW_THRESHOLD));
        
                    // 1 = BETLOW
                    b.actions.push_back(fttoi(1, hs, u, b.current_round));
                    return pre + ss2.str();
                } else if(pre == "RAISE:" && call_vs_raise >= 8) {
                    int upper = int(double(i1) * 1.5); // can tweak 1.5
                    int actual = rand() % (upper-i1+1) + i1;
        
                    stringstream ss2;
                    ss2 << min(i2, min(actual, LOW_THRESHOLD));
        
                    // 1 = BETLOW
                    b.actions.push_back(fttoi(1, hs, u, b.current_round));
                    return pre + ss2.str();
                } else return "CALL";
            }
        }
    }  
}

// HAVE NOT INCORPORATED VOLATILITY
void Training::train(int payoff)
{
    ofstream fout("probabilities.out");

    // every action in b.actions was some degree of "correctness"
    for(int i = 0; i < b.actions.size(); ++i) {
        int three_tuple = b.actions[i] % NUM_STATES;
        int action = b.actions[i] / NUM_STATES;

        // might need a separate function to deal with this as it gets more complicated
        // or a new design -- maybe keep checkfold/betlow/bethigh in an array
        // i know i was sloppy :(((
        if(action == 0) {
            s[three_tuple].checkfold += VOLATILE * payoff;
            //s[three_tuple].betlow -= VOLATILE * payoff / 2;
            //s[three_tuple].bethigh -= VOLATILE * payoff / 2;
        }
        else if(action == 1) {
            //s[three_tuple].checkfold -= VOLATILE * payoff / 2;
            s[three_tuple].betlow += VOLATILE * payoff;
            //s[three_tuple].bethigh -= VOLATILE * payoff / 2;
        }
        else if(action == 2) {
            //s[three_tuple].checkfold -= VOLATILE * payoff / 2;
            //s[three_tuple].betlow -= VOLATILE * payoff / 2;
            s[three_tuple].bethigh += VOLATILE * payoff;
        }
    }
    for(int i = 0; i < NUM_STATES; ++i) {
        s[i].checkfold = max(0.1, min(0.9, s[i].checkfold));
        s[i].betlow = max(0.1, min(0.9, s[i].betlow));
        s[i].bethigh = max(0.1, min(0.9, s[i].bethigh));
    }
}
