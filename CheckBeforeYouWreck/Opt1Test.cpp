// ReSharper disable CppClangTidyMiscUseInternalLinkage
// ReSharper disable CppIncorrectBlankLinesNearBraces
#include "includes.h"
#include "Opt1.h"

using util::Opt1;

TEST(Opt1Test, test) {

  Opt1<ChatteringClass> const s{std::in_place};
  ASSERT_TRUE(s.valid());
  ChatteringClass const& x{*s};
  std::ignore = x;
}

TEST(Opt1Test, constexpr_test) {

  static constexpr Opt1 z{5};
  ASSERT_TRUE(z.valid());
  ASSERT_TRUE(*z == 5);
}