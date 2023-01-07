#pragma once

#include <winsock2.h>
#include <Minhook.h>
#include <cstdio>
#include <fstream>
#include <string>
#include <stdint.h>
#include <iostream>

#include "Scoundrel.h"

extern uintptr_t gameAssembly;

namespace Hooks {
	void Init(int version);
}