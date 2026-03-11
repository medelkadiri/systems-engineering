cmake_minimum_required(VERSION 3.16)
project(systems_engineering
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "C++17 systems mini projects"
)

# -----------------------------------------------------------------------------
# C++ standard used
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------------------------------------------------------
# Build type: default Release for single config generators if not set
# -----------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# -----------------------------------------------------------------------------
# Tooling: compile_commands.json for clangd, etc.
# -----------------------------------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------------------
# Options
# -----------------------------------------------------------------------------
option(BUILD_TESTS "Build and register tests" ON)
if(BUILD_TESTS)
    enable_testing()
endif()

# -----------------------------------------------------------------------------
# Compiler warnings (GCC/Clang  all targets inherit via subdirs)
# -----------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# -----------------------------------------------------------------------------
# Subprojects
# -----------------------------
set(SUBPROJECTS fd_wrapper)

foreach(sub ${SUBPROJECTS})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/projects/${sub}/CMakeLists.txt")
        add_subdirectory(projects/${sub})
    endif()
endforeach()
