#include <gtest/gtest.h>
#include <type_traits>
#include <string>
#include "type_traits.hpp"

TEST(TypeTraitsTest, RemoveReference) {
    ASSERT_TRUE((std::is_same<gstd::remove_reference_t<int&>, int>::value));
    ASSERT_TRUE((std::is_same<gstd::remove_reference_t<int&&>, int>::value));
    ASSERT_TRUE((std::is_same<gstd::remove_reference_t<int>, int>::value));
}

TEST(TypeTraitsTest, Move) {
    int x = 10;

    ASSERT_TRUE((std::is_same<decltype(gstd::move(x)), int&&>::value));
    ASSERT_TRUE((std::is_same<decltype(gstd::move(std::move(x))), int&&>::value));
    ASSERT_TRUE((std::is_same<decltype(std::move(gstd::move(x))), int&&>::value));
}

TEST(TypeTraitsTest, Forward) {
    std::string x = "Hello";

    ASSERT_TRUE((std::is_same<decltype(gstd::forward<std::string&>(x)), std::string&>::value));
    ASSERT_TRUE((std::is_same<decltype(gstd::forward<std::string>(gstd::move(x))), std::string&&>::value));
    ASSERT_TRUE((std::is_same<decltype(gstd::forward<std::string>("Hello")), std::string&&>::value));
}   
