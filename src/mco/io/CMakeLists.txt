add_library(mco_io
    # anti-iostreams bikeshedding
    stream.cpp
    stream_utils.cpp
    writable_stream.cpp

    # concrete stream implementations
    file_stream.cpp
    memory_stream.cpp
)

if(ZLIB_FOUND)
    message(STATUS "mco::io - Enabling gzip")
    target_sources(mco_io PRIVATE
        gzip_in_stream.cpp
    )

    target_link_libraries(mco_io PUBLIC
        ZLIB::ZLIB
    )
endif()

mco_target(mco_io)
target_link_libraries(mco_io PUBLIC
    mco::base
)

add_library(mco::io ALIAS mco_io)