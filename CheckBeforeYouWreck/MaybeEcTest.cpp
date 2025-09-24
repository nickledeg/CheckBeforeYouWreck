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

  x = MyEc::nonTechnicalBossSaysNotToWorryAboutTechnicalDebt;

  ASSERT_TRUE(x.hasEc());
  ASSERT_TRUE(
    x.ec() ==
    MyEc::nonTechnicalBossSaysNotToWorryAboutTechnicalDebt);

  x = 10;
  ASSERT_FALSE(x.hasEc());
  ASSERT_TRUE(*x == 10);
}