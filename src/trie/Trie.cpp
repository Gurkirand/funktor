#include <iostream>
#include "../../include/trie/trie.h"

using std::string;
using std::vector;
using std::stack;
using std::bitset;
using std::cout;
using std::endl;

typedef string::size_type str_size;
const char TERMINAL_CHAR = TRIE_TERMINAL_CHAR;
const char ERROR_CHAR = '!';

Node::Node(string _key, Node* _parent, string _value): key(_key), parent(_parent), value(_value) {}

Node::~Node(void)
{
	node_vec::size_type size = children.size();
	for (int i = 0; i < size; i++)
	{
		delete children[i];
	}
}

int Node::find_index(int ord)
{
	int i = 0;
	int index = 0;
	while (i < ord)
	{
		index += bitmap[i];
		i++;
	}

	return  index;
}

bool Node::contains(char c)
{
	int ord = Trie::custom_ordinal(c);
	if (bitmap[ord] == 0) {
		return false;
	}
	return true;
}

Node* Node::get(char c)
{
	int ord = Trie::custom_ordinal(c);
	if (bitmap[ord] == 0) {
		return nullptr;
	}
	return children[find_index(ord)];
}

bool Node::insert(Node *node, char c)
{
	int ord = Trie::custom_ordinal(c),
	    pos = find_index(ord);
	if (bitmap[ord] == 1) {
		return false;
	}

	children.insert(children.begin() + pos, node);
	node->parent = this;
	bitmap[ord] = 1;
	return true;
}

bool Node::insert(string key, string value)
{
	int ord = Trie::custom_ordinal(*(key.begin())),
	    pos = find_index(ord);
	if (bitmap[ord] == 1) {
		return false;
	}

	Node *node = new Node(key, this);
	node->insert_terminal(value);
	children.insert(children.begin() + pos, node);
	bitmap[ord] = 1;
	return true;
}

bool Node::remove(Node *node)
{
	int ord = Trie::custom_ordinal(*(node->key.begin())),
	    pos = find_index(ord);
	if (bitmap[ord] == 0) {
		return false;
	}

	children.erase(children.begin() + pos);
	bitmap[ord] = 0;
	return true;
}

bool Node::insert_terminal(string value)
{
	int ord = TRIE_CHILDREN_LEN - 1;
	if (bitmap[ord] == 1) {
		return false;
	}

	bitmap[ord] = 1;
	children.push_back(new Node(string(&TERMINAL_CHAR), this, value));
	return true;
}

bool Node::has_terminal()
{
	return bitmap[TRIE_CHILDREN_LEN - 1];
}

bool Node::compare(Node& node)
{
	return key.compare(node.key);
}

Trie::Trie(void) {}

Trie::~Trie(void)
{
	size_t size = sizeof(base) / sizeof(base[0]);
	for (int i = 0; i < size; i++)
	{
		delete base[i];
	}
}

bool Trie::insert(const string& _key, const string value)
{
	string key = create_valid_key(_key);
	str_itr keyItr = key.begin(),
	        keyEnd = key.end();
	int ord = Trie::custom_ordinal(*keyItr);

	if (base[ord] == nullptr)
	{
		base[ord] = new Node(key);
		base[ord]->insert_terminal(value);
		return true;
	}

	Node *currNode = base[ord],
		*tempNode;
	string *compKey;
	int pref;
	while (keyItr != keyEnd)
	{
		compKey = &currNode->key;
		pref = prefix_compare(keyItr, keyEnd, 
				*compKey);
		if (pref != compKey->size())
		{
			string sharedPref = string(keyItr, keyItr + pref),
			       insertSuff = string(keyItr + pref, keyEnd),
			       currSuff = string(compKey->begin() + pref, compKey->end());
			rotation_insertion(sharedPref, insertSuff, value, currNode, currSuff);
			return true;
		}
		else
		{
			if (pref == (keyEnd - keyItr))
			{
				return currNode->insert_terminal(value);
			}

			tempNode = currNode->get(*(keyItr + pref));
			if (!tempNode) 
			{
				return currNode->insert(
					string(keyItr + pref, keyEnd), value);
			}
			currNode = tempNode;
		}
		
		keyItr += pref;
	}

	return true;
}

void Trie::rotation_insertion(string& sharedPref,
							   string& insertSuff,
							   const string& value,
							   Node* root,
							   string& rootSuff)
{
	Node* pivot = new Node(sharedPref, root->parent);
	pivot->children.push_back(root);
	root->parent = pivot;

	if (pivot->parent) 
	{
		pivot->parent->remove(root);
		pivot->parent->insert(pivot, *sharedPref.begin());
	}
	else 
	{
		base[Trie::custom_ordinal(*sharedPref.begin())] = pivot;
	}
	
	root->key = rootSuff;
	pivot->bitmap[
		Trie::custom_ordinal(*(rootSuff.begin()))] = 1;

	if (insertSuff.empty()) 
	{
		pivot->insert(insertSuff, value);
	}
	else 
	{
		pivot->insert_terminal(value);
	}
}

