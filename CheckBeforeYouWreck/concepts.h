// ReSharper disable CppIncorrectBlankLinesNearBraces
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

  template <typename T>
  concept EnumOrBool = Enum<T> || std::same_as<T, bool>;

  template <typename T, typename... Candidates>
  concept OneOf = (std::same_as<T, Candidates> || ...);

  template <typename T>
  concept Signed = OneOf<
    T,
    int8_t,
    int16_t,
    int32_t,
    int64_t,
    long,
    long long>;

  template <typename T>
  concept Unsigned = OneOf<
    T,
    uint8_t,
    uint16_t,
    uint32_t,
    uint64_t,
    unsigned long,
    unsigned long long>;

  template <typename T>
  concept StrictInt = Signed<T> || Unsigned<T>;

  template <typename T>
  concept EnumBoolOrStrictInt = EnumOrBool<T> || StrictInt<T>;
}