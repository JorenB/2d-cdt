#include <iostream>
#include <fstream>
#include "observable.hpp"


void Observable::write() {
    std::string filename = data_dir + name + "-" + identifier + extension;

    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::app);

	assert(file.is_open());

	file << output << "\n";
	file.close();

    std::cout << filename << "\n";
}

void Observable::clear() {
    std::string filename = data_dir + name + extension;

    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::trunc);
	
	assert(file.is_open());

	file.close();
}
