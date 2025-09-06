
add_library(mco_objective
    service_bucket.cpp
)

mco_target(mco_objective)

add_library(mco::objective ALIAS mco_objective)