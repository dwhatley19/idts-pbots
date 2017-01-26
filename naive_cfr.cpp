#include <string>
#include <vector>
using namespace std;

string cards_to_hand(string a, string b)
{
	string ans = "";
	if(a[1] != b[1]) {
		return ans + a[0] + b[0] + 'o';
	} else {
		return ans + a[0] + b[0] + 's';
	}
}

// returns number between 0 and 12
int number(string card)
{
	int stuff[100];
	stuff['T'] = 8;
	stuff['J'] = 9;
	stuff['Q'] = 10;
	stuff['K'] = 11;
	stuff['A'] = 12;

	return ((card[0] >= '2' && card[0] <= '9') ? card[0] - '2' : stuff[card[0]]);
}

// returns number between 0 and 3
int suit(string card)
{
	int stuff[150];
	stuff['c'] = 0;
	stuff['d'] = 1;
	stuff['h'] = 2;
	stuff['s'] = 3;
	return stuff[card[1]];
}

// returns number between 0 and 51
int rank(string card)
{
	return 4 * number(card) + suit(card);
}

/*
Input: 5-card hand
Output: "position" of the hand in all possible 5-card
	hands
	currently very crude -- outputs number between 0 and 4
	NEED TO IMPROVE
*/
int hand_to_number(vector<string> hand)
{
	// count frequencies of each suit and number
	vector<int> freq_suits(4, 0), freq_nums(13, 0);

	for(int i = 0; i < hand.size(); ++i) {
		freq_suits[suit(hand[i])]++;
		freq_nums[number(hand[i])]++;
	}

	int flush = -1, straight = -1, four = -1, three = -1;
	int two = -1, pairs = 0;

	for(int i = 0; i < 4; ++i) {
		if(freq_suits[i] == 5) flush = i;
	}

	for(int i = 0; i < 13; ++i) {
		if(freq_nums[i] == 4) four = i;
		else if(freq_nums[i] == 3) three = i;
		else if(freq_nums[i] == 2) {
			two = i;
			pairs++;
		}

		if(i <= 8 && freq_nums[i] == 1 && freq_nums[i+1] == 1 && freq_nums[i+2] == 1 && freq_nums[i+3] == 1 && freq_nums[i+4] == 1)
			straight = i+4;
	}

	if(straight >= 0 || flush >= 0 || four >= 0 || three >= 8) return 0;
	if(three >= 0 && two >= 0) return 0;
	if(three >= 0) return 1;
	if(pairs == 2 && two >= 11) return 1;
	if(pairs == 2) return 2;
	if(pairs == 1 && two >= 11) return 2;
	if(pairs == 1) return 3;
	return 4;
}

/*
hand_strength2: determines strength of 2-card hand
hand_strength5: determines strength of 5-card set
hand_strength6: determines strength of 6-card set
hand_strength7: determines strength of 7-card set

vector<string> hand: the input hand/set
	e.g., "2d", "3d", "4d" "5d", "6d"
bool add: used only in sets of sizes 5 and 6 to indicate
	that the function should try all possible combinations
	of one more card

All outputs are integers from 0 to 4 inclusive.
*/
int hand_strength2(vector<string> hand)
{
	
}

int hand_strength4(vector<string> hand)
{

}

int hand_strength5(vector<string> hand, bool add)
{
	string cards = "23456789TJQK", suits = "cdhs";
}

int hand_strength6(vector<string> hand, bool add)
{
	string cards = "23456789TJQK", suits = "cdhs";
	int min = 5;

	for(int i = 0; i < hand.size(); ++i) {
		vector<string> hand2 = hand;
		hand2.erase(hand2.begin() + i);
		if(hand_strength5(hand2, 0));
	}
}

int hand_strength7(vector<string> hand)
{
	
}

/*
Determines usefulness of a given hand.
Only applicable for 2-card hands.

string hole: 2 hole cards
string table: current table cards
*/
int usefulness(vector<string> hole, vector<string> table)
{
	vector<string> table1 = table, table2 = table, table3 = table;
	table1.push_back(hole[0]);
	table2.push_back(hole[1]);
	table3.push_back(hole[0]);
	table3.push_back(hole[1]);

	if(table.size() == 3) {
		int strength1 = hand_strength4(table1);
		int strength2 = hand_strength4(table2);
		int strength3 = hand_strength5(table3, 0);
	} else if(table.size() == 4) {
		int strength1 = hand_strength5(table1, 0);
		int strength2 = hand_strength5(table2, 0);
		int strength3 = hand_strength6(table3, 0);
	} else if(table.size() == 5) {
		int strength1 = hand_strength6(table1, 0);
		int strength2 = hand_strength6(table2, 0);
		int strength3 = hand_strength7(table3);
	}
}

