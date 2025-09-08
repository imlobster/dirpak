#include "packpack.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
	if(argc <= 1) {
		std::cout << "dirpak: no path was provided" << std::endl;
		return 1;
	} else if(argc >= 3) {
		std::cout << "dirpak: only first argument will be used" << std::endl;
	}

	return packpack::pack(argv[1]);
}
