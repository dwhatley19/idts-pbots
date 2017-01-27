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

    State(): checkfold(1), bethigh(0), betlow(0) { }
};

class Bot {
public:
	int hand_strength(vector<string> hand);
	int usefulness(string hole, string table);

	int current_round;
	vector<int> actions;

	Bot(): current_round(0) {}
};

class Training {
public:
	Bot b;
	string get_action(vector<string> hole, vector<string> table, vector<string> all_cards, vector<string> legal_actions);
	void train();

	// hand strength | usefulness | current round
	// range: 0-4 | 0-2 | 0-3
	int ttoi(int a, int b, int c)
	{ return 12*a + 4*b + c; }

	void itot(int i, int *a, int *b, int *c)
	{
		*a = i/12;
		i %= 12;
		*b = i/4;
		*c = i%4;
	}
};

#endif  // __PLAYER_HPP__
