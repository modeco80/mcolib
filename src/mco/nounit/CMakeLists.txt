
add_library(mco_nounit
    assertion_format.cpp
    test.cpp
    test_runner.cpp
    main_impl.cpp
)

mco_target(mco_nounit)

add_library(mco::nounit ALIAS mco_nounit)

# helper function to add a nounit test to CTest
function(mco_nounit_add_test testTargetName)
    add_test(NAME ${testTargetName} COMMAND ${testTargetName})
endfunction()