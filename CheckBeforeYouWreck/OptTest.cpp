#include "includes.h"
#include "Opt.h"

using util::Opt;

TEST(Opt, xx) {

  Opt<string> s{"gay"};
  ASSERT_TRUE(s.valid());
  std::string const x{*std::move(s)};
  std::ignore = x;
}