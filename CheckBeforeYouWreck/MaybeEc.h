// ReSharper disable CppIncorrectBlankLinesNearBraces
// ReSharper disable GrammarMistakeInComment
#pragma once
// ReSharper disable CppNonExplicitConvertingConstructor
#include <expected>

#include "StateWrapper.h"

namespace util {

  template <Undecorated T, EnumBoolOrStrictInt Ec>
  class [[nodiscard]] MaybeEc {

    //Don't allow T or Ec to be convertible to each
    //other. This allows us to cleanly return a T or
    //an Ec which converts to a MaybeEc without confusion

    //Causes problems with incomplete types if set
    //as a requires constraint.
    //Use a static_assert instead.
    static_assert(
      !std::convertible_to<T, Ec> &&
      !std::convertible_to<Ec, T>);

    std::expected<T, Ec> x_;

#ifndef NDEBUG

    enum class [[nodiscard]] State : uint8_t {
      unchecked,
      error,
      valid
    };

    mutable StateWrapper<State> state_{};
#endif

  public:
    static constexpr bool DEBUG_CHANGES_LAYOUT{true};
    using value_type = T;
    using error_type = Ec;

    [[nodiscard]] constexpr MaybeEc(T&& x) noexcept :
      x_{std::in_place, std::move(x)} {}

    [[nodiscard]] constexpr MaybeEc(T const& x) noexcept
      requires(std::is_trivially_copyable_v<T>) :
      x_{std::in_place, std::move(x)} {}

    constexpr MaybeEc(T const&)
      requires(!std::is_trivially_copyable_v<T>) = delete;
    constexpr MaybeEc(T&)
      requires(!std::is_trivially_copyable_v<T>) = delete;

    template <typename... Args>
      requires (std::constructible_from<T, Args...>)
    [[nodiscard]] constexpr MaybeEc(
      std::in_place_t, Args&&... args) noexcept(
      std::is_nothrow_constructible_v<T, Args...>) :
      x_{std::in_place, std::forward<Args>(args)...} {}

    [[nodiscard]] constexpr MaybeEc(Ec ec) noexcept :
      x_{std::unexpect, ec} {}

    [[nodiscard]] constexpr bool hasEc() const noexcept {

#ifndef NDEBUG
      if (x_.has_value())
        state_ = State::valid;
      else
        state_ = State::error;
#endif

      return !x_.has_value();
    }

    [[nodiscard]] constexpr Ec ec() const noexcept {

      assert(state_.in(State::error));
      assert(!x_.has_value());
      return x_.error();
    }

    //pass through
    [[nodiscard]] constexpr T const& operator*(
    ) const & noexcept {

      assert(state_.in(State::valid));
      assert(x_.has_value());
      return *x_;
    }

    //pass through
    [[nodiscard]] constexpr T& operator*(
    ) & noexcept {

      assert(state_.in(State::valid));
      assert(x_.has_value());
      return *x_;
    }

    //pass through
    [[nodiscard]] constexpr T&& operator*(
    ) && noexcept {

      assert(state_.in(State::valid));
      assert(x_.has_value());
      return *std::move(x_);
    }

    //pass through
    [[nodiscard]] constexpr T const* operator->(
    ) const & noexcept {

      assert(state_.in(State::valid));
      assert(x_.has_value());
      return x_.operator->();
    }

    //pass through
    [[nodiscard]] constexpr T* operator->(
    ) & noexcept {

      assert(state_.in(State::valid));
      assert(x_.has_value());
      return x_.operator->();
    }

    //I like having this
    [[nodiscard]] constexpr T const* ptr(
    ) const & noexcept {

      assert(state_.in(State::valid));
      assert(x_.has_value());
      return std::addressof(*x_);
    }

    //I like having this
    [[nodiscard]] constexpr T* ptr(
    ) & noexcept {

      assert(state_.in(State::valid));
      assert(x_.has_value());
      return std::addressof(*x_);
    }
  };
}