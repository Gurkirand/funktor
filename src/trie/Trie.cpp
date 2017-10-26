#include <iostream>
#include "Trie.h"

using namespace std;
typedef string::size_type str_size;
const char TERMINAL_CHAR = TRIE_TERMINAL_CHAR;
const char ERROR_CHAR = '!';

Node::Node() {}

Node::Node(string _key)
{
	key = _key;
	parent = nullptr;
}

Node::Node(string _key, Node* _parent)
{
	key = _key;
	parent = _parent;
}

Node::Node(string _key, string _value)
{
	key = _key;
	value = _value;
	parent = nullptr;
}

Node::Node(string _key, string _value, Node* _parent)
{
	key = _key;
	value = _value;
	parent = _parent;
}

Node::~Node(void)
{
	node_vec::size_type size = children.size();
	for (int i = 0; i < size; i++)
	{
		delete children[i];
	}
}

bool Node::contains(char c)
{
	int ord = custom_ordinal(c);
	if (bitmap[ord] == 0)
	{
		return false;
	}
	return true;
}

Node* Node::get(char c)
{
	int ord = custom_ordinal(c);
	if (bitmap[ord] == 0)
	{
		return nullptr;
	}
	return children[count_bits(bitmap, ord)];
}

bool Node::insert(Node *node, char c)
{
	int ord = custom_ordinal(c),
	    pos = count_bits(bitmap, ord);
	if (bitmap[ord] == 1)
	{
		return false;
	}
	children.insert(children.begin() + pos, node);
	node->parent = this;
	bitmap[ord] = 1;
	return true;
}

bool Node::insert(string key, string value)
{
	int ord = custom_ordinal(*(key.begin())),
	    pos = count_bits(bitmap, ord);
	if (bitmap[ord] == 1)
	{
		return false;
	}
	Node *node = new Node(key, this);
	node->insert_terminal(value);
	children.insert(children.begin() + pos, node);
	bitmap[ord] = 1;
	return true;
}

bool Node::insert_terminal(string value)
{
	int ord = TRIE_BASE_LEN - 1;
	if (bitmap[ord] == 1)
	{
		return false;
	}

	bitmap[ord] = 1;
	children.push_back(new Node(string(&TERMINAL_CHAR), value, this));
	return true;
}

bool Node::has_terminal()
{
	return bitmap[TRIE_BASE_LEN - 1];
}

bool Node::compare(Node& node)
{
	return key.compare(node.key);
}

Trie::Trie(void)
{
	base.resize(TRIE_BASE_LEN);
}

Trie::~Trie(void)
{
	node_vec::size_type size = base.size();
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
	int ord = custom_ordinal(*keyItr);

	if (base[ord] == nullptr)
	{
		base[ord] = new Node(key);
		base[ord]->insert_terminal(value);
		return true;
	}

	Node *currNode = base[ord],
		*prevNode = nullptr,
		*tempNode;
	string *compKey;
	int pref;
	while (keyItr != keyEnd)
	{
		compKey = &currNode->key;
		pref = prefix_compare(keyItr, keyEnd, 
				*compKey);
		if (pref == (keyEnd - keyItr))
		{
			return currNode->insert_terminal(value);
		}
		else if (pref == compKey->size())
		{
			prevNode = currNode;
			tempNode = currNode->get(*(keyItr + pref));
			if (tempNode == nullptr)
			{
				return currNode->insert(
					string(keyItr + pref, keyEnd), value);
			}
			currNode = tempNode;
		}
		else
		{
			currNode->key = string(
					compKey->begin() + pref, compKey->end());
			tempNode = new Node(string(keyItr, keyItr + pref), currNode->parent);
			tempNode->children.push_back(currNode);
			currNode->parent = tempNode;
			tempNode->bitmap[
				custom_ordinal(*(compKey->begin() + pref))] = 1;
			tempNode->insert(string(keyItr + pref, keyEnd), value);
			if (prevNode == nullptr)
			{
				base[custom_ordinal(*(keyItr))] = tempNode;
			}
			else
			{
				prevNode->insert(tempNode, *(keyItr));
			}
			return true;
		}
		keyItr += pref;
	}

	return true;
}

