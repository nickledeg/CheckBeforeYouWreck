#pragma once

//#define GTEST_DONT_DEFINE_ASSERT_TRUE true
//#define GTEST_DONT_DEFINE_ASSERT_FALSE true
//#define GTEST_DONT_DEFINE_EXPECT_TRUE true
//#define GTEST_DONT_DEFINE_EXPECT_FALSE true
//#define GTEST_DONT_DEFINE_ASSERT_EQ true
//#define GTEST_DONT_DEFINE_ASSERT_NE true
//#define GTEST_DONT_DEFINE_ASSERT_LE true
//#define GTEST_DONT_DEFINE_ASSERT_LT true
//#define GTEST_DONT_DEFINE_ASSERT_GE true
//#define GTEST_DONT_DEFINE_ASSERT_GT true

#include <gtest/gtest.h>

//template <typename T, typename U>
//concept SimilarTo =
//std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;
//
//#define ASSERT_TRUE(condition) \
//  do { \
//    \
//    auto const assertResult_______{(condition)}; \
//    \
//    static_assert( \
//      SimilarTo<decltype(assertResult_______), bool>, \
//      "condition given to ASSERT_TRUE is not a bool."); \
//    \
//    GTEST_TEST_BOOLEAN_( \
//      assertResult_______, #condition, false, true, GTEST_FATAL_FAILURE_); \
//  } while (false)
//
//#define ASSERT_FALSE(condition) \
//  do { \
//    \
//    auto const assertResult_______{(condition)}; \
//    \
//    static_assert( \
//      SimilarTo<decltype(assertResult_______), bool>, \
//      "condition given to ASSERT_FALSE is not a bool."); \
//    \
//    GTEST_TEST_BOOLEAN_( \
//      !assertResult_______, #condition, true, false, GTEST_FATAL_FAILURE_); \
//  } while (false)
//
//#define ASSERT_EQ(val1, val2) \
//  ASSERT_TRUE((val1) == (val2))
//
//#define ASSERT_NE(val1, val2) \
//  ASSERT_TRUE((val1) != (val2))
//
//#define ASSERT_LE(val1, val2) \
//  ASSERT_TRUE((val1) <= (val2))
//
//#define ASSERT_LT(val1, val2) \
//  ASSERT_TRUE((val1) < (val2))
//
//#define ASSERT_GE(val1, val2) \
//  ASSERT_TRUE((val1) >= (val2))
//
//#define ASSERT_GT(val1, val2) \
//  ASSERT_TRUE((val1) > (val2))
//
//#define ASSERT_ERR(VALUE, ...) \
//  ASSERT_TRUE(::test::errIs(VALUE, __VA_ARGS__))
//
//#define ASSERT_IS(MAYBE_VALUE, EXPECTED_VALUE) \
//    ASSERT_TRUE(::test::valIs(MAYBE_VALUE, EXPECTED_VALUE))
//
//#define ASSERT_NO_ERR(ERROR_HANDLE) \
//  ASSERT_TRUE(!::test::hasError(ERROR_HANDLE))
//
//#define ASSERT_UNWRAP_EQ(WRAPPED, COMPARE) \
//  ASSERT_TRUE(::test::unwrapEq(WRAPPED, COMPARE))
