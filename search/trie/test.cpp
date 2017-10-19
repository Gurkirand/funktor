#include "Node.h"
#include <string>
#include <iostream>
#include <vector>
#include <bitset>
#include <tuple>

using namespace std;
typedef string::iterator str_itr;

int main()
{
	Trie trie;
	string keys[] = {"Buddy", "Buds", "Burl", "Amadeus", "Amazing", "Mozart", "Mozzie"};
	string value = "";
	for (int i = 0; i <  sizeof(keys) / sizeof(keys[0]); i++)
	{
		trie.insert(keys[i], value);
	}
	trie.print();
}

