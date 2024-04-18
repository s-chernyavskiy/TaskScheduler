#include <gtest/gtest.h>

#include <lib/any.hpp>

TEST(LUVABL_ANY_TESTS, INIT_TEST) {
  luvabl::any a = 2;
  ASSERT_EQ(luvabl::any_cast<int>(a), 2);
}

TEST(LUVABL_ANY_TESTS, INIT_FROM_OTHER_ANY_TEST) {
  luvabl::any a = luvabl::any(2);

  ASSERT_EQ(luvabl::any_cast<int>(a), 2);
}

TEST(LUVABL_ANY_TESTS, COMPARE_ANY_TEST) {
  luvabl::any a = 2;
  luvabl::any b = 2;
  ASSERT_EQ(luvabl::any_cast<int>(b), luvabl::any_cast<int>(a));
}

TEST(LUVABL_ANY_TESTS, ANY_THROW_TEST) {
  luvabl::any a = 2;

  ASSERT_THROW(luvabl::any_cast<float>(a), luvabl::bad_any_cast);
}
