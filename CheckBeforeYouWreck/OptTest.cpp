// ReSharper disable CppClangTidyMiscUseInternalLinkage
#include "includes.h"
#include "Opt.h"

using util::Opt;

TEST(OptTest, constexpr_test) {

  //static constexpr Opt x{5};
  //ASSERT_TRUE(x.valid());
  //ASSERT_TRUE(*x == 5);

}

[[nodiscard]] Opt<ChatteringClass> getInPlace() {
  return std::in_place;
}

TEST(OptTest, test) {

  Opt const s{getInPlace()};
  ASSERT_TRUE(s.valid());
  ChatteringClass const& x{*s};
  std::ignore = x;
}