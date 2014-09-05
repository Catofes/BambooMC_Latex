#include <CppUTest/TestHarness.h>

TEST_GROUP(DetectorGroup)
{
};

TEST(DetectorGroup, FirstTest)
{
  STRCMP_EQUAL("Fail me!", "Fail me!");
}
