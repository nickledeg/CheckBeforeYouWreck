// ReSharper disable CppClangTidyMiscUseInternalLinkage
#include "includes.h"
#include "Opt1.h"

using util::Opt1;

TEST(Opt1Test, constexpr_test) {

  static constexpr Opt1 z{5};
  //internal compiler error
  //ASSERT_TRUE(z.valid());
  ASSERT_TRUE(*z == 5);
}

[[nodiscard]] Opt1<ChatteringClass> getInPlace() {
  return std::in_place;
}

TEST(Opt1Test, test) {

  Opt1 const s{getInPlace()};
  ASSERT_TRUE(s.valid());
  ChatteringClass const& x{*s};
  std::ignore = x;
}