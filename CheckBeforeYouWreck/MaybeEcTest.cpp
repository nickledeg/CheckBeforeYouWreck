// ReSharper disable CppIncorrectBlankLinesNearBraces
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidMagicNumbers
#include "includes.h"
#include "MaybeEc.h"

using util::MaybeEc;

enum class [[nodiscard]] MyEc : uint8_t {
  worldEnds,
  userDidBadThings,
  nonTechnicalBossSaysNotToWorryAboutTechnicalDebt,
  vibeCoderHired
};

TEST(MaybeEcTest, generalUse) {

  MaybeEc<int, MyEc> x{5};
  ASSERT_FALSE(x.hasEc());
  ASSERT_TRUE(*x == 5);

  x = MyEc::vibeCoderHired;
  ASSERT_TRUE(x.hasEc());
  ASSERT_TRUE(x.ec() == MyEc::vibeCoderHired);

  x = 10;
  ASSERT_FALSE(x.hasEc());
  ASSERT_TRUE(*x == 10);

  x = {std::in_place, 100};
  ASSERT_FALSE(x.hasEc());
  ASSERT_TRUE(*x == 100);
}

TEST(MaybeEcTest, constexpr_test) {

  //constexpr MaybeEc<int, MyEc> y{5};
}