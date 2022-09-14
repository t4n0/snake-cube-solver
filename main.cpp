#include "math_constants.h"
#include "quaternion.h"

#include <matplot/matplot.h>

#include <array>
#include <iostream>
#include <vector>

const math::Quaternion kOrigin{0.0, 0.0, 0.0};
const math::Quaternion kNullOrientation{1.0, 0.0, 0.0, 0.0};
const math::Quaternion kQuarterPitch = math::CreateRotation(math::TAU / 4.0, 0.0, 1.0, 0.0);

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

    std::cout << "Created flat cube with these beam orientations:\n";
    for (const auto& beam : cube)
    {
        std::cout << beam.orientation << '\n';
    }

    return cube;
}

int main()
{
    std::cout << "Solving cube puzzle with brute force.\n";
    std::cout << "Attempting to calculate up to 4.294.967.296 solutions.\n";  // = 4^16

    std::vector<Beam> cube = CreateFlatCube();

    std::vector<double> x{0.0};
    std::vector<double> y{0.0};
    std::vector<double> z{0.0};

    math::Quaternion current_location{0.0, 0.0, 0.0};        // In global coordinates.
    math::Quaternion current_orientation{kNullOrientation};  // In global coordinates.
    for (const auto& beam : cube)
    {
        current_orientation = beam.orientation * current_orientation;
        current_location += beam.tip.RotateBy(current_orientation);
        const auto current_coordinates = current_location.GetVectorPart();
        x.push_back(current_coordinates.at(0));
        y.push_back(current_coordinates.at(1));
        z.push_back(current_coordinates.at(2));
    }

    matplot::show();
    matplot::plot3(x, y, z);

    return 0;
}