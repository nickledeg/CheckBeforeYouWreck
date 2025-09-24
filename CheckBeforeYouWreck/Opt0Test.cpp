// ReSharper disable CppClangTidyMiscUseInternalLinkage
#include "includes.h"
#include "Opt0.h"

using util::Opt0;

TEST(Opt0Test, constexpr_test) {

  static constexpr Opt0 z{5};
  ASSERT_TRUE(z.valid());
  ASSERT_TRUE(*z == 5);
}

[[nodiscard]] Opt0<ChatteringClass> getInPlace() {
  return std::in_place;
}

TEST(Opt0Test, test) {

  static constexpr Opt0<int> z{};
  ASSERT_TRUE(z.valid());


  Opt0 const s{getInPlace()};
  ASSERT_TRUE(s.valid());
  ChatteringClass const& x{*s};
  std::ignore = x;
}