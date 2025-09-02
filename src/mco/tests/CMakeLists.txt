
if(BUILD_TESTING)
    add_executable(file_stream_tests
        file_stream_tests.cpp
    )

    target_link_libraries(file_stream_tests PRIVATE
        mco::nounit
        mco::lib
    )

    mco_target(file_stream_tests)

    add_executable(memory_stream_tests
        memory_stream_tests.cpp
    )

    target_link_libraries(memory_stream_tests PRIVATE
        mco::nounit
        mco::lib
    )

    mco_target(memory_stream_tests)

    add_executable(streaming_iterator_tests
        streaming_iterator_tests.cpp
    )

    target_link_libraries(streaming_iterator_tests PRIVATE
        mco::nounit
        mco::lib
    )

    mco_target(streaming_iterator_tests)


    # Tell CMake about the tests
    mco_nounit_add_test(file_stream_tests)
    mco_nounit_add_test(memory_stream_tests)
    mco_nounit_add_test(streaming_iterator_tests)
endif()