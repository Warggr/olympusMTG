#ifndef STDMAP_DICTIONARY_H
#define STDMAP_DICTIONARY_H

#include <iostream>
#include <map>

template<typename T>
class Dictionary_tpl {
protected:
	std::map<const char*, T> map;
	bool is_a_copy;
public:
    typedef typename std::map<const char*, T>::const_iterator iterator;
    const iterator not_found;

	Dictionary_tpl(): is_a_copy(false), not_found(map.end()){};
	Dictionary_tpl(int nbinserts, const char* const * inserts);
	Dictionary_tpl(Dictionary_tpl const& copy): map(std::move(copy.map)), is_a_copy(true){};
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
Dictionary_tpl<T>::Dictionary_tpl(int nbinserts, const char* const* inserts): is_a_copy(false) {
    for(int i=0; i<nbinserts; i++){
        int j;
        for(j=0; inserts[i][j] != 0; j++);
        insert(inserts[i], j, static_cast<T>(i));
    }
}

using Dictionary = Dictionary_tpl<int>;

#endif //STDMAP_DICTIONARY_H
