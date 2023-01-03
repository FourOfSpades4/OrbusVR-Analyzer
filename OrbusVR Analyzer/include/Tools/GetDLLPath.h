#pragma once

#include <string>

namespace DllPath {
	const std::string& GetPath();
	const std::string& GetParent();

	bool Initialize();
	std::string MakeRelative(std::string_view path);
}
