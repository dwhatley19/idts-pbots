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

    string probs = "0.502 0.957 3.046 0.5 1 0.2 0.5 1 0.2 0.5 1.192 1.001 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.698 1.068 1.867 0.5 1 0.2 0.5 1 0.2 0.5 0.8 0.481 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.696 2.872 5.109 0.858 1.609 1.338 0.5 1.749 1.941 0.5 1 0.2 0.5 1 0.2 0.685 1.951 2.191 0.5 1.871 1.844 0.5 0.801 1.729 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.827 0.686 4.057 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 1.266 0.5 1 0.982 0.5 1 0.2 0.5 1 0.2 0.775 3.504 5.815 0.5 3.028 4.325 0.5 2.029 1.511 0.758 3.975 11.956 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.5 1 0.2 0.498 5.601 7.471 0.5 2.529 2.704 0.5 1.212 0.76";

    stringstream sep(probs);
    double op1, op2, op3;
    int loc_str = 0;
    while(sep >> op1 >> op2 >> op3) t.s[loc_str++] = State(op1, op2, op3);

    std::string line;
    //cout << "Hello\n";

    vector<string> holeCards;
    string myName, oppName;
    double timeBank;
    int stackSize, bb, numHands;
    bool giveup = false;

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
            if(giveup == false && (myBank > 4 * (numHands - handId) || myBank < -8 * numHands - handId)) giveup = true;

            //cout << "\n============== GAME " << handId << " ==============\n";

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
            if(giveup) {
                stream << "CHECK" << endl;
                continue;
            }
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
            //cout << action_taken << endl;
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
