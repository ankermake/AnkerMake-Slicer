__install_inc_directory(${CMAKE_CURRENT_SOURCE_DIR}/../boost)

set(boost_includes ${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${boost_includes})
add_definitions(-DBOOST_ALL_DYN_LINK)

__add_all_directory()