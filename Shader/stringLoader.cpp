#include <iostream>

#include <string>
#include <fstream>

#include "stringLoader.hpp"

std::string load(const std::string path) {
	std::fstream stream(path, std::fstream::in);
	if (!stream) {
		std::cerr << "Failed to load the filepath " << path << "." << std::endl;
		getchar();
		return NULL;
	}

	std::string finalString;
	
	std::getline(stream, finalString, '\0');

	stream.close();
	return finalString;
}