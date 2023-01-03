#pragma once

#include <string_view>

namespace VrSystem {
	enum System {
		None = -1,
		Oculus = 0,
		Steam = 1,
	};

	std::string_view ToString(System system);
}
