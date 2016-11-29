#pragma once
#include <map>

class IndexMap
{
	typedef size_t key_t;
public:
	IndexMap();
	~IndexMap();
	key_t Add(size_t index);
	size_t Next(key_t) const;
	void Remove(key_t,size_t shift);
	void Print();
	size_t operator[](key_t) const;
protected:
private:
	std::map<key_t,size_t> _map;
};
