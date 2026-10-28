add_executable(quant_tests test_analytics.cpp)
target_include_directories(quant_tests SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/third_party/doctest")
target_link_libraries(quant_tests PRIVATE quant)
add_test(NAME quant_unit_tests COMMAND quant_tests)
