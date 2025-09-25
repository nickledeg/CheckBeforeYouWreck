// ReSharper disable CppClangTidyMiscUseInternalLinkage
#include "includes.h"
#include "Opt.h"

using util::Opt;

TEST(OptTest, test) {

  Opt<ChatteringClass> const s{std::in_place};
  ASSERT_TRUE(s.valid());
  ChatteringClass const& x{*s};
  std::ignore = x;
}

TEST(OptTest, constexpr_test) {

  //static constexpr Opt x{5};
  //ASSERT_TRUE(x.valid());
  //ASSERT_TRUE(*x == 5);
}