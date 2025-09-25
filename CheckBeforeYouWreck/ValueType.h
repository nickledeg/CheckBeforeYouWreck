// ReSharper disable CppIncorrectBlankLinesNearBraces
#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <ranges>

// ReSharper disable once CppClangTidyBugproneMacroParentheses
#define HAS_TYPE_HELPER(CLASS_NAME, TYPE_NAME) \
template <typename T> \
class [[nodiscard]] CLASS_NAME { \
  template <typename U> \
  [[nodiscard]] static constexpr bool f(typename U::TYPE_NAME*) { \
    return true; \
  } \
\
  template <typename> \
  [[nodiscard]] static constexpr bool f(...) { \
    return false; \
  } \
\
public: \
  static bool constexpr value = f<T>(nullptr); \
}

namespace util::internal {
  HAS_TYPE_HELPER(HasValueTypeHelper, value_type);
}

namespace util {
  template <typename T>
  using remove_vref_t = std::remove_volatile_t<std::remove_reference_t<T>>;

  template <typename T>
  // ReSharper disable once CppInconsistentNaming
  concept has_value_type = internal::HasValueTypeHelper<T>::value;

  template <typename T>
  concept PseudoEnum =
    requires(T x) {
      { x.enumValue };
    } &&
    sizeof(T) == sizeof(decltype(T::enumValue)) &&
    std::integral<std::remove_cvref_t<decltype(T::enumValue)>>;
}

namespace util::internal {

  struct NoType {};

  template <typename>
  struct ValueTypeHelper;

  template <std::input_or_output_iterator T>
  struct ValueTypeHelper<T> {
    using Type = remove_vref_t<decltype(*std::declval<T&>())>;
  };

  template <std::ranges::range T>
  struct ValueTypeHelper<T> {
    using Type = remove_vref_t<
      decltype(*std::ranges::begin(std::declval<T&>()))>;
  };

  template <has_value_type T>
    requires(
      !std::input_or_output_iterator<T> &&
      !std::ranges::range<T>)
  struct ValueTypeHelper<T> {
    using Type = T::value_type;
  };

  template <typename T>
    requires(std::is_enum_v<T>)
  struct ValueTypeHelper<T> {
    using Type = std::underlying_type_t<T>;
  };

  template <PseudoEnum T>
  struct ValueTypeHelper<T> {
    using Type = decltype(T::enumValue);
  };

  template <typename T>
    requires (
      !has_value_type<T> &&
      !std::input_or_output_iterator<T> &&
      !std::ranges::range<T> &&
      !std::is_enum_v<T> &&
      !PseudoEnum<T>)
  struct ValueTypeHelper<T> {
    using Type = NoType;
  };
}

namespace util {
  template <typename T>
  using ValueAsIs = internal::ValueTypeHelper<T>::Type;

  template <typename T>
  using ValueType = std::remove_const_t<ValueAsIs<T>>;

  template <typename T>
  using ValueType2 = ValueType<ValueType<T>>;
}