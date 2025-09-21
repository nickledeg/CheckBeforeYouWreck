// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once
#include <cassert>

#include "StateWrapper.h"
#include "ValueType.h"
#include "concepts.h"

#include <optional>

//1. No operator==                    PASS
//2. No conversion to bool            PASS
//3. Disallow references and C arrays PASS
//4. Accidental copies prevented      PASS
//5. Disallow Opt<Opt<T>>             PASS
//6. Guaranteed access check          PASS

namespace util {

  //Undecorated concepts stops T being a
  //reference or a C array
  template <Undecorated T>
  //we have class wrappers for std::expected
  //this requires clause stops Opt being used
  //with such types.
  //!HasErr<T> && !HasEc<T> avoids such complex
  //types such as
  //std::optional<std::expected<X, Y>>
  //
  //We disallow std::same_as<T, std::in_place_t>
  //because reasons.
    requires(
  !HasErr<T> &&
    !HasEc<T> &&
    !std::same_as<T, std::in_place_t>)
  class [[nodiscard]] Opt {
    std::optional<T> x_{};

    //Disallow Opt<Opt<T>>
    static_assert(!std::same_as<T, Opt<ValueType<T>>>);

#ifndef _NDEBUG

    enum class [[nodiscard]] State : uint8_t {
      unchecked,
      valid,
      invalid
    };

    mutable StateWrapper<State> state_{};
#endif

  public:
    static constexpr bool DEBUG_CHANGES_LAYOUT{true};
    using value_type = T;

      //Note that none of the constructors are explicit
      //This is because we want to be able to return
      //default ctor
      constexpr Opt() noexcept = default;

      //pass through in-place constructor
      template <typename... Args>
        requires(std::is_constructible_v<T, Args...>)
      constexpr Opt(std::in_place_t, Args&&... args) noexcept(
        std::is_nothrow_constructible_v<T, Args...>) :
        x_{ std::in_place, std::forward<Args>(args)... } {
      }

      //rvalue reference constructor
      constexpr Opt(T&& x) noexcept :
        x_{ std::move(x) } {
      }

      //we do not allow non trivially copyable T
      //to construct Opt<T> since we want to avoid
      //accidental copies.
      constexpr Opt(T const& x) noexcept
        requires(std::is_trivially_copyable_v<T>) :
        x_{ x } {
      }

      //again same reasoning as above
      constexpr Opt(T& x) noexcept requires(
        std::is_trivially_copyable_v<T>) :
        x_{ x } {
      }

      //we delete this constructor for non trivially
      //copyable types for the same reason as above
      constexpr Opt(T&)
        requires(!std::is_trivially_copyable_v<T>) = delete;

      //pass through function
      template <typename... Args>
        requires(std::constructible_from<T, Args...>)
      constexpr T& emplace(Args&&... args) noexcept(
        std::is_nothrow_constructible_v<T, Args...>) {

      T& res{x_.emplace(std::forward<Args>(args)...)};

#ifndef _NDEBUG
      if (state_.in(State::invalid))
        state_ = State::unchecked;
#endif

      return res;
    }

      //pass through function
      [[nodiscard]] constexpr T const* operator->() const noexcept {

		assert(state_.in(State::valid));
        assert(x_.has_value());
        return x_.operator->();
      }

      //pass through function
      [[nodiscard]] constexpr T* operator->() noexcept {

        assert(state_.in(State::valid));
        assert(x_.has_value());
        return x_.operator->();
      }

      //pass through function
      [[nodiscard]] constexpr T const& operator*() const& noexcept {

        assert(state_.in(State::valid));
        assert(x_.has_value());
        return x_.operator*();
      }

      //pass through function
      [[nodiscard]] constexpr T& operator*() & noexcept {

        assert(state_.in(State::valid));
        assert(x_.has_value());
        return x_.operator*();
      }

      //pass through function
      [[nodiscard]] constexpr T&& operator*() && noexcept {

        assert(state_.in(State::valid));
        assert(x_.has_value());
        return std::move(x_).operator*();
      }

      //I prefer the briefer valid() rather than has_value()
      //note that we do not define a converter to bool since
      //it is confusing
      [[nodiscard]] constexpr bool valid() const noexcept {

#ifndef _NDEBUG
        if (x_.has_value())
          state_ = State::valid;
        else
          state_ = State::invalid;
#endif

        return x_.has_value();
      }

      //pass through function
      constexpr void reset() noexcept {
        return x_.reset();
      }

      //same as operator== but more
      //explicit to prevent confusion
      template <std::equality_comparable_with<T> U>
      [[nodiscard]] constexpr bool eq(U const& x) const noexcept {
        return x_.has_value() && *x_ == x;
      }

      //I don't think that std::optional has this.
      //I find it useful.
      [[nodiscard]] constexpr T const* toPtr() const noexcept {

        if (x_.has_value())
          return std::addressof(*x_);

        return {};
      }

      //I don't think that std::optional has this.
      //I find it useful.
      [[nodiscard]] constexpr T* toPtr() noexcept {

        if (x_.has_value())
          return std::addressof(*x_);

        return {};
      }
  };

  //concept top test if a type is an Opt
  template <typename T>
  concept Optional = std::same_as<T, Opt<ValueType<T>>>;
}
