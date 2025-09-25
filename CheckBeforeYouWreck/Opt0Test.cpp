// ReSharper disable CppClangTidyMiscUseInternalLinkage
// ReSharper disable CppIncorrectBlankLinesNearBraces
#include "includes.h"
#include "Opt0.h"

using util::Opt0;

TEST(Opt0Test, test) {
  Opt0<ChatteringClass> const s{std::in_place};
  ASSERT_TRUE(s.valid());
  ChatteringClass const& x{*s};
  std::ignore = x;
}

TEST(Opt0Test, unchecked) {

  Opt0 const z{5};
  ASSERT_TRUE(z.valid());
  ASSERT_TRUE(*z == 5);
}

TEST(Opt0Test, constexpr_test) {

  static constexpr Opt0 z{5};
  ASSERT_TRUE(z.valid());
  ASSERT_TRUE(*z == 5);
}