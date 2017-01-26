#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <boost/asio.hpp>
#include <string>
using boost::asio::ip::tcp;
using namespace std;

class Player {
public:
  Player();
  void run(tcp::iostream &stream);

private:
  string myName;
  string oppName;
  int stackSize;
  int bb;
  int numHands;
  double timeBank;
  int handId;
  bool button;
  string holeCard1;
  string holeCard2;
  int myBank;
  int otherBank;
  string getAction();
  int myStack;
  int oppStack;
  int potSize;
  vector<string> boardCards;
  vector<string> lastActions;
  vector<string> legalActions;
};

#endif  // __PLAYER_HPP__
