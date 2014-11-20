#ifndef VECMAP_HPP_INCLUDED
#define VECMAP_HPP_INCLUDED

#include <vector>
#include <map>

template<class value_t>
class VecMap
{
	typedef size_t key_t;
	typedef typename std::vector<value_t>::iterator iter_t;
public:
	// Constructor
	VecMap(std::vector<value_t>& p_b):_buffer(p_b),_refs() {}

	// Key -> Map -> Offset
	value_t* PointedBy(const key_t& p_k) const
	{
		if (_refs.find(p_k)==_refs.end())
		{
			return nullptr;
		}
		return &*_refs.at(p_k);
	}

	key_t Create(size_t p_index)
	{
		key_t id=p_index;
		_refs[id]=_buffer.begin()+p_index;
		//cout<<"Created entry [ "<<id<<" -> "<<p_index<<" ("<<(void*)(&*_buffer.begin()+p_index)<<")]\n";
		return id;
	}

	// Delete by key and erase buffer up to next entry.
	void Delete(const key_t& p_key)
	{
		auto mapit=_refs.find(p_key);
		//cout<<"Deleting entry "<<p_key<<" "<<_refs.count(p_key)<<endl;
		if (mapit==_refs.end())
		{
			//cout<<"Can't delete entry "<<p_key<<", it doesn't exist.\n";
			return;
		};
		iter_t from=_refs.at(p_key);
		iter_t to=_buffer.end();
		auto nextup=next(mapit,1);
		if (nextup!=_refs.end())
			to=next(mapit,1)->second;

		_buffer.erase(from,to);

		// shift
		for (auto i=mapit; i!=_refs.end(); ++i)
		{
			(i->second)-=(to-from);
		}
		_refs.erase(p_key);
	}

	// debug
	/*void Show() const
	{
		cout<<"Map:\nBuffer:"<<static_cast<const void*>(_buffer.data())<<"\n";
		const void* adr;
		for (auto zob:_refs)	cout<<zob.first<<";"<<(adr=(&*zob.second))<<"\n";
		cout<<endl;
	}*/
private:
	std::vector<value_t>& _buffer;
	std::map<key_t,iter_t> _refs;
};

#endif // VECMAP_HPP_INCLUDED
