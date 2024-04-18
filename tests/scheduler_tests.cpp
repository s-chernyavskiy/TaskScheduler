#include <gtest/gtest.h>

#include <lib/scheduler.hpp>

TEST(LUVABL_SCHEDULER_TESTS, INIT_TEST) {
  luvabl::TTaskScheduler scheduler;

  ASSERT_NO_THROW(scheduler);
}

TEST(LUVABL_SCHEDULER_TESTS, ADD_TASK_TEST) {
  luvabl::TTaskScheduler scheduler;
  auto id1 = scheduler.add([](int a, int b) { return a + b; }, 1, 2);
  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<int>(id1), 3);
}

TEST(LUVABL_SCHEDULER_TESTS, GET_FUTURE_RESULT_TEST) {
  luvabl::TTaskScheduler scheduler;
  auto id1 = scheduler.add([](int a, int b) { return a + b; }, 1, 2);
  auto id2 = scheduler.add([](int a, int b) { return a + b; }, 1, scheduler.getFutureResult<int>(id1));

  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<int>(id2), 4);
}

TEST(LUVABL_SCHEDULER_TESTS, TEST_EVERY_TASK_EXECUTED_ONCE) {
  luvabl::TTaskScheduler scheduler;
  static int count = 0;
  auto id1 = scheduler.add([](int a, int b) { count++; return a + b; }, 1, 2);
  auto id2 = scheduler.add([](int a, int b) { count++; return a + b; }, 1, scheduler.getFutureResult<int>(id1));
  auto id3 = scheduler.add([](int a, int b) { count++; return a + b; }, 1, scheduler.getFutureResult<int>(id2));

  scheduler.executeAll();

  ASSERT_EQ(count, 3);
}
