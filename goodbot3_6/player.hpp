#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <boost/asio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

using boost::asio::ip::tcp;

class Player {
 public:
  Player();
  void run(tcp::iostream &stream);
};

class State {
public:
    double checkfold;
    double bethigh;
    double betlow;

    State(): checkfold(0.5), bethigh(0.2), betlow(1) { }
    State(double a, double b, double c): checkfold(a), betlow(b), bethigh(c) {}
};

class Bot {
public:
	int hand_strength(vector<string> hand);
	int hand_strength_discard(vector<string> hand);
	int usefulness(vector<string> hole, vector<string> table);

	int current_round;
	vector<int> actions;

	Bot(): current_round(0) {}
};

class Training {
public:
	Bot b;
	string get_action(vector<string> hole, vector<string> table, vector<string> legal_actions);
	void train(int payoff);
	State s[500];

	// hand strength | usefulness | current round
	// range: 0-4 | 0-2 | 0-3
	// tweak these functions as we introduce more states
	int ttoi(int a, int b, int c)
	{ return 12*a + 4*b + c; }

	// d = action we took
	// 0 = checkfold, 1 = betlow, 2 = bethigh
	int fttoi(int d, int a, int b, int c)
	{ return 60*d + 12*a + 4*b + c; }

	Training() { srand(time(NULL)); }
};

#endif  // __PLAYER_HPP__
