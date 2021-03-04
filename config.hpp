// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <cassert>

class ConfigReader {
public:
	void read(std::string fname) {
		std::ifstream infile(fname);
		assert(infile.is_open());
		std::string key, value;

		while (infile >> key >> value) {
			dict[key] = value;
		}

		assert(dict.find("lambda") != dict.end());
		assert(dict.find("targetVolume") != dict.end());
		assert(dict.find("slices") != dict.end());
		assert(dict.find("sphere") != dict.end());
		assert(dict.find("seed") != dict.end());
		assert(dict.find("fileID") != dict.end());
		assert(dict.find("measurements") != dict.end());
		assert(dict.find("importGeom") != dict.end());
	}

	int getInt(std::string key) {
		return std::stoi(dict[key]);
	}

	double getDouble(std::string key) {
		return std::stod(dict[key]);
	}

	std::string getString(std::string key) {
		return dict[key];
	}

private:
	std::unordered_map<std::string, std::string> dict;
};
