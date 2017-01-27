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
    while (std::getline(stream, line)) {
        // For now, just print out whatever date is read in.
        //std::cout << line << "\n";

        stringstream sin(line);
        string packet_type;
        sin >> packet_type;

        vector<string> holeCards;
        string myName;

        if (packet_type == "NEWGAME") {
            sin >> myName >> oppName >> stackSize >> bb >> numHands >> timeBank;
        } else if(packet_type == "NEWHAND") {
            sin >> handId >> std::boolalpha >> button >> holeCard1 >> holeCard2 >> myBank >> otherBank >> timeBank;
            holeCards.clear();
            holeCards.push_back(holeCard1);
            holeCards.push_back(holeCard2);
        } else if(packet_type == "GETACTION") {
            // Respond with CHECK when playing, you'll want to change this.
            sin >> potSize;
            readVec(sin, boardCards);
            readVec(sin, lastActions);
            readVec(sin, legalActions);
            sin >> timeBank;

            stream << t.get_action(holeCards, boardCards, legalActions);
        } else if(packet_type == "HANDOVER") {
            sin >> myStack >> oppStack;
            vector<string> boardCards;
            vector<string> lastActions;
            readVec(sin, boardCards);
            readVec(sin, lastActions);
            sin >> timeBank;

            for(int i = 0; i < lastActions.size(); ++i) {
                if(lastActions[i].size() > 3 && lastActions[i].substring(0, 3) == "WIN") {
                    stringstream ss;
                    
                }
            }

            int payoff = //however much we won
            t.train(payoff);
        } else if (packet_type == "REQUESTKEYVALUES") {
            // FINISh indicates no more keyvalue pairs to store.
            stream << "FINISH\n";
        }
    }

    std::cout << "Gameover, engine disconnected.\n";
}
