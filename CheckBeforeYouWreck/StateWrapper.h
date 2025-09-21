#pragma once
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
// ReSharper disable CppRemoveRedundantBraces
#include "concepts.h"

#include <memory>
#include <thread>

#ifndef NDEBUG
namespace util {

  //note that we deliberately do not
  //specify move ctor or operator.
  //We don't even delete them.
  //We do not want any move elision.
  
  template <Enum E>
  class [[nodiscard]] StateWrapper {

    //Used to block this class from being used as a
    //constexpr with static storage duration.
    //This is necessary because StateWrapper is always
    //a mutable data member of a class.
    //Editing a "truly const" variable at runtime
    //is undefined behaviour.
    //Under MSVC, it would cause the compiler to crash!
    std::unique_ptr <std::byte> constexprBlocker_;

    //We only perform checking on the same thread on
    //which the class is instantiated.
    std::jthread::id threadId_;
    E x_{};

    [[nodiscard]] static constexpr std::unique_ptr<std::byte>
      createConstexprBlocker() noexcept {

      if consteval {
        try {
          return std::make_unique<std::byte>();
        }
        catch (...) {
          //constexpr allocation error
          std::unreachable();
        }
      }

      return {};
    }

    [[nodiscard]] static constexpr std::jthread::id
    curThreadId() noexcept {

      if consteval {
        return {};
      }

      return std::this_thread::get_id();
    }

  public:
    constexpr StateWrapper() noexcept :
      constexprBlocker_{ createConstexprBlocker() },
      threadId_{ curThreadId() } {
    }

    constexpr StateWrapper(StateWrapper const& other) noexcept :
      constexprBlocker_{ createConstexprBlocker() },
      threadId_{ curThreadId() },
      x_{ other.x_ } {
    }

    constexpr StateWrapper& operator=(StateWrapper const& other) noexcept {

      if (&other != this)
        x_ = other.x_;

      return *this;
    }

    constexpr ~StateWrapper() noexcept = default;

    //deliberately not nodiscard
    constexpr StateWrapper& operator=(E x) noexcept {

      if (threadId_ == curThreadId())
        x_ = x;

      return *this;
    }

    template <std::same_as<E>... Args>
    [[nodiscard]] constexpr bool in(Args... args) const noexcept {

      return threadId_ != curThreadId() || ((x_ == args) || ...);
    }
  };
}
#endif