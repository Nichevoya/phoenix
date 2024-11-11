cmake_minimum_required(VERSION 3.27.4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("Phoenix")

# ------------------------------ phoenix steps ----------------------------- 

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_MODE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Validation")
    add_definitions(-DVALIDATION_MODE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DRELEASE_MODE)
endif()

# ------------------------------ configuration ----------------------------- 

if (UNIX)
    add_definitions(-DUNIX)
    
    set(COMPILATION_FLAGS
        -std=c++20 -fPIC 
        -Wall -Wextra -Werror -Wpedantic -g 
    )

elseif(WIN32)
    add_definitions(-DWIN32)

    set(COMPILATION_FLAGS
        /std:c++20 
        /W4 /Zi 
    )

endif()

set(INCLUDE_DIRECTORIES 
    "./source/"
    "./source/engine"
    "./source/experimental"
)

set(LIBRARY_LINK_FLAGS
    phoenixStatic 
)

file(GLOB_RECURSE HEADERS "./source/*.hpp")
file(GLOB_RECURSE SOURCES "./source/*.cpp")

add_library(phoenixStatic STATIC 
    ${HEADERS}
    ${SOURCES}
)

add_library(phoenixShared SHARED 
    ${HEADERS}
    ${SOURCES}
)

# ---------------------------- compilation flags --------------------------- 

target_compile_options(phoenixStatic PRIVATE 
    ${COMPILATION_FLAGS}
)

target_compile_options(phoenixShared PRIVATE 
    ${COMPILATION_FLAGS}
)

# ----------------------------- link libraries ----------------------------- 

target_link_libraries(phoenixShared PRIVATE 
    ${LIBRARY_LINK_FLAGS}
)

# ----------------------------- header include ----------------------------- 

target_include_directories(phoenixStatic PRIVATE 
    ${INCLUDE_DIRECTORIES}
)

target_include_directories(phoenixShared PRIVATE 
    ${INCLUDE_DIRECTORIES}
)

# ---------------------------- target properties --------------------------- 

set_target_properties(phoenixStatic phoenixShared PROPERTIES 
    OUTPUT_NAME "phoenix"
    LINKER_LANGUAGE CXX
)