Node* Trie::get(const string& _key)
{
	string key = create_valid_key(_key) + "$";
	Node *currNode = base[Trie::custom_ordinal(*key.begin())];
	str_itr keyItr = key.begin(),
	        keyEnd = key.end();
	int pref;
	while (currNode != nullptr && keyItr < keyEnd - 1)
	{
		keyItr += prefix_compare(keyItr, keyEnd, currNode->key);
		currNode = currNode->get(*keyItr);
	}
	return currNode;
}

void Trie::print()
{
	cout << endl;
	size_t size = sizeof(base) / sizeof(base[0]),
	       i;
	node_vec baseNodes;
	for (i = 0; i < size; i ++)
	{
		if (base[i])
		{
			baseNodes.push_back(base[i]);
		}
	}
	print(baseNodes, "");
}

void Trie::print(node_vec nodes, string tabs)
{
	int printIndex;
	Node *currNode;
	node_vec front, back;
	node_vec_itr start, half, end;
	for (int i = 0; i < nodes.size(); i++)
	{
		currNode = nodes[i];

		if (currNode->children.empty())
		{
			cout << tabs << currNode->key << endl;
			continue;
		}

		start = currNode->children.begin();
		half = currNode->children.begin() + 
			(currNode->children.size() / 2);
		end = currNode->children.end();
		front = node_vec(start, half);
		back = node_vec(half, end);

		cout << endl;
		print(front, tabs + "\t");
		cout << tabs << nodes[i]->key << endl;
		print(back, tabs + "\t");
		continue;
	}
	return;
}

vector<string> Trie::find_keys(Node *node)
{

	vector<char> _lexeme(node->key.begin(), node->key.end());
	stack<NodeVisitor> visited;
	vector<string> keys;
	push_children(visited, node);

	NodeVisitor* currVisit;
	int i = 0;
	while (!visited.empty())
	{
		currVisit = &visited.top();
		if (currVisit->node->key.compare(&TERMINAL_CHAR) == 0 || currVisit->node->children.empty())
		{
			keys.push_back(string(_lexeme.begin(), _lexeme.end()));
		}
		else if (!currVisit->visited)
		{
			push_children(visited, currVisit->node);
			copy(currVisit->node->key.begin(),
					currVisit->node->key.end(),
					back_inserter(_lexeme));
			currVisit->visited = true;
			continue;
		}
		else
		{
			str_size size = currVisit->node->key.size();
			while (size > 0)
			{
				_lexeme.pop_back();
				size--;
			}
		}
		visited.pop();
		i++;
	}
	return keys;
}

vector<string> Trie::find_keys(string pref)
{
	pref = create_valid_key(pref);
	Node *node = get(pref);
	return find_keys(node);
}

void Trie::push_children(stack<NodeVisitor>& visited, Node *node)
{
	node_vec_itr start = node->children.begin(),
	             end = node->children.end();
	while (start != end)
	{
		NodeVisitor visitor = {*start, false};
		visited.push(visitor);
		start++;
	}
}


char Trie::create_valid_char(char c)
{
	if ((c >= '0' && c <= '9') || 
			(c >= 'a' && c <= 'z')
			|| c == '$')
	{
		return c;
	}
	else if (c >= 'A' && c <= 'Z')
	{
		return c - 'A' + 'a';
	}
	return ERROR_CHAR;
}

string Trie::create_valid_key(const string& key)
{
	char keycpy[key.size() + 1], c;
	string::const_iterator itr = key.begin(),
	        end = key.end();
	int i = 0;
	while (itr != end)
	{
		c = create_valid_char(*itr);
		if (c == ERROR_CHAR)
		{
			i--;
		}
		else
		{
			keycpy[i] = c;
		}
		itr++;
		i++;
	}

	keycpy[i] = '\0';
	
	return string((char *) keycpy);
}

int Trie::custom_ordinal(const char& c)
{
	if (c == '$')
	{
		return TRIE_CHILDREN_LEN - 1;
	}
	else if (c >= 'A' && c <= 'Z')
	{
		return c - 'A';
	}
	else if (c >= 'a' && c <= 'z')
	{
		return c - 'a';
	}
	else if (c >= '0' && c <= '9')
	{
		return 26 + c - '0'; //Numbers come after letters, therefore add 26 to ord
	}
	return -1;
}

int Trie::prefix_compare(str_itr itr1, str_itr end1, str_itr itr2, str_itr end2)
{
	int end = (end1 - itr1) < (end2 - itr2) ? (end1 - itr1) : (end2 - itr2);
	str_itr begin1 = itr1;
	end1 = itr1 + end;

	for (; itr1 < end1; itr1++, itr2++)
	{
		if (*itr1 != *itr2)
		{
			break;
		}
	}

	return itr1 - begin1;
}

