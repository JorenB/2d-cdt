// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef simplex_hpp
#define simplex_hpp

class Simplex {
public:
	int key() const { return key_; }  // Getter
	int key(int key__) {  // Setter
		auto tmp = key_;
		key_ = key__;
		return tmp;
	}

private:
	int key_;  // Label or next free entry in pool
};

#endif
