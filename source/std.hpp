#pragma once

/* ----------------------------------- std ---------------------------------- */

#ifdef UNIX
    #define platform "unix"

    #include <dlfcn.h>
    #include <bits/stdc++.h>

#elif WIN32
    #define platform "win32"

    #include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <regex>

#include <assert.h>

#include <filesystem>

#include <typeinfo>
#include <bitset>
#include <set>

#include <memory>
#include <thread>
#include <mutex>

#include <chrono>
#include <random>

#include <unordered_map>
#include <vector>
#include <array>
#include <queue>
#include <map>
