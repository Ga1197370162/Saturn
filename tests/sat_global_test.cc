#include "gtest.h"

/**
 * EXPECT_TRUE(RET): ret == true
 * EXPECT_FALSE(RET): ret == false
 *
 * EXPECT_EQ(EXPECTED, ACTUAL): EXPECTED == ACTUAL
 * EXPECT_NE(EXPECTED, ACTUAL): EXPECTED != ACTUAL
 * EXPECT_FLOAT_EQ(EXPECTED, ACTUAL): (float)EXPECTED == ACTUAL
 * EXPECT_DOUBLE_EQ(EXPECTED, ACTUAL): (double)EXPECTED == ACTUAL
 * EXPECT_NEAR(VAR1, VAR2, TOL): abs(VAR1 - VAR2) <= TOL
 *
 * EXPECT_STREQ(S1, S2): S1 == S2
 * EXPECT_STRNE(S1, S2): S1 != S2
 * EXPECT_STRCASEEQ(S1, S2): TO_LOWER(S1) == TO_LOWER(S2)
 * EXPECT_STRCASENE(S1, S2): TO_LOWER(S1) != TO_LOWER(S2)
*/

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}