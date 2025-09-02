# Core compile arguments used for the whole project
#
# This is the driver, we include compiler/platform specific files here

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	include(CompilerFlags-GNU)
else()
	message(WARNING "Unsupported (for now?) compiler ${CMAKE_CXX_COMPILER_ID}. Configuration will continue,"
				    "however this configuration may break at any time.")
endif()
