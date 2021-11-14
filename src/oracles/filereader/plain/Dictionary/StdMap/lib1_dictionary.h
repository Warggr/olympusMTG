#ifndef STDMAP_DICTIONARY_H
#define STDMAP_DICTIONARY_H

#include <iostream>
#include <map>
#include <functional>
#include <cstring>

//adapted from https://stackoverflow.com/questions/4157687/using-char-as-a-key-in-stdmap
struct StrCompare : public std::binary_function<const char*, const char*, bool> {
public:
    bool operator() (const char* str1, const char* str2) const
    { return std::strcmp(str1, str2) < 0; }
};

template<typename T>
class Dictionary_tpl {
protected:
	std::map<const char*, T, StrCompare> map;
	bool is_a_copy;
public:
    class iterator {
    	typedef std::map<const char *, T>::const_iterator underlying;
        underlying x;
    public:
    	constexpr iterator(underlying x): x(x) {};
        T operator*() const { return x->second; }
        bool operator==(const iterator& other) const { return x == other.x; }
    };
    const iterator not_found;

	Dictionary_tpl(): is_a_copy(false), not_found(map.end()){};
	Dictionary_tpl(int nbinserts, const char* const * inserts);
	Dictionary_tpl(Dictionary_tpl const& copy): map(std::move(copy.map)), is_a_copy(true), not_found(map.end()){};
	~Dictionary_tpl() = default;

	iterator find(const char* key) const {
		return map.find(key);
	};

	void insert(const char* key, int, T id){
		map.insert({key, id});
	};

	//void show() const { if(map.empty()) std::cout << "No values!\n"; else for(auto& i : map) std::cout << "[" <<  };
};

template<typename T>
Dictionary_tpl<T>::Dictionary_tpl(int nbinserts, const char* const* inserts): is_a_copy(false), not_found(map.end()) {
    for(int i=0; i<nbinserts; i++){
        int j;
        for(j=0; inserts[i][j] != 0; j++);
        insert(inserts[i], j, static_cast<T>(i));
    }
}

using Dictionary = Dictionary_tpl<int>;

#endif //STDMAP_DICTIONARY_H
