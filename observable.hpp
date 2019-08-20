// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef OBSERVABLE_HPP_
#define OBSERVABLE_HPP_

#include <chrono>
#include <string>
#include "universe.hpp"

class Observable {
public:
	std::string name;

	Observable() {
		identifier = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	}

	void measure() {
		process();
		write();
	}

	void clear();

private:
	std::string identifier;

protected:
	virtual void process() = 0;

	void write();

	std::string data_dir = "out/";
	std::string extension = ".dat";
	std::string output;
};
#endif  // OBSERVABLE_HPP_
