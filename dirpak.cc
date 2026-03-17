#include <vector>
#include <filesystem>
#include <fstream>
#include <stdio.h>

#define LOG "dirpak: "

int pack(int iec, char **iev) {
	std::vector<std::filesystem::path> paths;

	for(int i = 0; i < iec; ++i) {
		if(!std::filesystem::exists(iev[i])) {
			fprintf(stderr, LOG "unable to find entry: '%s'\n", iev[i]);
			continue;
		}

		if(std::filesystem::is_regular_file(iev[i])) {
			paths.emplace_back(iev[i]);
			continue;
		}

		for(const auto &e : std::filesystem::recursive_directory_iterator(iev[i])) {
			std::filesystem::path e_path = e.path();
			if(!std::filesystem::is_regular_file(e_path)) continue;
			paths.emplace_back(std::move(e_path));
		}
	}

	if(paths.empty()) {
		fprintf(stderr, LOG "nothing to dump\n");
		return 1;
	}

	for(size_t i = 0; i < paths.size(); ++i) {
		std::ifstream efd(paths[i]);

		if(!efd) {
			fprintf(stderr, LOG "unable to open file for reading: '%s'\n", paths[i].c_str());
			continue;
		}

		fputs(paths[i].c_str(), stdout);

		std::string ln;
		while(std::getline(efd, ln)) {
			fputs("\n~ ", stdout);
			fputs(ln.c_str(), stdout);
		}

		efd.close();

		if(i + 1 < paths.size()) {
			fputs("\n\n", stdout);
		} else {
			fputs("\n", stdout);
		}
	}

	return 0;
}

int main(int argc, char **argv) {
	if(argc <= 1) { fprintf(stderr, LOG "no entries provided.\n"); return 1; }
	return pack(argc - 1, &argv[1]);
}

