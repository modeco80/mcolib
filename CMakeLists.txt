cmake_minimum_required(VERSION 3.10..4.69)


# Prohibit in-source tree builds.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are strictly prohibited.")
endif()

project(mcolib
        VERSION 0.1.0
        LANGUAGES C CXX
        DESCRIPTION "my common utils"
        )

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(CTest)

include(Policies)
include(ProjectFuncs)
include(CompilerFlags)

option(MCO_WITH_ZLIB "Enable zlib features" OFF)

if(MCO_WITH_ZLIB)
    # dependencies
    find_package(ZLIB)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(src/mco)