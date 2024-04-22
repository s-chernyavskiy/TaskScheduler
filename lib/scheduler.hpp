#pragma once

#include <lib/any.hpp>

#include <map>
#include <vector>

namespace luvabl {

class TTaskScheduler {
 private:
  struct ITask {
    virtual ~ITask() = default;
    virtual luvabl::any execute() = 0;
    virtual bool is_executed() const = 0;
  };

  template<typename Func, typename T>
  struct Task : public ITask {
    Func func;
    int arg1;
    int arg2;
    bool executed = false;

    Task(Func f, T a1, T a2) : func(f), arg1(a1), arg2(a2) {}

    luvabl::any execute() override {
      if (!executed) {
        auto result = func(arg1, arg2);
        executed = true;
        return result;
      }
      return {};
    }

    bool is_executed() const override {
      return executed;
    }
  };

  std::vector<std::shared_ptr<ITask>> tasks;
  std::map<size_t, luvabl::any> results;

 public:
  TTaskScheduler() = default;

  template<typename Func, typename T>
  size_t add(Func func, T arg1, T arg2) {
    auto task = std::make_shared<Task<Func, T>>(func, arg1, arg2);
    tasks.push_back(task);
    return tasks.size() - 1;
  }

  void executeAll() {
    for (size_t i = 0; i < tasks.size(); ++i) {
      if (!tasks[i]->is_executed()) {
        results[i] = tasks[i]->execute();
      }
    }
  }

  template<typename T>
  T getResult(size_t id) {
    return luvabl::any_cast<T>(results[id]);
  }

  template<typename T>
  T getFutureResult(size_t id) {
    while (results.find(id) == results.end()) {
      executeAll();
    }
    return luvabl::any_cast<T>(results[id]);
  }
};

} // namespace luvabl
