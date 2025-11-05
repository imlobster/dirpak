#include <stdio.h>
#include <filesystem>
#include <fstream>
#include <random>
#include <string>
#include <unordered_set>

#include "pack.h"

namespace {
	enum class errors : int {
		udf,
		ok,
		invalid_path,
		invalid_name,
		cant_open_directory,
		cant_w_open_file,
		cant_r_open_file,
	};

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
		".bak", ".tmp", ".temp", ".swp", ".log", ".cache"
	};
}

int pack(const char* ipath, const char* ifilename) {
	if(!ipath) { fprintf(stderr, "dirpak: path is invalid\n"); return static_cast<int>(errors::invalid_path); }

	std::filesystem::path path(ipath);

	if(!std::filesystem::is_directory(path)) { fprintf(stderr, "dirpak: unable to open or find directory\n"); return static_cast<int>(errors::cant_open_directory); }

	std::filesystem::path write_path(
		std::filesystem::current_path() /
		(ifilename == nullptr ?
			("dir_" + std::to_string(std::random_device{}()) + ".dirpak")
		:
			std::string(ifilename)
		)
	);
	std::ofstream out_stream(write_path);

	if(!out_stream) { fprintf(stderr, "dirpak: unable to open file for writing\n"); return static_cast<int>(errors::cant_w_open_file); }

	out_stream << ipath << " dir dump w/ dirpak\n";

	for(const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path)) {
		if(!std::filesystem::is_regular_file(entry.path())) { continue; }

		std::filesystem::path entry_path = entry.path();

		std::string extension = entry_path.extension().string();
		if(extension == ".dirpak") { continue; }
		if(ignored_extensions.find(extension) != ignored_extensions.end()) {
			fprintf(stdout, "dirpak: omitting file: %s\n", entry_path.string().c_str());
			continue;
		}

		std::ifstream entry_content(entry_path);
		if(!entry_content) {
			fprintf(stderr, "dirpak: unable to open file for reading: %s\n", entry_path.string().c_str());
			out_stream.close();
			std::filesystem::remove(write_path);
			return static_cast<int>(errors::cant_r_open_file);
		}

		out_stream << '\n' << entry_path.string() << '\n';

		std::string line;
		while(std::getline(entry_content, line)) { out_stream << "~ " << line << '\n'; }

		entry_content.close();
	}

	out_stream.close();
	fprintf(stdout, "dirpak: dir dumped to: %s\n", write_path.string().c_str());
	return 0;
}
