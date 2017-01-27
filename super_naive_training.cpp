#include "player.hpp"

#define LOW_THRESHOLD 15
#define NUM_STATES 60
#define VOLATILE 0.005

// this is terrible code
string Training::get_action(vector<string> hole, vector<string> table, vector<string> legal_actions)
{
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
    if(action < cur.checkfold * 100) {
        // 0 = CHECK
        b.actions.push_back(fttoi(0, hs, u, b.current_round));
        return "CHECK";
    }
    if(action < cur.bethigh * 100) {
        int lower = int(double(i2) * 0.3); // can tweak 0.5
        int actual = rand() % (100-lower+1) + lower;

        stringstream ss2;
        ss2 << max(i1, actual);

        // 2 = BETHIGH
        b.actions.push_back(fttoi(2, hs, u, b.current_round));
        return pre + ss2.str();
    }
    else {
        if(i1 > LOW_THRESHOLD) {
            // 0 = FOLD
            b.actions.push_back(fttoi(0, hs, u, b.current_round));
            return "FOLD";
        } else {
            int upper = int(double(i1) * 1.15); // can tweak 1.15
            int actual = rand() % (upper-i1+1) + i1;

            stringstream ss2;
            ss2 << min(i2, min(actual, LOW_THRESHOLD));

            // 1 = BETLOW
            b.actions.push_back(fttoi(1, hs, u, b.current_round));
            return pre + ss2.str();
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
