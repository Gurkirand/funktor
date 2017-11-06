#include "../include/trie/trie.h"
#include "../src/util/util.h"
#include "data/load.h"
#include <string>
#include <iostream>
#include <vector>
#include <bitset>
#include <tuple>
#include <random>

using namespace std;

/* vector<string> split(string s) */
/* { */
/* 	int pos; */
/* 	vector<string> splitS; */
/* 	while ((pos = s.find(' ')) != string::npos) */
/* 	{ */
/* 		splitS.push_back(s.substr(0, pos)); */
/* 		s.erase(0, pos + 1); */
/* 	} */
/* 	return splitS; */
/* } */

/* vector<string> load_data() */
/* { */
/* 	vector<string> data; */
/* 	vector<string> splitString; */
/* 	string line; */
/* 	ifstream dataFile("test/data/composers_small.txt"); */
/* 	if (dataFile.is_open()) */
/* 	{ */
/* 		while (getline(dataFile, line)) */
/* 		{ */
/* 			splitString = split(line); */
/* 			data.insert(data.end(), splitString.begin(), splitString.end()); */
/* 		} */
/* 		dataFile.close(); */
/* 	} */
/* 	return data; */
/* } */

void test_trie_insert(Trie& trie, vector<string>& keys)
{
	cout << "TESTING INSERT" << endl;
	string value = "";
	for (int i = 0; i <  keys.size(); i++)
	{
		trie.insert(keys[i], value);
	}
	trie.print();
	cout << endl;
}

void test_trie_search(Trie& trie, vector<string>& keys)
{
	cout << "TESTING SEARCH" << endl;
	mt19937 rng;
	rng.seed(random_device()());
	uniform_int_distribution<mt19937::result_type> randIndex(0, keys.size()-1); 

	string key = keys[randIndex(rng)];
	cout << "SEARCHING FOR: " << key << endl << endl;

	str_itr keyItr = key.begin(),
	        keyEnd = key.end();
	TriePath triePath(trie);
	vector<string> minkeys;
	while (keyItr != keyEnd)
	{
		cout << endl << "ENTERED: " << *keyItr << endl;
		if (!triePath.update_state(*keyItr))
		{
			cout << "FAILED" << endl;
			break;
		}
		minkeys = triePath.min_keys();
		cout << "POSSIBLE COMPLETIONS:" << endl;
		for (int i = 0; i < minkeys.size(); i++)
		{
			cout << minkeys[i] << endl;
		}
		keyItr++;
	}
	cout << endl;
}


int main()
{
	Trie trie;
	string dataFile = "data/composers_small.txt";
	vector<string> values = load_text_file(dataFile);
	vector<string> keys;
	vector<string> split;
	for (vector<string>::iterator itr = values.begin(), end = values.end();
			itr < end; itr++)
	{
		split = gud::split(*itr, ' ');
		keys.insert(keys.end(), split.begin(), split.end());
	}

	test_trie_insert(trie, keys);
	/* test_trie_search(trie, keys); */

	/* string value = ""; */
	/* cout << "INSERTING" << endl; */
	/* for (int i = 0; i <  keys.size(); i++) */
	/* { */
	/* 	trie.insert(keys[i], value); */
	/* } */
	/* trie.print(); */

	cout << "TESTING SEARCH" << endl;
	mt19937 rng;
	rng.seed(random_device()());
	uniform_int_distribution<mt19937::result_type> randIndex(0, keys.size()-1); 

	string key = keys[randIndex(rng)];
	cout << "SEARCHING FOR: " << key << endl << endl;

	str_itr keyItr = key.begin(),
	        keyEnd = key.end();
	TriePath triePath(trie);
	vector<string> minkeys;
	while (keyItr != keyEnd)
	{
		cout << endl << "ENTERED: " << *keyItr << endl;
		if (!triePath.update_state(*keyItr))
		{
			cout << "FAILED" << endl;
			break;
		}
		minkeys = triePath.min_keys();
		cout << "POSSIBLE COMPLETIONS:" << endl;
		for (int i = 0; i < minkeys.size(); i++)
		{
			cout << minkeys[i] << endl;
		}
		keyItr++;
	}
	cout << endl;
}
