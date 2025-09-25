// ReSharper disable CppVariableCanBeMadeConstexpr
// ReSharper disable CppClangTidyMiscUseAnonymousNamespace
// ReSharper disable CppClangTidyMiscUseInternalLinkage
// ReSharper disable CppRedundantBlankLines
// ReSharper disable CppIncorrectBlankLinesNearBraces
#include "includes.h"













#pragma region everyday use
TEST(EverydayUse, test) {

  std::optional<string> const x{"some text"s};

  //check whether x has a value
  ASSERT_TRUE(x.has_value());
  //similarly, via implicit conversion to bool:
  ASSERT_TRUE(x);

  //get value from x
  ASSERT_TRUE(x.value() == "some text");
  //similarly, via "dereferencing"
  ASSERT_TRUE(*x == "some text");

  //We note that we could safely call x.value()
  //because x.has_value() has been checked
  //However, std::optional does not enforce
  //that such a check has been conducted.
}
#pragma endregion













#pragma region operator== and conversion to bool
TEST(BoolTests, optional_bool_is_so_confusing) {

  optional<bool> const nullopt_value{};

  //optional in nullopt state converts to false
  ASSERT_FALSE(nullopt_value);
  //(optional has a conversion operator to bool)

  //std::optional in nullopt state
  //is not equal to either false or true
  ASSERT_FALSE(nullopt_value == false);
  ASSERT_FALSE(nullopt_value == true);
  ASSERT_TRUE(nullopt_value != false);
  ASSERT_TRUE(nullopt_value != true);
  //(optional provides operator== against underlying)
}

TEST(BoolTests, lets_try_with_an_enum_instead) {

  enum class NoYes : uint8_t { no, yes };

  optional<NoYes> const nullopt_value{};

  //optional in nullopt state converts to false
  ASSERT_FALSE(nullopt_value);
  //(optional has a conversion operator to bool)

  //this feels better! I can grok that an
  //optional in nullopt state is not equal to
  //an enum value.

  ASSERT_FALSE(nullopt_value == NoYes::no);
  ASSERT_FALSE(nullopt_value == NoYes::yes);
  ASSERT_TRUE(nullopt_value != NoYes::no);
  ASSERT_TRUE(nullopt_value != NoYes::yes);
}

#pragma region conclusions
//CONCLUSIONS:
//1. DITCH operator== because
//   a) it is confusing
//   b) it branches
//   c) it is superfluous (see next test)
//
//2. DITCH conversion to bool
//   a) it is confusing
//   b) it is superfluous (see next test)
//
//3. It is tempting to ban optional<bool>
//   but we can rescue it
#pragma endregion

TEST(BoolTests, more_explicit) {
  optional<bool> const nullopt_value{};

  //much clearer
  ASSERT_FALSE(nullopt_value.has_value());

  //**********don't do this***************
  //ASSERT_FALSE(nullopt_value == false);

  //***********do this instead*********
  ASSERT_FALSE(
    nullopt_value.has_value() &&
    *nullopt_value == false);

  //it shows branching explicitly through &&.
  //The extra code will encourage you to only
  //check against has_value once.
}
#pragma endregion













#pragma region copying and moving

#pragma region bad - copy
optional<ChatteringClass> bad_copy() {

  ChatteringClass const res;
  return res;
}

TEST(CopyingTests, bad_copy_test) {

  optional const x{bad_copy()};
  ASSERT_TRUE(x.has_value());
}
#pragma endregion



#pragma region better - move
optional<ChatteringClass> better_move() {

  ChatteringClass res;
  return res;
}

TEST(CopyingTests, better_move_test) {

  optional const x{better_move()};
  ASSERT_TRUE(x.has_value());
}
#pragma endregion



#pragma region better - move - similar to above
optional<ChatteringClass> better_move_similar_to_above() {

  return ChatteringClass{};
}

TEST(CopyingTests, better_move_similar_to_above_test) {

  optional const x{better_move_similar_to_above()};
  ASSERT_TRUE(x.has_value());
}
#pragma endregion



#pragma region best - in place
optional<ChatteringClass> best_in_place() {

  return optional<ChatteringClass>{std::in_place};
}

TEST(CopyingTests, best_in_place) {

  optional const x{best_in_place()};
  ASSERT_TRUE(x.has_value());
}
#pragma endregion
#pragma endregion












