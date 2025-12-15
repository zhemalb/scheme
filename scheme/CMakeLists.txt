set(SRC scheme.cpp)
file(GLOB_RECURSE SRC_SUB CONFIGURE_DEPENDS
  "core/*.cpp"
  "runtime/*.cpp"
  "reader/*.cpp"
  "builtins/*.cpp"
  "util/*.cpp"
)
list(APPEND SRC ${SRC_SUB})

add_library(libscheme ${SRC})
target_include_directories(libscheme PUBLIC . core runtime reader builtins util)

# scheme-repl is not used for testing,
# but you may use it if you need to experiment with the code.
add_executable(scheme-repl repl/main.cpp)
target_link_libraries(scheme-repl libscheme)

file(GLOB SRC_TEST CONFIGURE_DEPENDS "tests/*.cpp")
add_catch(test_scheme ${SRC_TEST})
target_link_libraries(test_scheme PRIVATE libscheme)
