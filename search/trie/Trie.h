#ifndef TRIE_H
#define TRIE_H
#define TRIE_BASE_LEN 37
#define TRIE_ALPHA_LEN 26
#define TERMINAL_CHAR "$"

#include <string>
#include <bitset>
#include <vector>

//trie/Trie.h

class Node;

typedef std::vector<Node *> node_vec;
typedef std::string::iterator str_itr;

class Node
{
	public:
		std::string key;
		std::string value;
		std::bitset<TRIE_BASE_LEN> bitmap;
		node_vec children;
		

		Node();
		Node(std::string _key);
		~Node();
		bool contains(char c);
		Node* get(char c);
		bool insert(Node *node, char c);
		bool insert(std::string sub, std::string key, std::string value);
		bool insert_terminal(std::string key, std::string value);

	private:
		Node(std::string _key, std::string _value);
};

class Trie
{
	public:
		node_vec base;

		Trie();
		~Trie();
		bool insert(const std::string& _key, std::string value);
		void print();

	private:
		void print(node_vec nodes, std::string tabs);
};

/* void create_valid_key(std::string& key, std::string::size_type size); */
std::string create_valid_key(const std::string& key);
int custom_ordinal(const char& c);
int count_bits(std::bitset<TRIE_BASE_LEN> bitmap, int index);
int prefix_compare(str_itr itr1, str_itr end1, str_itr itr2, str_itr end2);
/* int prefix_compare(std::string str1, std::string str2); */


#endif
