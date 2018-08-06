#pragma once

#include <vector>
#include <string>
#include "TerrainMaster/Utils/File.h"
//#include "Engine/Utils/json.hpp"

namespace TM {

	//using jjson = nlohmann::json;

	class Files {
	public:
		static std::string ReadFile(File file, std::string lineSeparator = "");
		static std::vector<std::string> ReadLines(File file);

		static bool CopyFile(const File& from, const File& to);

		//static jjson LoadJson(const File& file);
		//static void SaveJson(jjson& js, const File& file);
	};
}