#include <iostream>
#include "Node.h"
#include <stack>

using namespace std;
typedef string::size_type str_size;

Node::Node() {}

Node::Node(string _key)
{
	key = _key;
}

Node::Node(string _key, string _value)
{
	key = _key;
	value = _value;
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
	bitmap[ord] = 1;
	return true;
}

bool Node::insert(string sub, string key, string value)
{
	int ord = custom_ordinal(*(key.begin())),
	    pos = count_bits(bitmap, ord);
	if (bitmap[ord] == 1)
	{
		return false;
	}
	Node *node = new Node(sub);
	node->insert_terminal(key, value);
	children.insert(children.begin() + pos, node);
	bitmap[ord] = 1;
	return true;
}

bool Node::insert_terminal(string key, string value)
{
	int ord = TRIE_BASE_LEN - 1;
	if (bitmap[ord] == 1)
	{
		return false;
	}

	bitmap[ord] = 1;
	children.push_back(new Node(key, value));
	return true;
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
		base[ord]->insert_terminal(key, value);
		return true;
	}

	Node *currNode = base[ord],
		*prevNode,
		*tempNode;
	string *compKey;
	int pref;
	while (keyItr != keyEnd)
	{
		compKey = &currNode->key;
		pref = prefix_compare(keyItr, keyEnd, 
				compKey->begin(), compKey->end());
		if (pref == (keyEnd - keyItr))
		{
			return currNode->insert_terminal(key, value);
		}
		else if (pref == compKey->size())
		{
			prevNode = currNode;
			if (currNode->contains(*(keyItr + pref)))
			{
				currNode = currNode->get(*(keyItr + pref));
			}
			else
			{
				return currNode->insert(
					string(keyItr + pref, keyEnd), key, value);
			}
		}
		else
		{
			currNode->key = string(
					compKey->begin() + pref, compKey->end());
			tempNode = new Node(string(keyItr, keyItr + pref));
			tempNode->children.push_back(currNode);
			tempNode->bitmap[
				custom_ordinal(*(compKey->begin() + pref))] = 1;
			tempNode->insert(string(keyItr + pref, keyEnd), key, value);
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

/* bool Trie::insert(const string& _key, const string value) */
/* { */
/* 	string key = create_valid_key(_key); */
/* 	int keySize = key.size(); */
	
/* 	int ord = custom_ordinal(key[0]); */
/* 	if (base[ord] == nullptr) */
/* 	{ */
/* 		base[ord] = new Node(); */
/* 		base[ord]->key = key; */
/* 		base[ord]->value = value; */
/* 		return true; */
/* 	} */

/* 	Node *currNode = base[ord], */
/* 		*prevNode = nullptr, *tempNode, *newBase; */
/* 	int i = 0, pref, pos, tempOrd; */
/* 	string sub; */

/* 	while (i < keySize) */
/* 	{ */
/* 		sub = key.substr(i, keySize - i); */
/* 		pref = prefix_compare(sub, currNode->key); */
/* 		if (pref == currNode->key.size()) */
/* 		{ */
/* 			ord = custom_ordinal(sub[pref]); */
/* 			pos = count_bits(currNode->bitmap, ord); */
/* 			if (currNode->bitmap[ord] == 0) */
/* 			{ */
/* 				tempNode = new Node( */
/* 						sub.substr(pref, sub.size() - pref)); */
/* 				tempNode->value = value; */
/* 				currNode->children.insert( */
/* 						currNode->children.begin() + pos, */
/* 						tempNode); */
/* 				currNode->bitmap[ord] = 1; */
/* 				return true; */
/* 			} */
/* 			prevNode = currNode; */
/* 			currNode = currNode->children[pos]; */
/* 		} */
/* 		else */
/* 		{ */
/* 			currNode->key = currNode->key.substr(pref, currNode->key.size() - pref); */
/* 			tempNode = new Node(); */
/* 			tempNode->key = sub.substr(0, pref); */
/* 			tempNode->children.push_back(currNode); */
/* 			ord = custom_ordinal(currNode->key[pref]); */
/* 			tempNode->bitmap[custom_ordinal(currNode->key[pref])] = 1; */
			
/* 			if (prevNode == nullptr) */
/* 			{ */
/* 				base[custom_ordinal(sub[0])] = tempNode; */
/* 			} */
/* 			else */
/* 			{ */
/* 				prevNode->children[sub[0]] = tempNode; */
/* 			} */

/* 			currNode = new Node(); */
/* 			currNode->key = sub.substr(pref, sub.size() - pref); */
/* 			ord = custom_ordinal(sub[pref]); */
/* 			pos = count_bits(tempNode->bitmap, ord); */
/* 			tempNode->children.insert(tempNode->children.begin() + pos, currNode); */
/* 			tempNode->bitmap[ord] = 1; */
/* 			return true; */
/* 		} */
/* 		i = pref; */
/* 	} */

/* 	return true; */
/* } */

void Trie::print()
{
	cout << endl;
	node_vec baseNodes;
	for (int i = 0; i < base.size(); i ++)
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
	node_vec::const_iterator start, half, end;
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

string create_valid_key(const string& key)
{
	char keycpy[key.size() + 1], c;
	string::const_iterator itr = key.begin(),
	        end = key.end();
	int i = 0;

	while (itr != end)
	{
		c = *itr;
		if ((c >= '0' && c <= '9') || 
				(c >= 'A' && c <= 'Z')
				|| c == '$')
		{
			keycpy[i] = c;
		}
		else if (c >= 'a' && c <= 'z')
		{
			keycpy[i] = c - 'a' + 'A';
		}
		else
		{
			/* itr++; */
			/* continue; */
			i--;
		}
		itr++;
		i++;
	}

	keycpy[i] = '\0';
	
	return string((char *) keycpy);
}

void create_valid_key(string& key, str_size size)
{
	int i = 0;
	char c;

	while (i < size)
	{
		c = key[i];
		if ((c >= '0' && c <= '9') || 
				(c >= 'A' && c <= 'Z')
				|| c == '$')
		{
			i++;
			continue;
		}
		else if (c >= 'a' && c <= 'z')
		{
			c = c - 'a' + 'A';
			key[i] = c;
			i++;
		}
		else
		{
			key.erase(i, 1);
			size = key.size();
		}
	}
}

int custom_ordinal(const char& c)
{
	/* if (c == '$') */
	/* { */
	/* 	return TRIE_BASE_LEN - 1; */
	/* } */
	if (c >= 'A' && c <= 'Z')
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
	while (i <= index)
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

/* int prefix_compare(string str1, string str2) */
/* { */
/* 	if (str1.empty() || str2.empty()) */
/* 	{ */
/* 		return 0; */
/* 	} */
/* 	int size = str1.size() < str2.size() ? str1.size() : str2.size(), */
/* 	    i = 0; */

/* 	for (int i = 0; i < size; i++) */
/* 	{ */
/* 		if (str1[i] != str2[i]) */
/* 		{ */
/* 			return i; */
/* 		} */
/* 	} */
/* 	return i; */
/* } */

