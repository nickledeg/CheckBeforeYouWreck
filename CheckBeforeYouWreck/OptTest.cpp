// ReSharper disable CppClangTidyMiscUseInternalLinkage
#include "includes.h"
#include "Opt.h"

using util::Opt;

[[nodiscard]] Opt<ChatteringClass> ff() {
  return std::in_place;
}

TEST(Opt, xx) {

  Opt const s{ff()};
  ASSERT_TRUE(s.valid());
  ChatteringClass const& x{*s};
  std::ignore = x;
}