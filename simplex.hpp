// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef SIMPLEX_HPP_
#define SIMPLEX_HPP_

class Simplex {
public:
	int key() const { return key_; }  // Getter
	void key(int key__) { key_ = key__; }  // Setter

private:
	int key_;  // Label or next free entry in pool
};

#endif  // SIMPLEX_HPP_
