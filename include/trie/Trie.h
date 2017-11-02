#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <bitset>
#include <vector>
#include <stack>
#include <deque>
/* #include <tuple> */

//trie/Trie.h

/*
 * Global constants for trie.
 */
extern const int TRIE_BASE_LEN = 37; //Accepted character length, [a-z0-9$]
extern const int TRIE_ALPHA_LEN = 26; //Position of last alphabet character + 1
extern const char TRIE_TERMINAL_CHAR = '$'; //Terminal character to designate leaf nodes in the trie
extern const int TRIE_PATH_MIN_KEYS = 5; //How many keys to search for

class Node;
class Trie;
class TriePath;
class TrieSearch;

typedef std::vector<Node*> node_vec;
typedef node_vec::const_iterator node_vec_itr;
typedef std::string::const_iterator str_itr;

class Node
{
	public:
		std::string key;
		std::string value;
		std::bitset<TRIE_BASE_LEN> bitmap;
		node_vec children;
		Node* parent;

		Node(std::string _key, Node* _parent = nullptr, std::string value = "");
		~Node();
		int find_index(int ord);
		bool contains(char c);
		Node* get(const char c);
		bool insert(Node *node, char c);
		bool insert(std::string key, std::string value);
		bool remove(Node *node);
		bool remove(std::string key);
		bool insert_terminal(std::string value);
		bool has_terminal();
		bool compare(Node& node);
};


class Trie
{
	public:
		Node* base[TRIE_BASE_LEN];

		Trie();
		~Trie();
		bool insert(const std::string& _key, const std::string value);
		Node* get(const std::string& key);
		std::vector<std::string> find_keys(Node *node);
		std::vector<std::string> find_keys(std::string pref);
		void print();

		static char create_valid_char(char c);
		static std::string create_valid_key(const std::string& key);
		static int custom_ordinal(const char& c);
		static int prefix_compare(str_itr itr1, str_itr end1, str_itr itr2, str_itr end2);
		static int prefix_compare(str_itr itr1, str_itr end1, std::string& str2);
		static int prefix_compare(std::string& str1, std::string& str2);


	private:
		struct NodeVisitor
		{
			Node* node;
			bool visited;
		};

		void _print(node_vec nodes, std::string tabs);
		void _push_children(std::stack<NodeVisitor>& visited, Node *node);
		void _rotation_insertion(std::string& pivot, std::string&childKey, const std::string& value,
									Node* root, std::string& rootKey);
};


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
		std::vector<Node*> _keys;
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

