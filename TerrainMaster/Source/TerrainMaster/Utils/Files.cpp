#include "Files.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cstring>
#include <io.h>
#include <stdio.h>
//#include "boost/filesystem.hpp"

namespace TM {
	std::string Files::ReadFile(File file, std::string lineSeparator) {
		std::ifstream stream(file.GetPath());

		std::string content = "";
		std::string line;
		if (stream.is_open()) {
			while (std::getline(stream, line)) {
				content += line + lineSeparator;
			}
			stream.close();
		}
		return content;
	}

	std::vector<std::string> Files::ReadLines(File file) {
		std::vector<std::string> lines;
		std::ifstream stream(file.GetPath());
		std::string line;
		if (stream.is_open()) {
			while (std::getline(stream, line)) {
				lines.push_back(line);
			}
			stream.close();
		}
		return lines;
	}

	bool Files::CopyFile(const File& from, const File& to) {
		//boost::filesystem::copy_file(from.GetPath(), to.GetPath());
		return true;
	}

	/*jjson Files::LoadJson(const File& file) {
		std::ifstream i(file.getPath());
		if (!i.is_open()) {
			return NULL;
		}
		jjson j;
		i >> j;
		i.close();
		return j;
	}

	void Files::SaveJson(jjson& js, const File& file) {
		std::ofstream o(file.getPath());
		o << std::setw(4) << js << std::endl;
		o.close();
	}*/
}