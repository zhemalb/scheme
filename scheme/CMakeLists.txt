file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "*.cpp")
add_library(libscheme ${SRC})
target_include_directories(libscheme PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
