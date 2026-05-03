set(UNIT_TEST_SRCS
    "unit_tests/test_type_traits.cpp"
)

set(INTEGRATION_TEST_SRCS
)

set(TEST_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/include/gstd/utility/"
    "${CMAKE_SOURCE_DIR}/include/gstd/"
)

set(GTEST_ESSENTIALS
    "GTest::gtest"
    "GTest::gtest_main"
)

# Unit tests
add_executable(unit_test ${UNIT_TEST_SRCS})
target_include_directories(unit_test PRIVATE ${TEST_INCLUDE_DIRS})
target_link_libraries(unit_test PRIVATE ${GTEST_ESSENTIALS})

# Integration tests