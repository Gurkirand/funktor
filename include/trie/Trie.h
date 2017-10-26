#ifndef TRIE_H
#define TRIE_H
#define TRIE_BASE_LEN 37
#define TRIE_ALPHA_LEN 26
#define TRIE_TERMINAL_CHAR '$'
#define TRIE_PATH_MIN_KEYS 5

#include <string>
#include <bitset>
#include <vector>
#include <stack>
#include <deque>
/* #include <tuple> */

//trie/Trie.h

class Node;

typedef std::vector<Node*> node_vec;
typedef node_vec::const_iterator node_vec_itr;

class Node
{
	public:
		std::string key;
		std::string value;
		std::bitset<TRIE_BASE_LEN> bitmap;
		node_vec children;
		Node* parent;

		Node();
		Node(std::string _key);
		Node(std::string _key, Node* _parent);
		~Node();
		bool contains(char c);
		Node* get(const char c);
		bool insert(Node *node, char c);
		bool insert(std::string key, std::string value);
		bool insert_terminal(std::string value);
		bool has_terminal();
		bool compare(Node& node);

	private:
		Node(std::string _key, std::string _value);
		Node(std::string _key, std::string _value, Node* _parent);
};


struct NodeVisitor
{
	Node* node;
	bool visited;
};

class Trie
{
	public:
		node_vec base;

		Trie();
		~Trie();
		bool insert(const std::string& _key, std::string value);
		Node* get(const std::string& key);
		std::vector<std::string> find_keys(Node *node);
		std::vector<std::string> find_keys(std::string pref);
		void print();


	private:
		void print(node_vec nodes, std::string tabs);
		void _push_children(std::stack<NodeVisitor>& visited, Node *node);
};

typedef std::string::const_iterator str_itr;

std::string create_valid_key(const std::string& key);
int custom_ordinal(const char& c);
int count_bits(std::bitset<TRIE_BASE_LEN> bitmap, int index);
int prefix_compare(str_itr itr1, str_itr end1, str_itr itr2, str_itr end2);
int prefix_compare(str_itr itr1, str_itr end1, std::string& str2);
int prefix_compare(std::string& str1, std::string& str2);

class TriePath
{
	public:
		TriePath(Trie&);
		~TriePath();
		Node* state;
		std::vector<std::string> min_keys();
		std::string lexeme();
		bool update_state(char c);

	private:
		Trie& _trie;
		str_itr currKeyItr;
		str_itr currKeyEnd;
		std::vector<char> _lexeme;
		std::vector<Node*> keys;
		std::deque<Node*> _searchQ;
		int qDepth;
		int qDepthEnd;

		void _find_min_keys();
		void _enqueue_depth();
		void _reset_search_queue();
		void _reset_keys();
};

class TrieSearch
{
	TriePath *current;
	std::stack<TriePath*> paths;
};

#endif

