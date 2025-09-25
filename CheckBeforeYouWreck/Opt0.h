// ReSharper disable CppNonExplicitConvertingConstructor
// ReSharper disable CppIncorrectBlankLinesNearBraces
#pragma once
#include "concepts.h"
#include "ValueType.h"

#include <optional>

//1. No operator==                    PASS
//2. No implicit conversion to bool   PASS
//3. Disallow references and C arrays PASS
//4. Accidental copies prevented      PASS
//5. Disallow Opt0<Opt0<T>>           PASS
//6. Guaranteed access check          FAIL

namespace util {

  //Undecorated concepts stops T being a
  //reference or a C array
  template <Undecorated T>
    //we have class wrappers for std::expected
    //this requires clause stops Opt0 being used
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
  class [[nodiscard]] Opt0 {
    std::optional<T> x_{};

    //Disallow Opt0<Opt0<T>>
    static_assert(!std::same_as<T, Opt0<ValueType<T>>>);

  public:
    //similar to may stl types
    using value_type = T;

    //Note that none of the constructors are explicit
    //This is because we want to be able to return
    //default ctor
    constexpr Opt0() noexcept = default;

    //pass through in-place constructor
    template <typename... Args>
      requires(std::is_constructible_v<T, Args...>)
    constexpr Opt0(std::in_place_t, Args&&... args) noexcept(
      std::is_nothrow_constructible_v<T, Args...>) :
      x_{std::in_place, std::forward<Args>(args)...} {}

    //rvalue reference constructor
    constexpr Opt0(T&& x) noexcept :
      x_{std::move(x)} {}

    //we do not allow non trivially copyable T
    //to construct Opt0<T> since we want to avoid
    //accidental copies.
    constexpr Opt0(T const& x) noexcept
      requires(std::is_trivially_copyable_v<T>) :
      x_{x} {}

    //again same reasoning as above
    constexpr Opt0(T& x) noexcept requires(
      std::is_trivially_copyable_v<T>) :
      x_{x} {}

    //we delete this constructor for non trivially
    //copyable types for the same reason as above
    constexpr Opt0(T const&)
      requires(!std::is_trivially_copyable_v<T>) = delete;

    //we delete this constructor for non trivially
    //copyable types for the same reason as above
    constexpr Opt0(T&)
      requires(!std::is_trivially_copyable_v<T>) = delete;

    //pass through function
    template <typename... Args>
      requires(std::constructible_from<T, Args...>)
    constexpr T& emplace(Args&&... args) noexcept(
      std::is_nothrow_constructible_v<T, Args...>) {

      T& res{x_.emplace(std::forward<Args>(args)...)};
      return res;
    }

    //pass through function
    [[nodiscard]] constexpr T const* operator->(
    ) const noexcept {

      assert(x_.has_value());
      return x_.operator->();
    }

    //pass through function
    [[nodiscard]] constexpr T* operator->(
    ) noexcept {

      assert(x_.has_value());
      return x_.operator->();
    }

    //pass through function
    [[nodiscard]] constexpr T const& operator*(
    ) const & noexcept {

      assert(x_.has_value());
      return x_.operator*();
    }

    //pass through function
    [[nodiscard]] constexpr T& operator*(
    ) & noexcept {

      assert(x_.has_value());
      return x_.operator*();
    }

    //pass through function
    [[nodiscard]] constexpr T&& operator*(
    ) && noexcept {

      assert(x_.has_value());
      return std::move(x_).operator*();
    }

    //I prefer the briefer valid() rather than has_value()
    //note that we do not define a converter to bool since
    //it is confusing
    [[nodiscard]] constexpr bool valid() const noexcept {

      return x_.has_value();
    }

    //pass through function
    constexpr void reset() noexcept {
      return x_.reset();
    }

    //same as operator== but more
    //explicit to prevent confusion
    template <std::equality_comparable_with<T> U>
    [[nodiscard]] constexpr bool eq(
      U const& x) const noexcept {

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
}