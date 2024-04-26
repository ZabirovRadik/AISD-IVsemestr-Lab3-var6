#pragma once
#include <iostream>
#include <deque>
#include <random>
#include <string>
#include <list>
#define LEN_WORD sizeof(size_t) * 8
#define L 11
#define A 33333

template<typename Key, typename Value, template<typename...> class Container = std::vector>
class HashTable {
	struct Pair {
		Key key;
		Value value;
		friend bool operator==(const Pair& first, const Pair& second) {
			return first.key == second.key && first.value == second.value;
		}
	};

	size_t hash(size_t key) const {
		size_t tmp = key * A;
		return ((tmp >> L) | (tmp << LEN_WORD - L));
	}

	std::vector<Container<Pair>> _data;

	Pair* get_pair(const Key& key){
		size_t ind = hash(key) % _data.size();
		for (Pair& p : _data[ind]) {
			if (p.key == key)
				return &p;
		}
		return nullptr;
	}
public:
	HashTable(size_t size) {
		if (size == 0)
			throw std::invalid_argument("size = 0?!");
		_data.resize(size);
	}

	HashTable(size_t size, int) {
		if (size == 0)
			throw std::invalid_argument("size = 0?!");
		_data.resize(size);
		for (size_t i = 0; i < size; ++i) {
			insert(rand(), rand());
		}
	}

	HashTable(const HashTable& second){
		_data = second._data;
	}

	void operator=(const HashTable& second) {
		_data = second._data;
	}

	const Pair* find(const Key& key) const{
		size_t ind = hash(key) % _data.size();
		for (const Pair& p : _data[ind]) {
			if (p.key == key)
				return &p;
		}
		return nullptr;
	}

	void insert(const Key& key, const Value& value) {
		size_t ind = hash(key) % _data.size();
		Pair new_pair;
		new_pair.key = key;
		new_pair.value = value;
		_data[ind].push_back(new_pair);
	}

	void insert_or_assigned(const Key& key, const Value& value) {
		Pair* tmp = get_pair(key);
		if (tmp)
			tmp->value = value;
		else {
			size_t ind = hash(key) % _data.size();
			Pair new_pair{ key, value };
			_data[ind].push_back(new_pair);
		}
	}

	bool contains(const Value& value) const{
		for (std::vector<Pair> v : _data) {
			for (Pair& p : v)
				if (p.value == value)
					return true;
		}
		return false;
	}

	bool erase(const Key& key) {
		size_t v_index = hash(key) % _data.size();
		for (auto iter = _data[v_index].begin(); iter != _data[v_index].end(); ++iter) {
			if (iter->key == key) {
				_data[v_index].erase(iter);
				return true;
			}
		}
		return false;
	}

	bool erase_all_occurences(const Key& key) {
		bool answer = false;
		size_t v_index = hash(key) % _data.size();
		for (auto iter = _data[v_index].begin(); iter != _data[v_index].end(); ++iter) {
				if (iter->key == key) {
					_data[v_index].erase(iter);
					answer = true;
				}
		}
		return answer;
	}

	size_t count(const Key& key) const{
		size_t ans = 0;
		for (std::vector<Pair> v : _data)
			for (const Pair& p : v)
				if (p.key == key)
					++ans;
		return ans;
	}

	void resize(size_t size) {
		if (size == 0) {
			_data.clear();
			return;
		}
		std::vector<Container<Pair>> history_data = _data;
		_data.clear();
		_data.resize(size);
		for (auto& v : history_data) {
			for (auto& p : v) {
				size_t ind = hash(p.key) % _data.size();
				_data[ind].push_back(p);
			}
		}
	}

	friend bool operator==(const HashTable& first, const HashTable& second){
		size_t v_index = 0;
		if (first._data.size() != second._data.size())
			return false;
		for (Container<Pair> v : first._data) {
			if (v.size() != second._data[v_index].size())
				return false;
			size_t p_ind = 0;
			for (const Pair& p : v) {
				auto appropriate = second._data[v_index].begin();
				std::advance(appropriate, p_ind);
				if ( p != *appropriate)
					return false;
				++p_ind;
			}
			++v_index;
		}
		return true;
	}

	friend std::ostream& operator << (std::ostream& os, const Pair* pair) {
		Key a = pair->key;
		Value b = pair->value;
		if (pair) {
			os << "key= " << a << ", value= " << b << std::endl;
		}
		else
			os << "Pair is empty!" << std::endl;
		return os;
	}

	friend std::ostream& operator<<(std::ostream& os, const HashTable& table) {
		os << "Hash Table(size= " << table._data.size() << "):" << std::endl;
		for (Container<Pair> v : table._data) {
			size_t k = 1;
			for (const Pair& p : v) {
				for (int i = 0; i < k; ++i)
					os << "\t";
				os << "key= " << p.key << ", value= " << p.value << " hash= " << table.hash(p.key) << std::endl;
				k += 1;
			}
		}
		return os;
	}
};


size_t hash_roman_to_arabish(const char& c) {
	switch (c) {
	case 'I': return 1;
	case 'V': return 5;
	case 'X': return 10;
	case 'L': return 50;
	case 'C': return 100;
	case 'D': return 500;
	case 'M': return 1000;
	default:
		throw std::invalid_argument("Ñan't convert your number from Roman to Arabic");
	}
}


size_t roman_to_arabish(const std::string& key) {
	HashTable<char, size_t, std::deque> values(key.size());
	for (char c : key) {
		values.insert(c, hash_roman_to_arabish(c));
	}
	size_t answer = 0;
	size_t previous = 0;
	for (size_t i = key.size(); i > 0; --i) {
		size_t in_arabish = values.find(key[i - 1])->value;
		if (in_arabish >= previous)
			answer += in_arabish;
		else
			answer -= in_arabish;
		previous = in_arabish;
	}
	return answer;
}
