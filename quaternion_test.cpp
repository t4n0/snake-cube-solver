#include "quaternion.h"

#include "math_constants.h"

#include <gmock/gmock.h>

#include <sstream>
#include <tuple>

namespace math
{
namespace
{

MATCHER_P(IsCloseTo, target, "")
{
    const auto IsCloseTo = [](const double candidate, const double target) {
        constexpr double generous_epsilon{1e-6};
        const bool is_above_upper = candidate > target + generous_epsilon;
        const bool is_below_lower = candidate < target - generous_epsilon;
        return !is_above_upper && !is_below_lower;
    };

    return IsCloseTo(arg.GetScalarPart(), target.GetScalarPart()) &&              //
           IsCloseTo(arg.GetVectorPart().at(0), target.GetVectorPart().at(0)) &&  //
           IsCloseTo(arg.GetVectorPart().at(1), target.GetVectorPart().at(1)) &&  //
           IsCloseTo(arg.GetVectorPart().at(2), target.GetVectorPart().at(2));
}

TEST(ConstructorsTest, GivenSample_ExpectCorrectInstance)
{
    const Quaternion sample{2.0, 3.0, 4.0};
    const Quaternion expected{0.0, 2.0, 3.0, 4.0};
    ASSERT_THAT(sample, expected);
}

TEST(AdditionOperatorTest, GivenSample_ExpectCorrectResult)
{
    const Quaternion a{0.1, 0.2, 0.3, 0.4};
    const Quaternion b{1.0, 2.0, 3.0, 4.0};
    const Quaternion expected{1.1, 2.2, 3.3, 4.4};

    ASSERT_THAT(a + b, expected);
}

TEST(SubtractionOperatorTest, GivenSample_ExpectCorrectResult)
{
    const Quaternion a{0.1, 0.2, 0.3, 0.4};
    const Quaternion b{1.0, 2.0, 3.0, 4.0};
    const Quaternion expected{-0.9, -1.8, -2.7, -3.6};

    ASSERT_THAT(a - b, expected);
}

TEST(StreamOperatorTest, GivenSample_ExpectCorrectString)
{
    const Quaternion sample{1.1, 2.0, 3, 4};
    std::stringstream stream{};
    stream << sample;
    ASSERT_THAT(stream.str(), std::string{"w: 1.1, i: 2, j: 3, k: 4"});
}

struct EqualityOperatorTest : public testing::TestWithParam<std::tuple<Quaternion, Quaternion, bool>>
{
    auto GetLhs() const { return std::get<0>(GetParam()); }
    auto GetRhs() const { return std::get<1>(GetParam()); }
    auto GetExpectedAreEqual() const { return std::get<2>(GetParam()); }
};

TEST_P(EqualityOperatorTest, GivenSample_ExpectCorrectResult)
{
    ASSERT_THAT(GetLhs() == GetRhs(), GetExpectedAreEqual());
    ASSERT_THAT(GetLhs() != GetRhs(), !GetExpectedAreEqual());
}

INSTANTIATE_TEST_SUITE_P(,
                         EqualityOperatorTest,
                         testing::Values(std::make_tuple(Quaternion{1, 2, 3, 0}, Quaternion{1, 2, 3, 0}, true),
                                         std::make_tuple(Quaternion{1, 0, 0, 0}, Quaternion{2, 0, 0, 0}, false),
                                         std::make_tuple(Quaternion{0, 1, 0, 0}, Quaternion{0, 2, 0, 0}, false),
                                         std::make_tuple(Quaternion{0, 0, 1, 0}, Quaternion{0, 0, 2, 0}, false),
                                         std::make_tuple(Quaternion{0, 0, 0, 1}, Quaternion{0, 0, 0, 2}, false)));

struct MultiplicationOperatorTest : public testing::TestWithParam<std::tuple<Quaternion, Quaternion, Quaternion>>
{
    auto GetLhs() const { return std::get<0>(GetParam()); }
    auto GetRhs() const { return std::get<1>(GetParam()); }
    auto GetExpectedResult() const { return std::get<2>(GetParam()); }
};

TEST_P(MultiplicationOperatorTest, GivenSample_ExpectCorrectResult)
{
    ASSERT_THAT(GetLhs() * GetRhs(), IsCloseTo(GetExpectedResult()));
}

// Basic rules of quaternion multiplication:
// i^2 = j^2 = k^2 = -1
// ij = -ji = k
// ki = -ik = j
// jk = -kj = i
INSTANTIATE_TEST_SUITE_P(
    ,
    MultiplicationOperatorTest,
    testing::Values(std::make_tuple(Quaternion{1, 0, 0, 0}, Quaternion{1, 0, 0, 0}, Quaternion{1, 0, 0, 0}),
                    std::make_tuple(Quaternion{1, 0, 0, 0}, Quaternion{0, 1, 0, 0}, Quaternion{0, 1, 0, 0}),
                    std::make_tuple(Quaternion{1, 0, 0, 0}, Quaternion{0, 0, 1, 0}, Quaternion{0, 0, 1, 0}),
                    std::make_tuple(Quaternion{1, 0, 0, 0}, Quaternion{0, 0, 0, 1}, Quaternion{0, 0, 0, 1}),
                    std::make_tuple(Quaternion{0, 1, 0, 0}, Quaternion{1, 0, 0, 0}, Quaternion{0, 1, 0, 0}),
                    std::make_tuple(Quaternion{0, 1, 0, 0}, Quaternion{0, 1, 0, 0}, Quaternion{-1, 0, 0, 0}),
                    std::make_tuple(Quaternion{0, 1, 0, 0}, Quaternion{0, 0, 1, 0}, Quaternion{0, 0, 0, 1}),
                    std::make_tuple(Quaternion{0, 1, 0, 0}, Quaternion{0, 0, 0, 1}, Quaternion{0, 0, -1, 0}),
                    std::make_tuple(Quaternion{0, 0, 1, 0}, Quaternion{1, 0, 0, 0}, Quaternion{0, 0, 1, 0}),
                    std::make_tuple(Quaternion{0, 0, 1, 0}, Quaternion{0, 1, 0, 0}, Quaternion{0, 0, 0, -1}),
                    std::make_tuple(Quaternion{0, 0, 1, 0}, Quaternion{0, 0, 1, 0}, Quaternion{-1, 0, 0, 0}),
                    std::make_tuple(Quaternion{0, 0, 1, 0}, Quaternion{0, 0, 0, 1}, Quaternion{0, 1, 0, 0}),
                    std::make_tuple(Quaternion{0, 0, 0, 1}, Quaternion{1, 0, 0, 0}, Quaternion{0, 0, 0, 1}),
                    std::make_tuple(Quaternion{0, 0, 0, 1}, Quaternion{0, 1, 0, 0}, Quaternion{0, 0, 1, 0}),
                    std::make_tuple(Quaternion{0, 0, 0, 1}, Quaternion{0, 0, 1, 0}, Quaternion{0, -1, 0, 0}),
                    std::make_tuple(Quaternion{0, 0, 0, 1}, Quaternion{0, 0, 0, 1}, Quaternion{-1, 0, 0, 0})));

TEST(InverseTest, GivenMultiplicationWithInverse_ExpectIdentity)
{
    const Quaternion sample{1.1, 2.2, 3.3, 4.4};
    const Quaternion identity{1.0, 0.0, 0.0, 0.0};
    ASSERT_THAT(sample * sample.Inverse(), identity);
}

TEST(InverseTest, GivenInverse_ExpectCounterRotation)
{
    const Quaternion rotation = CreateRotation(TAU / 2.0, 1.0, 0.0, 0.0);
    const Quaternion counter_rotation = CreateRotation(-TAU / 2.0, 1.0, 0.0, 0.0);
    ASSERT_THAT(rotation.Inverse(), counter_rotation);
}

TEST(RotateByTest, GivenSample_ExpectCorrectRotation)
{
    const Quaternion rotation = CreateRotation(TAU / 2, 1.0, 0.0, 0.0);
    const Quaternion vector_in{0.0, 0.0, 1.0, 0.0};

    const Quaternion vector_out = vector_in.RotateBy(rotation);

    const Quaternion expected{0.0, 0.0, -1.0, 0.0};
    ASSERT_THAT(vector_out, IsCloseTo(expected));
}

TEST(RotationTest, GivenConsecutiveRotation_ExpectCorrectRotation)
{
    const Quaternion first = CreateRotation(TAU / 4.0, 1.0, 0.0, 0.0);
    const Quaternion second = CreateRotation(TAU / 4.0, 0.0, 0.0, 1.0);
    const Quaternion arbitrary_location_before{0.0, 1.0, 1.0, 0.0};
    const Quaternion expected_location_after{0.0, 0.0, 1.0, 1.0};
    ASSERT_THAT(arbitrary_location_before.RotateBy(first).RotateBy(second), expected_location_after);
}

TEST(RotationTest, GivenDifferentOrderOfRotations_ExpectDifferentEndpoints)
{
    const Quaternion first = CreateRotation(TAU / 4.0, 1.0, 0.0, 0.0);
    const Quaternion second = CreateRotation(TAU / 4.0, 0.0, 0.0, 1.0);
    const Quaternion arbitrary_location_before{0.0, 1.0, 1.0, 0.0};
    ASSERT_THAT(arbitrary_location_before.RotateBy(first).RotateBy(second),
                testing::Ne(arbitrary_location_before.RotateBy(second).RotateBy(first)));

    const Quaternion expected_location_after{0.0, -1.0, 0.0, 1.0};
    ASSERT_THAT(arbitrary_location_before.RotateBy(second).RotateBy(first), IsCloseTo(expected_location_after));
}

TEST(RotationTest, GivenRotatedRotation_ExpectCorrectRotation)
{
    const Quaternion first = CreateRotation(TAU / 4.0, 0.0, 0.0, 1.0);
    const Quaternion second = CreateRotation(TAU / 4.0, 1.0, 0.0, 0.0);
    const Quaternion arbitrary_location_before{0.0, 1.0, 1.0, 0.0};
    const Quaternion expected_location_after{0.0, 0.0, 1.0, 1.0};
    ASSERT_THAT(arbitrary_location_before.RotateBy(first).RotateBy(second.RotateBy(first)),
                IsCloseTo(expected_location_after));
}

}  // namespace
}  // namespace math