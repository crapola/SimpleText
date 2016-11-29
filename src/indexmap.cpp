#include <cassert>
#include <iterator>
#include "indexmap.h"

#ifndef NDEBUG
#include <iostream>
#endif

IndexMap::IndexMap():_map()
{
}

IndexMap::~IndexMap()
{
}

IndexMap::key_t IndexMap::Add(size_t p_index)
{
	static key_t id=0;
	id++;
	assert(id!=0);
	_map.emplace(id,p_index);
#ifndef NDEBUG
	std::cout<<"Add {Key="<<id<<" : Index="<<p_index<<"}\n";
#endif
	return id;
}

size_t IndexMap::Next(key_t p_key) const
{
	auto it=_map.find(p_key);
	++it;
	if (it!=_map.end())
		return it->second;
	else
		return 0;
}

void IndexMap::Remove(key_t p_key,size_t p_shift)
{
	auto it=_map.find(p_key);
	if (it!=_map.end())
	{
#ifndef NDEBUG
		std::cout<<"Delete {Key="<<it->first<<" : Index="<<it->second<<"}\n";
#endif
		// Delete and shift next indices.
		it=_map.erase(it);
		for (; it!=_map.end(); ++it)
		{
			it->second-=p_shift;
		}
	}
	else
	{
#ifndef NDEBUG
		std::cerr<<"IndexMap::Remove: Invalid key.\n";
#endif
	}
}

void IndexMap::Print()
{
#ifndef NDEBUG
	for (auto it=_map.begin(); it!=_map.end(); ++it)
	{
		std::cout<<"{Key="<<it->first<<" : Index="<<it->second<<"}\n";
	}
#endif
}

size_t IndexMap::operator[](key_t p_k) const
{
	return _map.at(p_k);
}