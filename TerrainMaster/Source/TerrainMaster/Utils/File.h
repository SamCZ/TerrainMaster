#pragma once

#include <string>
#include <vector>

namespace TM {
	class File {
	public:
		~File();
		File();
		File(const char* file);
		File(std::string file);
		File(File parent, std::string file);

		std::string GetPath() const;
		std::string GetParent() const;
		File GetParentFile() const;

		bool IsExist() const;
		bool IsDirectory() const;
		bool Mkdirs() const;
		bool Delete() const;

		std::string GetName() const;
		std::string GetCleanName() const;
		std::string GetExtension(bool lower = true) const;
		bool HaveExtension(std::string ext);

		std::vector<File> ListFiles(bool recursive = false);

		static std::string FixPath(std::string path, const char from = '\\', const char to = '/');
	private:
		std::string _rootPath;
	};

	std::ostream& operator<<(std::ostream& os, const File& obj);
	bool operator==(const File& left, const File& right);
	bool operator!=(const File& left, const File& right);
}