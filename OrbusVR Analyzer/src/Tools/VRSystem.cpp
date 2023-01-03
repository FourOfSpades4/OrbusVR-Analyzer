#include <Tools/VRSystem.h>

namespace VrSystem {
	std::string_view ToString(System system) {
		switch (system) {
		case System::None: return "None";
		case System::Oculus: return "Oculus";
		case System::Steam: return "Steam";
		default: return "Unknown";
		}
	}
}