int Trie::prefix_compare(str_itr itr1, str_itr end1, string& str2)
{
	return prefix_compare(itr1, end1, 
				str2.begin(), str2.end());
}

int Trie::prefix_compare(string& str1, string& str2)
{
	return prefix_compare(str1.begin(), str1.end(),
						str2.begin(), str2.end());
}

TriePath::TriePath(Trie& trie, int minSearch): trie(trie), search_len(minSearch), state(nullptr) {}

TriePath::~TriePath()
{
	/* for (int i = 0; i < _states.size(); i++) */
	/* { */
	/* 	delete _states.top(); */
	/* 	_states.pop(); */
	/* } */
}

vector<string> TriePath::keys()
{
	vector<string> keys;
	Node *currNode;
	for (int i = 0; i < key_nodes.size(); i++)
	{
		currNode = key_nodes[i]->parent;
		string key;
		while (currNode != nullptr)
		{
			key.insert(0, currNode->key);
			currNode = currNode->parent;
		}
		keys.push_back(key);
	}
	return keys;
}

string TriePath::lexeme()
{
	return string(_lexeme.begin(), _lexeme.end());
}

bool TriePath::update_state(char c)
{
	c = Trie::create_valid_char(c);
	if (c == ERROR_CHAR) {
		return false;
	}

	int ord = Trie::custom_ordinal(c);
	Node* newState = nullptr;

	if (state == nullptr)
	{
		newState = trie.base[ord];
	}
	else if (key_itr != key_end)
	{
		if (*key_itr == c)
		{
			key_itr++;
			_lexeme.push_back(c);
			return true;
		}
	}
	else
	{
		newState = state->get(c);
	}

	if (newState == nullptr) {
		return false;
	}

	state = newState;
	key_itr = state->key.begin() + 1;
	key_end = state->key.end();
	_lexeme.push_back(c);
	reset_key_nodes();
	reset_search_queue();
	find_key_nodes();

	return true;
}

/*
 * 
 *  Currently, finds the 5 closest completions via BreadthFirstSearch.
 *  Keeps track of depth. When next step is taken, that node will queue up
 *  to that depth of its child nodes, and continue search. Also checks
 *  to see if any of the previous completions originated from that 
 *  nodes subtree.
 * 
 *  TODO: Keep track of which subarray of the queue came form which child 
 *  node. Only need to keep track of 1 depth at each step. This would make
 *  it easier to update queue on move.
 * 
 */

void TriePath::find_key_nodes()
{
	if (state == nullptr) {
		return;
	}

	if (search_q.empty()) {
		search_q.push_back(state);
	}

	if (q_depth != 0) {
		enqueue_depth();
	}

	node_vec_itr start, end;
	Node *currNode;
	while (key_nodes.size() < search_len && !search_q.empty())
	{
		currNode = search_q.front();
		search_q.pop_front();

		start = currNode->children.begin();
		end = currNode->children.end() - (currNode->has_terminal());
		while (start < end)
		{
			if ((*start)->has_terminal())
			{
				key_nodes.push_back(*((*start)->children.end()-1));
			}
			search_q.push_back(*start);
			start++;
		}

		if (q_partition == 0)
		{
			q_partition = search_q.size();
			q_depth++;
		}
		else
		{
			q_partition--;
		}
	}
}

void TriePath::enqueue_depth()
{
	int _depth = 0;
	int _partition = 0;
	Node *currNode;
	while (_depth < q_depth && !search_q.empty())
	{
		currNode = search_q.front();
		search_q.pop_front();
		copy(currNode->children.begin(),
				currNode->children.end(),
				back_inserter(search_q));
		if (_partition == 0)
		{
			_partition = search_q.size();
			_depth++;
		}
		else
		{
			_partition--;
		}
	}
}

void TriePath::reset_search_queue()
{
	if (search_q.size() == 0) {
		return;
	}

	q_depth--;
	Node *frontNode = search_q.front();
	for (int i = q_depth; i > 0; i--)
	{
		frontNode = frontNode->parent;
	}

	if (frontNode->compare(*state) > 0)
	{
		q_depth++;
	}

	search_q.clear();
}

void TriePath::reset_key_nodes()
{
	if (key_nodes.size() == 0) {
		return;
	}

	Node *currNode;
	int i = 0;
	while (i < key_nodes.size())
	{
		bool keep = false;
		currNode = key_nodes[i];
		while (currNode != nullptr)
		{
			if (currNode == state)
			{
				keep = true;
				break;
			}
			currNode = currNode->parent;
		}

		if (!keep)
		{
			key_nodes.erase(key_nodes.begin() + i);
			continue;
		}

		i++;
	}
}
