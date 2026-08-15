#include <Eigen/Dense>
#include <gtest/gtest.h>

TEST(Eigen, VectorAddition)
{
	Eigen::Vector3d a(1.0, 2.0, 3.0);
	Eigen::Vector3d b(4.0, 5.0, 6.0);

	Eigen::Vector3d result = a + b;

	EXPECT_DOUBLE_EQ(result.x(), 5.0);
	EXPECT_DOUBLE_EQ(result.y(), 7.0);
	EXPECT_DOUBLE_EQ(result.z(), 9.0);
}
