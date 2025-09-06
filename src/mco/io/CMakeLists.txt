add_library(mco_io
    # anti-iostreams bikeshedding
    stream.cpp
    stream_utils.cpp
    writable_stream.cpp

    # concrete stream implementations
    file_stream.cpp
    memory_stream.cpp
)

mco_target(mco_io)
target_link_libraries(mco_io PUBLIC
    mco::base
)

add_library(mco::io ALIAS mco_io)