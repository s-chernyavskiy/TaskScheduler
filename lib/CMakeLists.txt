add_library(scheduler scheduler.hpp)
add_library(any any.hpp)

set_target_properties(scheduler PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(any PROPERTIES LINKER_LANGUAGE CXX)