Node* Trie::get(const string& _key)
{
	string key = create_valid_key(_key) + "$";
	Node *currNode = base[custom_ordinal(*key.begin())];
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
	int i;
	node_vec baseNodes;
	for (i = 0; i < base.size(); i ++)
	{
		if (base[i] != nullptr)
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
	_push_children(visited, node);

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
			_push_children(visited, currVisit->node);
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

void Trie::_push_children(stack<NodeVisitor>& visited, Node *node)
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


char create_valid_char(char c)
{
	if ((c >= '0' && c <= '9') || 
			(c >= 'A' && c <= 'Z')
			|| c == '$')
	{
		return c;
	}
	else if (c >= 'a' && c <= 'z')
	{
		return c - 'a' + 'A';
	}
	return ERROR_CHAR;
}

string create_valid_key(const string& key)
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

int custom_ordinal(const char& c)
{
	if (c == '$')
	{
		return TRIE_BASE_LEN - 1;
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
		return TRIE_ALPHA_LEN + c - '0';
	}
	return -1;
}

int count_bits(bitset<TRIE_BASE_LEN> bitmap, int index)
{
	int i = 0;
	int count = 0;
	while (i < index)
	{
		count += bitmap[i];
		i++;
	}

	return  count;
}

int prefix_compare(str_itr itr1, str_itr end1, str_itr itr2, str_itr end2)
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

int prefix_compare(str_itr itr1, str_itr end1, string& str2)
{
	return prefix_compare(itr1, end1, 
				str2.begin(), str2.end());
}

int prefix_compare(string& str1, string& str2)
{
	return prefix_compare(str1.begin(), str1.end(),
						str2.begin(), str2.end());
}

TriePath::TriePath(Trie& trie): _trie(trie) 
{
	state = nullptr;
}

TriePath::~TriePath()
{
	/* for (int i = 0; i < _states.size(); i++) */
	/* { */
	/* 	delete _states.top(); */
	/* 	_states.pop(); */
	/* } */
}

string TriePath::lexeme()
{
	return string(_lexeme.begin(), _lexeme.end());
}

bool TriePath::update_state(char c)
{
	c = create_valid_char(c);
	if (c == ERROR_CHAR)
	{
		return false;
	}
	int ord = custom_ordinal(c);

	Node* newState = nullptr;

	if (state == nullptr)
	{
		newState = _trie.base[ord];
	}
	else if (currKeyItr != currKeyEnd)
	{
		if (*currKeyItr == c)
		{
			currKeyItr++;
			_lexeme.push_back(c);
			return true;
		}
	}
	else
	{
		newState = state->get(c);
	}

	if (newState == nullptr)
	{
		return false;
	}
	currKeyItr = state->key.begin() + 1;
	currKeyEnd = state->key.end();
	state = newState;
	_lexeme.push_back(c);

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

void TriePath::_find_min_keys()
{
	if (state == nullptr)
	{
		return;
	}
	
	if (_searchQ.empty())
	{
		_searchQ.push_back(state);
	}

	if (qDepth != 0)
	{
		_enqueue_depth();
	}

	node_vec_itr start, end;
	Node *currNode;

	while (keys.size() < TRIE_PATH_MIN_KEYS && !_searchQ.empty())
	{
		currNode = _searchQ.front();
		_searchQ.pop_front();

		start = currNode->children.begin();
		end = currNode->children.end();

		while (start != end)
		{
			if ((*start)->has_terminal())
			{
				keys.push_back(*start);
			}
			_searchQ.push_back(*start);
			start++;
		}

		if (qDepthEnd == 0)
		{
			qDepthEnd = _searchQ.size();
			qDepth++;
		}
		else
		{
			qDepthEnd--;
		}
	}
}

void TriePath::_enqueue_depth()
{
	int _depth = 0;
	int _depthEnd = 0;
	Node *currNode;
	while (_depth < qDepth)
	{
		currNode = _searchQ.front();
		_searchQ.pop_front();
		copy(currNode->children.begin(),
				currNode->children.end(),
				back_inserter(_searchQ));
		if (_depthEnd == 0)
		{
			_depthEnd = _searchQ.size();
			_depth++;
		}
		else
		{
			_depthEnd--;
		}
	}
}

void TriePath::_reset_search_queue()
{
	qDepth--;

	Node *frontNode = _searchQ.front();
	for (int i = qDepth; i > 0; i--)
	{
		frontNode = frontNode->parent;
	}

	if (frontNode->compare(*state) > 0)
	{
		qDepth++;
	}

	_searchQ.clear();
}

void TriePath::_reset_keys()
{
	Node *currNode;
	for (int i = 0; i < keys.size(); i++)
	{
		bool keep = false;
		currNode = keys[i];
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
			keys.erase(keys.begin() + i);
		}
	}
}
