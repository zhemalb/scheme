add_catch(test_scheme
  test_boolean.cpp
  test_control_flow.cpp
  test_eval.cpp
  test_integer.cpp
  test_lambda.cpp
  test_list.cpp
  test_symbol.cpp
)
target_link_libraries(test_scheme PRIVATE libscheme)
target_include_directories(test_scheme PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_catch(test_scheme_parser test_parser.cpp)
target_link_libraries(test_scheme_parser PRIVATE libscheme)

add_catch(test_scheme_tokenizer test_tokenizer.cpp)
target_link_libraries(test_scheme_tokenizer PRIVATE libscheme)
