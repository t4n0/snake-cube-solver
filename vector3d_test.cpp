#include "vector3d.h"

#include <gmock/gmock.h>

namespace math
{
namespace
{

struct Vector3DEqualityOperatorParameter
{
    Vector3D a{};
    Vector3D b{};
    bool expected_result{};
};

class Vector3DEqualityOperatorTest : public testing::TestWithParam<Vector3DEqualityOperatorParameter>
{
};

TEST_P(Vector3DEqualityOperatorTest, GivenSample_ExpectCorrectResult)
{
    ASSERT_THAT(GetParam().a == GetParam().b, GetParam().expected_result);
}

INSTANTIATE_TEST_SUITE_P(,
                         Vector3DEqualityOperatorTest,
                         testing::Values(Vector3DEqualityOperatorParameter{{1.0, 2.0, 3.0}, {1.0, 2.0, 3.0}, true},
                                         Vector3DEqualityOperatorParameter{{1.0, 2.0, 3.0}, {0.7, 2.0, 3.0}, false},
                                         Vector3DEqualityOperatorParameter{{1.0, 2.0, 3.0}, {1.0, 0.7, 3.0}, false},
                                         Vector3DEqualityOperatorParameter{{1.0, 2.0, 3.0}, {1.0, 2.0, 0.7}, false}));

TEST(Vector3DAdditionOperatorTest, GivenSample_ExpectCorrectResult)
{
    Vector3D a{1.0, 2.0, 3.0};
    Vector3D b{4.0, 5.0, 6.0};
    Vector3D expected{5.0, 7.0, 9.0};

    ASSERT_THAT(a + b, expected);
}

TEST(Vector3DSubtractionOperatorTest, GivenSample_ExpectCorrectResult)
{
    Vector3D a{1.0, 2.0, 3.0};
    Vector3D b{4.0, 3.0, 2.0};
    Vector3D expected{-3.0, -1.0, 1.0};

    ASSERT_THAT(a - b, expected);
}

TEST(Vector3DScalarMultiplicationOperatorTest, GivenSample_ExpectCorrectResult)
{
    Vector3D a{1.0, 2.0, 3.0};
    double factor{2.0};
    Vector3D expected{2.0, 4.0, 6.0};

    ASSERT_THAT(a * factor, expected);
    ASSERT_THAT(factor * a, expected);
}

}  // namespace
}  // namespace math