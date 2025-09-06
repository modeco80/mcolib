add_library(mco_base
    utils.cpp
  
)

mco_target(mco_base)

add_library(mco::base ALIAS mco_base)

# io - io library
add_subdirectory(io)

# objective - object patterns
add_subdirectory(objective)

add_subdirectory(nounit)
add_subdirectory(tests)