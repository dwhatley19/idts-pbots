#include "player.hpp"

Player::Player() {
}


void readVec(stringstream& sin, vector<string>& v) {
  int n; sin >> n;
  v.clear();
  for (int i = 0; i < n; i++) {
    string s; sin >> s;
    v.push_back(s);
  }
}

/**
 * Simple example pokerbot, written in C++.
 *
 * This is an example of a bare bones pokerbot. It only sets up the socket
 * necessary to connect with the engine (using a Boost iostream for
 * convenience) and then always returns the same action.  It is meant as an
 * example of how a pokerbot should communicate with the engine.
 */
void Player::run(tcp::iostream &stream)
{
    Training t;

    /*string probs = "0.1 0.8992 0.8968 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.9 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.8878 0.9 0.9 0.1 0.9 0.1 0.1 0.9 0.1 0.1298 0.8966 0.9 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.6598 0.8826 0.9 0.9 0.9 0.9 0.8992 0.9 0.9 0.1 0.9 0.1 0.1 0.9 0.1 0.8976 0.9 0.9 0.898 0.86 0.9 0.1 0.8986 0.8604 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.8148 0.899 0.8996 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.8992 0.9 0.9 0.8596 0.8738 0.8378 0.1 0.9 0.1 0.1 0.9 0.1 0.8704 0.8804 0.8046 0.8988 0.9 0.9 0.1 0.9 0.8692 0.8984 0.899 0.86 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.1 0.9 0.1 0.886 0.8924 0.9 0.259 0.9 0.803 0.1148 0.2772 0.3426";

    stringstream sep(probs);
    double op1, op2, op3;
    int loc_str = 0;
    while(sep >> op1 >> op2 >> op3) t.s[loc_str++] = State(op1, op2, op3);*/

    std::string line;
    //cout << "Hello\n";

    vector<string> holeCards;
    string myName, oppName;
    double timeBank;
    int stackSize, bb, numHands;

    while (std::getline(stream, line)) {
        // For now, just print out whatever date is read in.
        //std::cout << line << "\n";

        stringstream sin(line);
        string packet_type;
        sin >> packet_type;

        if (packet_type == "NEWGAME") {
            sin >> myName >> oppName >> stackSize >> bb >> numHands >> timeBank;
        } else if(packet_type == "NEWHAND") {
            //cout << "hi\n";
            int handId;
            string button;
            string holeCard1, holeCard2;
            int myBank, otherBank;

            sin >> handId >> button;
            sin >> holeCard1 >> holeCard2;
            sin >> myBank >> otherBank >> timeBank;

            cout << "\n============== GAME " << handId << " ==============\n";

            if(handId % 10 == 0) {
                // every 10th hand, print probabilities
                for(int i = 0; i < 60; ++i) {
                    //fout << i << ": " << t.s[i].checkfold << ' ' << t.s[i].betlow << ' ' << t.s[i].bethigh << '\n';
                }
            }

            holeCards.clear();
            holeCards.push_back(holeCard1);
            holeCards.push_back(holeCard2);
            t.b.current_round = 0;
        } else if(packet_type == "GETACTION") {
            // Respond with CHECK when playing, you'll want to change this.
            vector<string> boardCards, legalActions, lastActions;
            int potSize;

            sin >> potSize;
            readVec(sin, boardCards);
            readVec(sin, lastActions);
            readVec(sin, legalActions);
            sin >> timeBank;

            if(boardCards.size() == 3) t.b.current_round = 1;
            else if(boardCards.size() == 4) t.b.current_round = 2;
            else if(boardCards.size() == 5) t.b.current_round = 3;
            else t.b.current_round = 0;

            string action_taken = t.get_action(holeCards, boardCards, legalActions);
            cout << action_taken << endl;
            stream << action_taken << endl;
        } else if(packet_type == "HANDOVER") {
            int myStack, oppStack;
            sin >> myStack >> oppStack;
            vector<string> boardCards, lastActions;
            readVec(sin, boardCards);
            readVec(sin, lastActions);
            sin >> timeBank;
            string win_name;

            int payoff = 0;
            for(int i = 0; i < lastActions.size(); ++i) {
                if(lastActions[i].size() > 3 && lastActions[i].substr(0, 3) == "WIN") {
                    int sz = lastActions[i].size();
                    while(sz > 0 && lastActions[i][sz] != ':') --sz;
                    win_name = lastActions[i].substr(sz + 1);

                    stringstream ss(lastActions[i].substr(4, sz - 4));
                    ss >> payoff;
                    if(ss.peek() == ':') ss.ignore();
                }
            }

            if(win_name == myName) t.train(payoff);
            else t.train(-payoff);
        } else if (packet_type == "REQUESTKEYVALUES") {
            // FINISh indicates no more keyvalue pairs to store.
            stream << "FINISH\n";
        }
    }

    ofstream fout("probabilities.txt");

    for(int i = 0; i < 60; ++i) {
        fout << t.s[i].checkfold << ' ' << t.s[i].betlow << ' ' << t.s[i].bethigh << ' ';
    }
    fout << endl;

    std::cout << "Gameover, engine disconnected.\n";
}
