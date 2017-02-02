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

    /*string probs = "-0.0895 1.45536 2.46358 0 0 1 0 0 1 0 0.46984 1.56384 0.3 0.3 0.6 0 0 1 0 0 1 0 0 1 0.3 0.3 0.6 0 0 1 0 0 1 0 0 1 -0.30392 1.46298 4.76622 0.3 0.3 0.6 0.3 0.3 0.6 0.13542 0.36102 1.18944 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 -0.50456 0.88724 7.94284 2.43698 0.57638 0.81976 5.30424 0.24164 1.53498 0.3 0.3 0.6 0.3 0.3 0.6 1.33696 0.76876 1.47548 2.54634 0.66398 0.60366 -0.09134 0.539 0.69676 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 -0.16766 0.26242 15.9708 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 1.47786 0.58278 1.30266 0.59932 0.5278 0.88866 0.3 0.3 0.6 0.3 0.3 0.6 4.15482 1.38068 5.79582 1.01992 1.33654 3.07014 0.22364 0.81698 1.15296 -1.34846 1.91178 36.8389 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 0.3 0.3 0.6 -0.92464 0.76318 8.0494 -10.5008 0.7946 1.13422 0.0024 0.20242 0.63906";

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

            //if(win_name == myName) t.train(payoff);
            //else t.train(-payoff);
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
