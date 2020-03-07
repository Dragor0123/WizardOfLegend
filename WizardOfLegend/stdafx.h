// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <crtdbg.h>
#include <time.h>
#include <conio.h>

//crtdbg랑 conio.h 는 최종본에서 빼자

// TODO: reference additional headers your program requires here

#pragma comment(lib, "msimg32")

#include <vector>
#include <array>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <string>
#include <cassert>
#include <stack>
#include <random>

// Sound 관련
#include <io.h>
#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")

using namespace std;
#include "Include.h"