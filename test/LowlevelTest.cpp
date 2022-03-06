#include <gtest/gtest.h>

#include <iostream>
#include "convert.h"


namespace
{
TEST(LowlevelTest, TestUnitConversion1)
{
	double ft = UnitConvert::convertUnit(5.0, STORAGE_FORMAT_FOOT, STORAGE_FORMAT_M);

	EXPECT_NEAR(16.4042, ft, 0.0001);
}

TEST(LowlevelTest, TestUnitConversion2)
{
	double ft = UnitConvert::convertUnit(toTString("5m"), STORAGE_FORMAT_FOOT);

	EXPECT_NEAR(16.4042, ft, 0.0001);
}
}