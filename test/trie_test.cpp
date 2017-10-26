#include "include/trie/Trie.h"
#include <string>
#include <iostream>
#include <vector>
#include <bitset>
#include <tuple>

using namespace std;

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

	TriePath triePath(trie);
	string key = keys[0];
	str_itr keyItr = key.begin(),
	        keyEnd = key.end();
	while (keyItr != keyEnd)
	{
		if (!triePath.update_state(*keyItr))
		{
			cout << "FAILED" << endl;
			break;
		}
		keyItr++;
	}
}

