#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
#include "gtest/gtest.h"

template <typename Sequence, typename Predicate>
Sequence trim(Sequence const& input, Predicate pred) {
    auto left = std::find_if_not(input.begin(), input.end(), pred);
    auto right = std::find_if_not(input.rbegin(), input.rend(), pred);

    if (left < right.base()) {
        return Sequence(left, right.base());
    }
    else if (std::all_of(input.begin(), input.end(), pred)) {
        return Sequence();
    }
    else {
        return input;
    }
}

TEST(TrimTest, TrimmingStringWithWhitespacePredicate) {
    std::string str = "              przykładowy string              ";
    auto trimmed = trim(str, [](char c) { return std::isspace(c); });
    ASSERT_EQ(trimmed, "przykładowy string") << "Test: TrimmingStringWithWhitespacePredicate failed";
}

TEST(TrimTest, TrimmingVectorWithZeroPredicate) {
    std::vector<int> vec = { 0, 0, 1, 2, 3, 0, 0 };
    auto trimmed = trim(vec, [](int i) { return i == 0; });
    std::vector<int> expected = { 1, 2, 3 };
    ASSERT_EQ(trimmed, expected) << "Test: TrimmingVectorWithZeroPredicate failed";
}

TEST(TrimTest, TrimmingStringWithEmptyPredicate) {
    std::string str = "hello world";
    auto trimmed = trim(str, [](char c) { return false; });
    ASSERT_EQ(trimmed, str) << "Test: TrimmingStringWithEmptyPredicate failed";
}

TEST(TrimTest, TrimmingEmptyVectorWithZeroPredicate) {
    std::vector<int> vec;
    auto trimmed = trim(vec, [](int i) { return i == 0; });
    ASSERT_TRUE(trimmed.empty()) << "Test: TrimmingEmptyVectorWithZeroPredicate failed";
}

TEST(TrimTest, TrimmingVectorWithZeroPredicateReturnsOriginalVector) {
    std::vector<int> vec = { 1, 2, 3 };
    auto trimmed = trim(vec, [](int i) { return i == 0; });
    ASSERT_EQ(trimmed, vec) << "Test: TrimmingVectorWithZeroPredicateReturnsOriginalVector failed";
}

TEST(TrimTest, TrimmingVectorWithAllElementsAsZeroPredicateReturnsEmptyVector) {
    std::vector<int> vec = { 0, 0, 0, 0, 0 };
    auto trimmed = trim(vec, [](int i) { return i == 0; });
    ASSERT_TRUE(trimmed.empty()) << "Test: TrimmingVectorWithAllElementsAsZeroPredicateReturnsEmptyVector failed";
}

TEST(TrimTest, TrimmingStringWithAllCharactersBeingWhitespacesReturnsEmptyString) {
    std::string str = "     ";
    auto trimmed = trim(str, [](char c) { return std::isspace(c); });
    ASSERT_TRUE(trimmed.empty()) << "Test: TrimmingStringWithAllCharactersBeingWhitespacesReturnsEmptyString failed";
}

TEST(trim, test_only_first_char_whitespace) {
    std::string str = "  hello world";
    auto trimmed = trim(str, [](char c) { return std::isspace(c); });
    EXPECT_EQ(trimmed, "hello world");
}

TEST(trim, test_only_last_char_whitespace) {
    std::string str = "hello world  ";
    auto trimmed = trim(str, [](char c) { return std::isspace(c); });
    EXPECT_EQ(trimmed, "hello world");
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}