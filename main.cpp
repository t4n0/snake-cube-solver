#include "math_constants.h"
#include "quaternion.h"

#include <matplot/matplot.h>

#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

const math::Quaternion kOrigin{0.0, 0.0, 0.0};
const math::Quaternion kNullOrientation{1.0, 0.0, 0.0, 0.0};
const math::Quaternion kQuarterPitch = math::CreateRotation(math::TAU / 4.0, 0.0, 1.0, 0.0);
const math::Quaternion kQuarterRoll = math::CreateRotation(math::TAU / 4.0, 1.0, 0.0, 0.0);

struct Beam
{
    math::Quaternion orientation{kNullOrientation};  // Relative to parent system.
    math::Quaternion tip{kOrigin};                   // Local coordinates.
};

std::ostream& operator<<(std::ostream& ostream, const Beam& beam)
{
    ostream << "orientation: " << beam.orientation << '\n';
    ostream << "tip: " << beam.tip;
    return ostream;
}

void LogCube(const std::vector<Beam> cube)
{
    for (const auto& beam : cube)
    {
        std::cout << beam.orientation << '\n';
    }
}

std::vector<Beam> CreateFlatCube()
{
    constexpr std::array<double, 17> kBeamLengths{
        3.0, 3.0, 3.0, 3.0, 2.0, 2.0, 2.0, 3.0, 3.0, 2.0, 2.0, 3.0, 2.0, 3.0, 2.0, 2.0, 3.0};

    std::vector<Beam> cube{};
    cube.emplace_back(Beam{kNullOrientation, math::Quaternion{kBeamLengths.front(), 0.0, 0.0}});

    auto next_orientation = math::Quaternion{kQuarterPitch};
    for (std::size_t idx{1}; idx < kBeamLengths.size(); idx++)
    {
        next_orientation = next_orientation.Inverse();
        cube.emplace_back(Beam{next_orientation, math::Quaternion{kBeamLengths.at(idx), 0.0, 0.0}});
    }

    return cube;
}

std::tuple<std::vector<double>, std::vector<double>, std::vector<double>> GenerateVertices(
    const std::vector<Beam>& cube)
{
    std::vector<double> x{0.0};
    std::vector<double> y{0.0};
    std::vector<double> z{0.0};

    math::Quaternion current_location{0.0, 0.0, 0.0};        // In global coordinates.
    math::Quaternion current_orientation{kNullOrientation};  // In global coordinates.
    for (const auto& beam : cube)
    {
        current_orientation = beam.orientation.PrependAsLocalRotationAfter(current_orientation);
        current_location += beam.tip.RotateBy(current_orientation);
        const auto current_coordinates = current_location.GetVectorPart();
        x.push_back(current_coordinates.at(0));
        y.push_back(current_coordinates.at(1));
        z.push_back(current_coordinates.at(2));
    }

    return {x, y, z};
}

void Plot(const std::vector<Beam>& cube)
{
    const auto vertices = GenerateVertices(cube);
    const auto& x = std::get<0>(vertices);
    const auto& y = std::get<1>(vertices);
    const auto& z = std::get<2>(vertices);
    matplot::plot3(x, y, z);
    matplot::axis(matplot::equal);
    matplot::xlabel("x");
    matplot::ylabel("y");
    matplot::zlabel("z");
}

const int kTotalPossibleRotations{1073741824};  // = 4^15

void PerformQuarterRotations(std::vector<Beam>& cube, const std::size_t index)
{
    if (index >= (cube.size() - 1UL))
    {
        Plot(cube);
        return;
    }

    for (std::size_t quarter_rotations{0}; quarter_rotations < 4; quarter_rotations++)
    {
        PerformQuarterRotations(cube, index + 1);
        cube.at(index).orientation = kQuarterRoll.PrependAsLocalRotationAfter(cube.at(index).orientation);
    }
}

int main()
{
    std::cout << "Solving cube puzzle with brute force.\n";
    std::cout << "Attempting to calculate up to " << kTotalPossibleRotations << " solutions.\n";

    std::vector<Beam> cube = CreateFlatCube();

    matplot::show();

    PerformQuarterRotations(cube, 1);

    return 0;
}