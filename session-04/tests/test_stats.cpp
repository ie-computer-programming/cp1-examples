/* test_stats.cpp — GTest.
 *
 * GTest is C++, so this file is .cpp and the project must enable CXX.
 *
 * stats.h declares C functions. A C++ compiler MANGLES function names (it
 * encodes the argument types into the symbol) so that overloading can work.
 * C does not. Without the extern "C" below, this file compiles fine and then
 * fails at LINK time with:
 *
 *     undefined reference to `stats_sum(int const*, unsigned long, int*)'
 *
 * -- note the argument types in the message: that is the mangled name.
 *
 * Two ways to fix it:
 *   1. wrap the include, as below -- use this when you do not own the header
 *      (your HW1 `cp1.h` is a frozen contract, so this is the one you need);
 *   2. put  #ifdef __cplusplus / extern "C" {  guards inside the header
 *      itself -- only possible when the header is yours to change.
 */
#include <gtest/gtest.h>

extern "C" {
#include "stats.h"
}

#include <climits>

TEST(Sum, AddsValues)
{
    const int values[] = {1, 2, 3, 4};
    int out = 0;
    ASSERT_EQ(stats_sum(values, 4, &out), 0);
    EXPECT_EQ(out, 10);
}

TEST(Sum, EmptyRangeIsZero)
{
    const int values[] = {1};
    int out = 99;
    ASSERT_EQ(stats_sum(values, 0, &out), 0);
    EXPECT_EQ(out, 0);
}

TEST(Sum, ReportsOverflowInsteadOfWrapping)
{
    const int values[] = {INT_MAX, 1};
    int out = 42;
    EXPECT_EQ(stats_sum(values, 2, &out), -1);
    EXPECT_EQ(out, 42) << "a failed call must leave the output untouched";
}

TEST(Mean, DividesAsFloatingPoint)
{
    const int values[] = {1, 2};
    double out = 0.0;
    ASSERT_EQ(stats_mean(values, 2, &out), 0);
    EXPECT_DOUBLE_EQ(out, 1.5) << "1+2 over 2 is 1.5, not 1";
}

TEST(Mean, EmptyIsAnError)
{
    const int values[] = {1};
    double out = 7.5;
    EXPECT_EQ(stats_mean(values, 0, &out), -1);
    EXPECT_DOUBLE_EQ(out, 7.5);
}

TEST(Max, FindsLargest)
{
    const int values[] = {-4, 9, 2};
    int out = 0;
    ASSERT_EQ(stats_max(values, 3, &out), 0);
    EXPECT_EQ(out, 9);
}

TEST(Max, AllNegative)
{
    const int values[] = {-4, -9, -2};
    int out = 0;
    ASSERT_EQ(stats_max(values, 3, &out), 0);
    EXPECT_EQ(out, -2);
}

#ifdef SHOW_FAILURE
/* Build with -DSHOW_FAILURE to see what a failing test looks like. */
TEST(Sum, DeliberatelyWrong)
{
    const int values[] = {1, 2, 3, 4};
    int out = 0;
    ASSERT_EQ(stats_sum(values, 4, &out), 0);
    EXPECT_EQ(out, 11);
}
#endif
