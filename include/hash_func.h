#ifndef GUD_HASH_H
#define GUD_HASH_H

#include "../lib/MurmurHash3.h"
#include <string>


namespace gud
{

template <typename K>
using hash_func = size_t (*) (const K& key);

template <typename K>
struct hash
{
	using argument_type = K;
	using result_type = size_t;

public:
	hash(hash_func<K> h): hashf(h) {}

	~hash() {}

	hash(const hash<K>& other)
	{
		hashf = other.hashf;
	}

	inline size_t operator() (const K& key) const
	{
		return hashf(key);
	}

protected:
	hash_func<K> hashf;
};

size_t generic_murmur_hash(const void* key, int len)
{
	uint32_t out[1];
	uint32_t seed = 42;

	MurmurHash3_x86_32( key, len, seed, out);
	return out[0];
}

template <typename K>
size_t murmur_hash_wrapper(const K& k)
{
	void *key = k;
	int len = sizeof(k);
	return generic_murmur_hash(key, len);
}

template <>
size_t murmur_hash_wrapper<std::string>(const std::string& str)
{
	const char *key = str.c_str();
	int len = strlen(key);
	return generic_murmur_hash(key, len);
}

template<typename T>
class murmur_hash: public hash<T>
{
public:
	murmur_hash(): hash<T>(murmur_hash_wrapper){}
};

} // end namespace gud

#endif // GUD_HASH_H
