
add_library(mco_nounit
    nounit_impl.cpp
)

mco_target(mco_nounit)

add_library(mco::nounit ALIAS mco_nounit)

# helper function to add a nounit test to CTest
function(mco_nounit_add_test testTargetName)
    add_test(NAME ${testTargetName} COMMAND ${testTargetName})
endfunction()