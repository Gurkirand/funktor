#include <unordered_map>
#include "data/load.h"
#include "../src/util/util.h"
#include "../include/hash_func.h"
#include "../lib/smhasher/src/MurmurHash3.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::back_insert_iterator;
using std::back_inserter;
using std::copy;

template <typename T>
using hash = gud::murmur_hash<T>;

template <typename K, typename V>
using hash_map = std::unordered_map<K, V, hash<K> >;

vector<string> load_test_data(const string& filename);
void test_insert(const vector<string>& data);

int main()
{
	string dataFile = "test/data/composers_small.txt";
	vector<string> data = load_test_data(dataFile);
	cout << data.size() << endl;

	test_insert(data);
}

vector<string> load_test_data(const string& filename)
{
	vector<string> data;
	vector<string> lines = load_text_file(filename);
	vector<string> split;
	back_insert_iterator< vector<string> > dataItr = back_inserter(data);
	for (vector<string>::iterator itr = lines.begin(), end = lines.end();
			itr < end; itr++)
	{
		split = gud::split(*itr, ' ');
		copy(split.begin(), split.end(), dataItr);
	}

	return data;
}

void test_insert(const vector<string>& data)
{
	cout << data.size() << endl;
	hash_map<string, string> test_hash_map;
	int i = 0;
	for (vector<string>::const_iterator itr = data.begin(), end = data.end();
			itr < end; itr++, i++)
	{
		test_hash_map[*itr] = *itr;
		if (i % 25 == 0)
		{
			cout << test_hash_map[*itr] << endl;
		}
	}
	cout << "PASSED INSERT TESTS" << endl;
}
