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

    std::cout << "Gameover, engine disconnected.\n";
}
