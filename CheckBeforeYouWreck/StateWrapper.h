#pragma once
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
// ReSharper disable CppRemoveRedundantBraces
#include "concepts.h"

#include <memory>
#include <thread>

//This class is only defined in debug
#ifndef NDEBUG
namespace util {

  //note that we deliberately do not
  //specify move ctor or operator.
  //We don't even delete them.
  //We always want a pure copy
  
  template <Enum E>
  class [[nodiscard]] StateWrapper {

    //Used to block this class from being used as a
    //compile time constant.
    //This is necessary because StateWrapper is always
    //a mutable data member of a class.
    //Editing a "truly const" variable at runtime
    //is undefined behaviour.
    //Under MSVC, it would cause the compiler to crash!
    std::unique_ptr <std::byte> constexprBlocker_{};

    //We only perform checking on the same thread on
    //which the class is instantiated.
    std::jthread::id threadId_{};

    //The enum value actually indicating the state
    //The default value of the enum should be
    //"unchecked"
    E x_{};

  public:
    //Default ctor. 
    constexpr StateWrapper() noexcept :
      //block from creation as a compile time constant
      constexprBlocker_{createConstexprBlocker()},
      //current thread id.
      //Checks are only performed on the creating thread 
      threadId_{curThreadId()} {}

    //copy ctor. Note we don't define a move ctor.
    constexpr StateWrapper(
      StateWrapper const& other) noexcept :
      //block from creation as a compile
      //time constant. We create a new instance
      //as we cannot copy a std::unique_ptr.
      constexprBlocker_{createConstexprBlocker()},
      //note that we do not copy the thread id.
      //i.e. the copied object is tied to the
      //thread that it is created on rather than
      //the creation thread of the object that
      //it was copied from.
      threadId_{curThreadId()},
      //the checked status is copied
      //from one object to another.
      x_{other.x_} {}

    //copy operator. Note that we don't define
    //a move operator.
    constexpr StateWrapper& operator=(
      StateWrapper const& other) noexcept {

      //note that the thread id is left as is.
      if (&other != this)
        x_ = other.x_;

      return *this;
    }

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
        x_ = x;

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
        ((x_ == args) || ...);
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