#pragma once

#ifdef UNIX
    #define platform "unix"

#elif WIN32
    #define platform "win32"

#endif

/* ----------------------------------- std ---------------------------------- */

#include "std.hpp"

/* --------------------------------- string --------------------------------- */

#include "string.hpp"

/* -------------------------------- integral -------------------------------- */

#include "number.hpp"
#include "random.hpp"

/* --------------------------------- system --------------------------------- */

#include "experimental/file.hpp"
#include "input.hpp"
#include "library.hpp"
#include "log.hpp"
#include "plugin.hpp"

/* --------------------------------- memory --------------------------------- */

#include "experimental/unique_void_ptr.hpp"

/* --------------------------------- engine --------------------------------- */

#include "engine/coordinator.hpp"
