#pragma once 

#ifdef UNIX
    #define platform "unix"

    #include <dlfcn.h>
    #include <bits/stdc++.h>

#elif WIN32
    #define platform "win32"

    #include <windows.h>
#endif
