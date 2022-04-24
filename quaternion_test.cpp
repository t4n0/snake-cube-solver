#include "quaternion.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sstream>
#include <tuple>

namespace math
{
namespace
{

TEST(ConstructorsTest, GivenSample_ExpectCorrectInstance)
{
    const Quaternion sample{2.0, 3.0, 4.0};
    const Quaternion expected{0.0, 2.0, 3.0, 4.0};
    ASSERT_THAT(sample, expected);
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
    ASSERT_THAT(GetLhs() * GetRhs(), GetExpectedResult());
}

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

}  // namespace
}  // namespace math