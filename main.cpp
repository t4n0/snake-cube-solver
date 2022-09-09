#include "math_constants.h"
#include "quaternion.h"

#include <sciplot/sciplot.hpp>

#include <array>
#include <iostream>
#include <vector>

const math::Quaternion kOrigin{0.0, 0.0, 0.0};
const math::Quaternion kNullOrientation{1.0, 0.0, 0.0, 0.0};

math::Quaternion BackTransform(const math::Quaternion& origin_global,
                               const math::Quaternion& orientation_global,
                               const math::Quaternion& point_local)
{
    return origin_global + point_local.RotateBy(orientation_global.Inverse());
}

struct Beam
{
    math::Quaternion location{kOrigin};
    math::Quaternion orientation{kNullOrientation};
    double length{0.0};

    math::Quaternion GetEndOfBeamInLocalCoor() const noexcept { return math::Quaternion{length, 0.0, 0.0}; }
};

std::ostream& operator<<(std::ostream& ostream, const Beam& beam)
{
    ostream << "location: " << beam.location << '\n';
    ostream << "orientation: " << beam.orientation << '\n';
    ostream << "length: " << beam.length;
    return ostream;
}

std::vector<Beam> CreateFlatCube()
{
    constexpr std::array<double, 17> kBeamLengths{
        3.0, 3.0, 3.0, 3.0, 2.0, 2.0, 2.0, 3.0, 3.0, 2.0, 2.0, 3.0, 2.0, 3.0, 2.0, 2.0, 3.0};

    std::vector<Beam> cube{};
    auto next_location = math::Quaternion{kOrigin};
    auto next_orientation = math::Quaternion{kNullOrientation};
    double rotation_angle_sign = 1.0;
    for (const auto beam_length : kBeamLengths)
    {
        cube.emplace_back(Beam{next_location, next_orientation, beam_length});
        const auto& latest = cube.back();
        next_location = BackTransform(latest.location, latest.orientation, latest.GetEndOfBeamInLocalCoor());
        next_orientation =
            next_orientation * math::CreateRotation(rotation_angle_sign * math::TAU / 4.0, 0.0, 1.0, 0.0);
        rotation_angle_sign *= -1.0;
    }

    return cube;
}

int main()
{
    std::cout << "Solving cube puzzle with brute force.\n";
    std::cout << "Attempting to calculate up to 4.294.967.296 solutions.\n";  // = 4^16

    std::vector<Beam> cube = CreateFlatCube();

    std::vector<double> x{};
    std::vector<double> y{};
    std::vector<double> z{};

    for (const auto& beam : cube)
    {
        const auto beginning = beam.location.GetVectorPart();
        x.push_back(beginning.at(0));
        y.push_back(beginning.at(1));
        z.push_back(beginning.at(2));
    }

    const auto final_beam_end =
        BackTransform(cube.back().location, cube.back().orientation, cube.back().GetEndOfBeamInLocalCoor());
    const auto end = final_beam_end.GetVectorPart();
    x.push_back(end.at(0));
    y.push_back(end.at(1));
    z.push_back(end.at(2));

    // Plotting

    sciplot::Plot3D plot{};

    plot.xlabel("x");
    plot.ylabel("y");
    plot.zlabel("z");

    plot.border().clear();
    plot.border().bottomLeftFront();
    plot.border().bottomRightFront();
    plot.border().leftVertical();

    plot.palette("dark2");

    plot.drawCurve(x, y, z).label("cube").lineColor("red");

    sciplot::Figure fig{{plot}};
    sciplot::Canvas canvas{{fig}};
    canvas.show();

    return 0;
}