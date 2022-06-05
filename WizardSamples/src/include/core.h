//
// Created by mecha on 04.04.2022.
//

#pragma once

#include <core/Assert.h>

#define assert_test(tag, actual, expected, condition) \
RUNTIME_INFO("{0}: expected result: {1}, actual result: {2}", tag, expected, actual); \
RUNTIME_ASSERT(condition, actual); \
RUNTIME_INFO("{0}: test passed!", tag);               \

#define assert_equals(tag, actual, expected) assert_test(tag, actual, expected, (actual) == (expected))
#define assert_not_null(tag, actual) assert_test(tag, actual, "not null", actual)
#define assert_null(tag, actual) assert_test(tag, actual, "null", !(actual))