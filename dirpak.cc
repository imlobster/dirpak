#include <vector>
#include <filesystem>
#include <fstream>
#include <stdio.h>

#define LOG "dirpak: "
#define MARK "dump w/ dirpak"

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

	fprintf(stdout, MARK "\n");

	for(const auto& path : paths) {
		std::ifstream efd(path);

		if(!efd) {
			fprintf(stderr, LOG "unable to open file for reading: '%s'\n", path.c_str());
			return 1;
		}

		fprintf(stdout, "\n%s\n", path.c_str());

		std::string ln;
		while(std::getline(efd, ln)) { fprintf(stdout, "~ %s\n", ln.c_str()); }

		efd.close();
	}

	return 0;
}

int main(int argc, char **argv) {
	if(argc <= 1) { fprintf(stderr, LOG "no entries provided.\n"); return 1; }
	return pack(argc - 1, &argv[1]);
}

