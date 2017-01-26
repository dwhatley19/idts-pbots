
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "player.hpp"
using namespace std;


Player::Player() {
}

/**
 * Simple example pokerbot, written in C++.
 *
 * This is an example of a bare bones pokerbot. It only sets up the socket
 * necessary to connect with the engine (using a Boost iostream for
 * convenience) and then always returns the same action.  It is meant as an
 * example of how a pokerbot should communicate with the engine.
 */
string Player::getAction() {
  cout << "potSize=" << potSize <<" ";
  cout << "I can do: ";
  for (string s : legalActions) {
    cout << s << ", ";
  }
  cout << endl;
  cout << "(" << holeCard1 << ", " << holeCard2 << "); ";
  for (string s : boardCards) {
    cout << s << " ";
  }
  cout << endl;
  
  int ri = rand() % legalActions.size();

  string ac = legalActions[ri];
  if (ac.find_first_of(':') != ac.find_last_of(':')) {
    ac = ac.substr(0, ac.find_last_of(':'));
  }


  return ac + "\n";
}

void readVec(stringstream& sin, vector<string>& v) {
  int n; sin >> n;
  v.clear();
  for (int i = 0; i < n; i++) {
    string s; sin >> s;
    v.push_back(s);
  }
}
void Player::run(tcp::iostream &stream) {
  string line;
  while (getline(stream, line)) {
    // For now, just print out whatever date is read in.
    //cout << line << "\n";
    stringstream sin(line);
    string packetType; sin >> packetType;
    if (packetType == "NEWGAME") {
      sin >> myName >> oppName >> stackSize >> bb >> numHands >> timeBank;
      
    } else if (packetType == "NEWHAND") {
      sin >> handId >> std::boolalpha >> button >> holeCard1 >> holeCard2 >> myBank >> otherBank >> timeBank;
    } else if (packetType == "GETACTION") {
      // Respond with CHECK when playing, you'll want to change this.
      sin >> potSize;
      readVec(sin, boardCards);
      readVec(sin, lastActions);
      readVec(sin, legalActions);
      sin >> timeBank;
      stream << getAction();
    } else if (packetType == "HANDOVER") {
      sin >> myStack >> oppStack;
      //more stuff goes here
      
    } else if (packetType == "REQUESTKEYVALUES") {
      // FINISH indicates no more keyvalue pairs to store.
      int bytesLeft; sin >> bytesLeft;
      //if you want store key value pairs here
      stream << "FINISH\n";
    }
  }

  cout << "Gameover, engine disconnected.\n";
}
