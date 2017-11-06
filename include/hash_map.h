#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <string>
#include <utility>
#include <memory>

//TODO:
//Fill up methods, sigh
//More importantly create a wrapper for buckets, which will store distance off


namespace gud
{

template <class Iterator, class NodeType> struct /*unspecified*/ {
    Iterator position;
    bool     inserted;
    NodeType node;
};
template <
	typename K, typename V, 
	typename Hash = std::hash<K>,
	typename KeyEqual = std::equal_to<Key>,
	typename Allocator = std::allocator< std::pair<const K, V> >
>
class hash_map
{
public:
	using key_type = K;
	using mapped_type = V;
	using value_type = std::pair<const K, V>;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using hasher = Hash;
	using key_equal = KeyEqual;
	using allocator_type = Allocator;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = std::allocator_traits<Allocator>::pointer;
	using const_pointer = std::alocator_traits<Allocator>::const_pointer;
	using iterator = ForwardIterator;
	using const_iterator = const ForwardIterator;
	using local_iterator = iterator;
	using const_local_iterator = const_iterator;
	/* using node_type = */ 
	/* using insert_return_type = */ 

    explicit hash_map(size_type n = see below,
                           const hasher& hf = hasher(),
                           const key_equal& eql = key_equal(),
                           const allocator_type& a = allocator_type());
	    
    template <class InputIterator>
    hash_map(InputIterator f, InputIterator l,
                  size_type n = see below,
                  const hasher& hf = hasher(),
                  const key_equal& eql = key_equal(),
                  const allocator_type& a = allocator_type());
    hash_map(const hash_map&);
    hash_map(hash_map&&);
    explicit hash_map(const Allocator&);
    hash_map(const hash_map&, const Allocator&);
    hash_map(hash_map&&, const Allocator&);
    hash_map(initializer_list<value_type>,
                  size_type = see below,
                  const hasher& hf = hasher(),
                  const key_equal& eql = key_equal(),
                  const allocator_type& a = allocator_type());
    ~hash_map();
    hash_map& operator=(const hash_map&);
    hash_map& operator=(hash_map&&);
    hash_map& operator=(initializer_list<value_type>);
    allocator_type get_allocator() const noexcept;	

	    // size and capacity
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;
 
    // iterators
    iterator       begin() noexcept;
    const_iterator begin() const noexcept;
    iterator       end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
 
    // modifiers
    template <class... Args> pair<iterator, bool> emplace(Args&&... args);
    template <class... Args> iterator emplace_hint(const_iterator position, Args&&... args);
    pair<iterator, bool> insert(const value_type& obj);
    template <class P> pair<iterator, bool> insert(P&& obj);
    iterator insert(const_iterator hint, const value_type& obj);
    template <class P> iterator insert(const_iterator hint, P&& obj);
    template <class InputIterator> void insert(InputIterator first, InputIterator last);
    void insert(initializer_list<value_type>);
 
    iterator erase(const_iterator position);
    size_type erase(const key_type& k);
    iterator erase(const_iterator first, const_iterator last);
    void clear() noexcept;
 
    void swap(unordered_map&);
 
 
    // observers
    hasher hash_function() const;
    key_equal key_eq() const;
 
    // lookup
    iterator find(const key_type& k);
    const_iterator find(const key_type& k) const;
    size_type count(const key_type& k) const;
    std::pair<iterator, iterator>
    equal_range(const key_type& k);
    std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const;
    mapped_type& operator[](const key_type& k);
    mapped_type& operator[](key_type&& k);
    mapped_type& at(const key_type& k);
    const mapped_type& at(const key_type& k) const;
 
    // bucket interface
    size_type bucket_count() const noexcept;
    size_type max_bucket_count() const noexcept;
    size_type bucket_size(size_type n) const;
    size_type bucket(const key_type& k) const;
    local_iterator begin(size_type n);
    const_local_iterator begin(size_type n) const;
    local_iterator end(size_type n);
    const_local_iterator end(size_type n) const;
    const_local_iterator cbegin(size_type n) const;
    const_local_iterator cend(size_type n) const;
    // hash policy
    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float z);
    void rehash(size_type n);
    void reserve(size_type n);

private:

}

//End Namespace
}

#endif
