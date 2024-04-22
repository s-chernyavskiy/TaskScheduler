#pragma once

#include <memory>

namespace luvabl {

class any {
 private:
  template<typename T>
  friend const T* any_cast(const any*) noexcept;

  template<typename T>
  friend T* any_cast(any*) noexcept;

  struct Base {
    virtual ~Base() = default;
    virtual Base* copy() const = 0;
  };

  template<typename T>
  struct Derived : public Base {
    template<typename... Args>
    explicit Derived(Args... args) : value(args...) {}

    Base* copy() const override {
      return new Derived<T>(value);
    }

    T value;
  };

  Base* instance = nullptr;

 public:
  constexpr any() noexcept = default;

  any(const any& other) noexcept {
    if (other.instance) {
      instance = other.instance->copy();
    }
  }

  template<typename T, typename... Args>
  T& emplace(Args... args) noexcept {
    auto new_inst = new Derived<T>(args...);
    T& value = static_cast<Derived<T>*>(new_inst)->value;
    instance = new_inst;
    return value;
  }

  template<typename T>
  any(T&& value) noexcept {
    emplace<T>(std::forward<T>(value));
  }

  void swap(any& other) noexcept {
    std::swap(instance, other.instance);
  }

  any& operator=(const any& rhs) noexcept {
    if (this != &rhs) {
      any(rhs).swap(*this);
    }
    return *this;
  }

  ~any() {
    delete instance;
  }
};

class bad_any_cast : public std::exception {
 public:
  [[nodiscard]] const char* what() const noexcept override {
    return "bad any cast";
  }
};

template<typename T>
T any_cast(any& anything) {
  if (auto* value = any_cast<T>(&anything)) {
    return static_cast<T>(*value);
  } else {
    throw bad_any_cast();
  }
}

template<typename T>
const T* any_cast(const any* anything) noexcept {
  if (!anything) return nullptr;
  auto* storage = dynamic_cast<any::Derived<T>*>(anything->instance);
  if (!storage) return nullptr;
  return &storage->value;
}

template<typename T>
T* any_cast(any* anything) noexcept {
  return const_cast<T*>(any_cast<T>(static_cast<const any*>(anything)));
}

} // namespace luvabl
