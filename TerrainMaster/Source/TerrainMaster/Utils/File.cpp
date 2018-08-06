#include "File.h"

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
//#include "boost/filesystem.hpp"
#include <algorithm>

namespace TM {
	File::~File() {

	}

	File::File() : File("Temp") {

	}

	File::File(const char* file) : File(std::string(file)) {

	}

	File::File(std::string file) {
		_rootPath = FixPath(file);
	}

	File::File(File parent, std::string file) : File(parent) {
		std::string fixedFile = FixPath(file);
		if (_rootPath[_rootPath.length() - 1] != '/') {
			_rootPath += '/';
		}
		_rootPath += fixedFile;
	}

	std::string File::FixPath(std::string path, const char from, const char to) {
		for (int i = 0; i < path.length(); i++) {
			if (path[i] == from) {
				path[i] = to;
			}
		}
		return path;
	}

	std::string File::GetPath() const {
		return _rootPath;
	}

	std::string File::GetParent() const {
		return _rootPath.substr(0, _rootPath.find_last_of('/'));
	}

	File File::GetParentFile() const {
		return File(GetParent());
	}

	bool File::IsExist() const {
		struct stat buffer;
		return (stat(_rootPath.c_str(), &buffer) == 0);
	}

	bool File::Mkdirs() const {
		//return boost::filesystem::create_directories(getPath());
		return false;
	}

	bool File::Delete() const {
		if (GetExtension() != "meta") {
			File metaFile(GetPath() + ".meta");
			if (metaFile.IsExist()) {
				metaFile.Delete();
			}
		}
		/*if (boost::filesystem::exists(getPath()))
			return boost::filesystem::remove(getPath());*/
		return false;
	}

	bool File::IsDirectory() const {
		struct stat info;
		if (stat(_rootPath.c_str(), &info) != 0)
			return 0;
		else if (info.st_mode & S_IFDIR)
			return 1;
		else
			return 0;
	}

	std::string File::GetName() const {
		std::string path = _rootPath;
		if (_rootPath.find_last_of('/') == _rootPath.length() - 1) {
			path = _rootPath.substr(0, _rootPath.find_last_of('/'));
		}
		return _rootPath.substr(_rootPath.find_last_of('/') + 1);
	}

	std::string File::GetCleanName() const {
		std::string name = GetName();
		int dotIndex = name.find_last_of('.');
		if (dotIndex > 0) {
			return name.substr(0, dotIndex);
		}
		else {
			return name;
		}
	}

	std::string File::GetExtension(bool lower) const {
		std::string name = GetName();
		std::string ext = name.substr(name.find_last_of('.') + 1);
		if (lower) {
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		}
		return ext;
	}

	bool File::HaveExtension(std::string ext) {
		return GetExtension() == ext;
	}

	std::vector<File> File::ListFiles(bool recursive) {
		std::vector<File> files;
		if (!IsDirectory()) return files;

		/*namespace fs = boost::filesystem;

		fs::path apk_path(getPath());
		if (recursive) {
			fs::recursive_directory_iterator end;

			for (fs::recursive_directory_iterator i(apk_path); i != end; ++i) {
				const fs::path cp = (*i);
				files.push_back(File(cp.string()));
			}
		}
		else {
			fs::directory_iterator end;

			for (fs::directory_iterator i(apk_path); i != end; ++i) {
				const fs::path cp = (*i);
				files.push_back(File(cp.string()));
			}
		}*/

		return files;
	}


	std::ostream& operator<<(std::ostream& os, const File& obj) {
		os << obj.GetPath();
		return os;
	}

	bool operator==(const File& left, const File& right) {
		return left.GetPath() == right.GetPath();
	}

	bool operator!=(const File& left, const File& right) {
		return !(left == right);
	}
}