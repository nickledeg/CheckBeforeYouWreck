#pragma once

#include <concepts>

namespace util {

  template <typename T, typename U>
  concept SimilarTo =
    std::same_as<
      std::remove_cvref_t<T>,
      std::remove_cvref_t<U>>;

  template <typename T>
  concept Undecorated =
    std::same_as<T, std::remove_cvref_t<T>> &&
    !std::is_array_v<T>;

  template <typename T>
  concept HasErr = requires(T x) {
    { x.hasErr() } -> std::same_as<bool>;
    { std::forward<T>(x).err() };
  };

  template <typename T>
  concept HasEc = requires(T x) {
    { x.hasEc() } -> std::same_as<bool>;
    { std::forward<T>(x).ec() };
  };

  template <typename T>
  concept Enum = std::is_enum_v<T>;
}