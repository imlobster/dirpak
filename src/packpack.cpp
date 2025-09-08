#include "packpack.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <random>
#include <unordered_set>

namespace packpack {
	static const std::unordered_set<std::string> ignored_extensions {
		// Archives
		".zip", ".rar", ".7z", ".tar", ".gz", ".bz2", ".xz", ".zst", ".cab", ".iso",
		// Audio
		".mp3", ".wav", ".flac", ".ogg", ".aac", ".m4a", ".wma",
		// Media
		".mp4", ".avi", ".mkv", ".mov", ".webm", ".flv", ".3gp",
		// Images
		".png", ".jpg", ".jpeg", ".gif", ".bmp", ".tiff", ".ico", ".webp", ".raw",
		// Docs
		".pdf", ".doc", ".docx", ".xls", ".xlsx", ".ppt", ".pptx",
		".odt", ".ods", ".odp",
		// Fonts
		".ttf", ".otf", ".woff", ".woff2",
		// Executables and binaries
		".exe", ".dll", ".bin", ".so", ".dylib", ".apk", ".msi",
		".deb", ".rpm", ".pkg", ".sys", ".pdb", ".obj",
		// Temp
		".bak", ".tmp", ".swp", ".log", ".cache",
		// Itself
		".dirpak"
	};

	int pack(const char* relative_path) {
		if(!relative_path) {
			std::cerr<<"dirpak: path was losted"<<std::endl;
			return 1;
		}
		if(!std::filesystem::is_directory(relative_path)) {
			std::cerr<<"dirpak: can't open or find directory"<<std::endl;
			return 2;
		}
		std::filesystem::path out_path=(std::filesystem::current_path()/("dir_"+std::to_string(std::random_device{}())+".dirpak"));
		std::ofstream out_file(out_path);

		if(!out_file) {
			std::cerr<<"dirpak: can't open file "<<out_path.string()<<std::endl;
			return 3;
		}

		out_file<<relative_path<<" dir dump w/ dirpak\n";

		for(const auto &entry : std::filesystem::recursive_directory_iterator(relative_path)) {
			if(!std::filesystem::is_regular_file(entry.path())) continue;

			auto ext=entry.path().extension().string();
			if(ext==".dirpak") continue;
			if(ignored_extensions.find(ext)!=ignored_extensions.end()) {
				std::cout<<"dirpak: bad file "<<entry.path().string()<<std::endl;
				continue;
			}

			std::ifstream input_file(entry.path());
			if(!input_file.is_open()) {
				std::cerr<<"dirpak: can't open file "<<entry.path()<<std::endl;
				out_file.close();
				std::filesystem::remove(out_path);
				return 4;
			}

			out_file<<"\n"<<entry.path().string()<<"\n";

			std::string line;
			while(std::getline(input_file, line)) {
				out_file<<"~ "<<line<<"\n";
			}

			input_file.close();
		}

		out_file.close();
		std::cout<<"dirpak: dir dumped to "<<out_path.string()<<std::endl;
		return 0;
	}
}
