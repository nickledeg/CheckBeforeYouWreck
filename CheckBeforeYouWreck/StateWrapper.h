// ReSharper disable CppIncorrectBlankLinesNearBraces
#pragma once
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
// ReSharper disable CppRemoveRedundantBraces
#include "concepts.h"

#include <memory>
#include <thread>

//This class is only defined in debug
#ifndef NDEBUG
namespace util {

  template <Enum E>
  class [[nodiscard]] StateWrapper {

    //The enum value indicating the state
    //The default value of the enum should be
    //"unchecked"
    E state_{};

    //We only perform checking on the same thread on
    //which the class is instantiated.
    std::jthread::id threadId_{};

    //Used to block this class from being used as a
    //compile time constant.
    //This is necessary because StateWrapper is always
    //a mutable data member of a class.
    //Editing a "truly const" variable at runtime
    //is undefined behaviour.
    //Under MSVC, it would cause the compiler to crash!
    std::unique_ptr<std::byte> constexprBlocker_{};

  public:
    //Default ctor.
    constexpr StateWrapper() noexcept :
      //current thread id.
      //Checks are only performed on the creating thread
      threadId_{curThreadId()},
      //block from creation as a compile time constant
      constexprBlocker_{createConstexprBlocker()} {}

    //copy ctor. Note we don't define a move ctor.
    constexpr StateWrapper(
      StateWrapper const& other) noexcept :
      //the status is copied
      //from one object to another.
      state_{other.state_},
      //note that we do not copy the thread id.
      //i.e. the copied object is tied to the
      //thread that it is created on rather than
      //the creation thread of the object that
      //it was copied from.
      threadId_{curThreadId()},
      //block from creation as a compile
      //time constant. We create a new instance
      //as we cannot copy a std::unique_ptr.
      constexprBlocker_{createConstexprBlocker()} {}

    //copy operator. Note that we don't define
    //a move operator.
    constexpr StateWrapper& operator=(
      StateWrapper const& other) noexcept {

      //note that the thread id and constexpr blocker
      //are left as they were.
      if (&other != this)
        state_ = other.state_;

      return *this;
    }

    //note that we deliberately do not
    //specify move ctor or operator.
    //We don't even delete them.
    //We always want a pure copy

    constexpr ~StateWrapper() noexcept = default;

    //deliberately not nodiscard
    //this function allows assignment of the
    //state enum to this StateWrapper.
    constexpr StateWrapper& operator=(E x) noexcept {

      //note that the assignment only occurs
      //if we are on the object's creation thread.
      //The checking that StateWrapper provides
      //is only effective on the creation thread.
      if (threadId_ == curThreadId())
        state_ = x;

      return *this;
    }

    //we use this function to check whether
    //the object is in a given state
    //or is in one of a list of states.
    template <std::same_as<E>... Args>
    [[nodiscard]] constexpr bool in(
      Args... args) const noexcept {

      //note that we always return true if we are
      //not on the object's creation thread.
      //The checking that StateWrapper provides
      //is only effective on the creation thread.
      return
        threadId_ != curThreadId() ||
        ((state_ == args) || ...);
    }

  private:
    //This function only allocates if being called
    //at compile time otherwise it returns nullptr.
    //The sole purpose of the allocation is to
    //stop StateWrapper being stored as a compile
    //time constant.
    [[nodiscard]] static constexpr
    std::unique_ptr<std::byte>
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

    //returns current thread id if called at
    //runtime.
    //Returns the default thread id if called
    //at compile time. (I don't think you can do
    //multi threading at compile time???)
    [[nodiscard]] static constexpr std::jthread::id
    curThreadId() noexcept {

      if consteval {
        return {};
      }

      //note this is not constexpr
      return std::this_thread::get_id();
    }
  };
}
#endif