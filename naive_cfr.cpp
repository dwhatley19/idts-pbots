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
*/
int hand_to_number(vector<string> hand)
{
	sort(hand.begin(), hand.end());
	int rank = 0;

	// check for flush

	// check for straight

	// check for 4-of-a-kind

	// check for 3-of-a-kind

	// check for 2-of-a-kind


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

int hand_strength5(vector<string> hand, bool add)
{
	
}

int hand_strength6(vector<string> hand, bool add)
{
	
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
int usefulness(string hole, string table)
{
	
}

