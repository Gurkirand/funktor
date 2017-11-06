#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <bitset>
#include <vector>
#include <stack>
#include <deque>
/* #include <tuple> */

//trie/Trie.h
/* TODO:
 * Determine whether values are necessary to store in the trie.
 *  - Most likely they are not. If so simply mark nodes that are terminal, instead of 
 *      storing a "terminal node".
 *  - If they are needed, split the node class into regular nodes and terminal nodes.
 *      This leaves the issue of storing tags with the same word (ex: John Cage vs.
 *      John Williams, if the search is "john")
 */

/*
 * Global constants for trie.
 */
extern const int TRIE_CHILDREN_LEN = 37; //Accepted character length, [a-z0-9$]
extern const char TRIE_TERMINAL_CHAR = '$'; //Terminal character to designate leaf nodes in the trie
extern const int TRIE_PATH_MIN_SEARCH_LEN = 5; //How many keys to search for

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
		std::bitset<TRIE_CHILDREN_LEN> bitmap;
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
		Node* base[TRIE_CHILDREN_LEN - 1]; //No need for terminal node

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

	private:
		struct NodeVisitor
		{
			Node* node;
			bool visited;
		};

		void print(node_vec nodes, std::string tabs);
		void push_children(std::stack<NodeVisitor>& visited, Node *node);
		void rotation_insertion(std::string& pivot, std::string&childKey, const std::string& value,
									Node* root, std::string& rootKey);

		static int prefix_compare(str_itr itr1, str_itr end1, str_itr itr2, str_itr end2);
		static int prefix_compare(str_itr itr1, str_itr end1, std::string& str2);
		static int prefix_compare(std::string& str1, std::string& str2);

};


class TriePath
{
	public:
		TriePath(Trie& trie, int minSearch = TRIE_PATH_MIN_SEARCH_LEN);
		~TriePath();
		Node* state;
		std::vector<std::string> keys();
		std::string lexeme();
		bool update_state(char c);

	private:
		Trie& trie;
		str_itr key_itr;
		str_itr key_end;
		std::vector<char> _lexeme;
		std::vector<Node*> key_nodes;
		std::deque<Node*> search_q;
		size_t search_len;
		size_t q_depth;
		size_t q_partition;

		void find_key_nodes();
		void enqueue_depth();
		void reset_search_queue();
		void reset_key_nodes();
};

class TrieSearch
{
	TriePath *current;
	std::stack<TriePath*> paths;
};

#endif

