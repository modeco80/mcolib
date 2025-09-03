add_library(mcolib
    utils.cpp
    # anti-iostreams bikeshedding
    io/stream.cpp
    io/stream_utils.cpp
    io/writable_stream.cpp
    io/file_stream.cpp
    io/memory_stream.cpp
)

mco_target(mcolib)

add_library(mco::lib ALIAS mcolib)

add_subdirectory(nounit)
add_subdirectory(tests)