#pragma once

/* All the includes are just shoved in here because why not. */

#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <vector>
#include <future>
#include <sstream>
#include <sysinfoapi.h>
#include <thread>
#include <winternl.h>
#include <fstream>
#include <ctime>
#include <sddl.h>
#include <stdio.h>
#include <string>
#include <locale.h>
#include <tchar.h>
#include <lmcons.h>
#include <psapi.h>
#include <strsafe.h>
#include <random>
#include <filesystem>

#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "d3d9.lib")

#include <d3d9.h>
#include "imgui.h" /* Include Dear ImGui */
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include "imgui_internal.h"

namespace marty {
	namespace gui {
		inline HWND hwnd{};
	}
}