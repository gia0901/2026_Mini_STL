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

TEST(TypeTraitsTest, Swap) {
    int x = 10;
    int y = 20;
    gstd::swap(x, y);
    ASSERT_EQ(x, 20);
    ASSERT_EQ(y, 10);

    std::string str1 = "Hello";
    std::string str2 = "World";
    gstd::swap(str1, str2);
    ASSERT_EQ(str1, "World");
    ASSERT_EQ(str2, "Hello");
}

TEST(TypeTraitsTest, Pair) {
    std::string str1 = "Hello";
    std::string str2 = "World";
    gstd::pair<std::string, std::string> p1(str1, str2);

    ASSERT_EQ(p1.first, "Hello");
    ASSERT_EQ(p1.second, "World");
    
    gstd::pair<std::string, std::string> p2(gstd::move(str1), gstd::move(str2));

    ASSERT_EQ(p2.first, "Hello");
    ASSERT_EQ(p2.second, "World");

    gstd::pair<int, std::string> p3(1, "Hello");
    ASSERT_EQ(p3.first, 1);
    ASSERT_EQ(p3.second, "Hello");
}

TEST(TypeTraitsTest, PairSwap) {
    int a = 10, b = 20;
    int c = 30, d = 40;

    gstd::pair<int, int> p1(a, b);
    gstd::pair<int, int> p2(c, d);

    p1.swap(p2);
    ASSERT_EQ(p1.first, 30);
    ASSERT_EQ(p1.second, 40);
    ASSERT_EQ(p2.first, 10);
    ASSERT_EQ(p2.second, 20);
}