#include "math_constants.h"
#include "quaternion.h"

#include <matplot/matplot.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <thread>

const math::Quaternion kNullOrientation{1.0, 0.0, 0.0, 0.0};
const math::Quaternion kQuarterPitch{math::CreateRotation(math::TAU / 4.0, 0.0, 1.0, 0.0)};
const math::Quaternion kQuarterRoll{math::CreateRotation(math::TAU / 4.0, 1.0, 0.0, 0.0)};

struct Beam
{
    math::Quaternion orientation{kNullOrientation};  // Relative to parent system.
    int blocks{0};
};

std::ostream& operator<<(std::ostream& ostream, const Beam& beam)
{
    ostream << "orientation: " << beam.orientation << '\n';
    ostream << "blocks: " << beam.blocks;
    return ostream;
}

using Cube = std::array<Beam, 17>;

void LogCube(const Cube& cube)
{
    for (const auto& beam : cube)
    {
        std::cout << beam.orientation << '\n';
    }
}

Cube CreateFlatCube()
{
    constexpr std::array<int, 17> kBeamLengths{3, 3, 3, 3, 2, 2, 2, 3, 3, 2, 2, 3, 2, 3, 2, 2, 3};

    Cube cube{};
    cube.at(0) = Beam{kNullOrientation, kBeamLengths.front()};

    auto next_orientation = math::Quaternion{kQuarterPitch};
    for (std::size_t idx{1}; idx < kBeamLengths.size(); idx++)
    {
        next_orientation = next_orientation.Inverse();
        cube.at(idx) = Beam{next_orientation, kBeamLengths.at(idx)};
    }

    return cube;
}

using Vertices = std::tuple<std::array<double, 27>, std::array<double, 27>, std::array<double, 27>>;

void LogVertices(const Vertices& vertices)
{
    const auto& [x, y, z] = vertices;
    for (std::size_t index{0}; index < x.size(); index++)
    {
        std::cout << x.at(index) << " " << y.at(index) << " " << z.at(index) << '\n';
    }
    std::cout << std::endl;
}

Vertices GenerateVertices(const Cube& cube)
{
    Vertices x_y_z{};

    math::Quaternion current_location{0.0, 0.0, 0.0};        // In global coordinates.
    math::Quaternion current_orientation{kNullOrientation};  // In global coordinates.
    std::size_t index{0};
    for (const auto& beam : cube)
    {
        current_orientation = beam.orientation.AppendAsLocalRotationAfter(current_orientation);
        const auto single_block = math::Quaternion{1.0, 0.0, 0.0}.RotateBy(current_orientation);
        for (int blocks{1}; blocks < beam.blocks; blocks++)
        {
            current_location += single_block;
            const auto current_coordinates = current_location.GetVectorPart();
            std::get<0>(x_y_z).at(index) = current_coordinates.at(0);
            std::get<1>(x_y_z).at(index) = current_coordinates.at(1);
            std::get<2>(x_y_z).at(index) = current_coordinates.at(2);
            index++;
        }
    }

    return x_y_z;
}

void Plot(const Cube& cube)
{
    const auto vertices = GenerateVertices(cube);
    const auto& [x, y, z] = vertices;
    matplot::plot3(x, y, z);
    matplot::axis(matplot::equal);
    matplot::xlabel("x");
    matplot::ylabel("y");
    matplot::zlabel("z");
}

const int kTotalPossibleRotations{1073741824};  // = 4^15

bool VerticesFitIn3by3Box(const Vertices& vertices)
{
    const auto& [x, y, z] = vertices;
    const auto x_minmax = std::minmax_element(x.begin(), x.end());
    const auto x_diff = (*x_minmax.second) - (*x_minmax.first);
    if (x_diff > 2.01)
    {
        return false;
    }

    const auto y_minmax = std::minmax_element(y.begin(), y.end());
    const auto y_diff = (*y_minmax.second) - (*y_minmax.first);
    if (y_diff > 2.01)
    {
        return false;
    }

    const auto z_minmax = std::minmax_element(z.begin(), z.end());
    const auto z_diff = (*z_minmax.second) - (*z_minmax.first);
    if (z_diff > 2.01)
    {
        return false;
    }

    return true;
}

double SquaredDistance(const double x1,
                       const double y1,
                       const double z1,
                       const double x2,
                       const double y2,
                       const double z2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2);
}

bool VerticesDontCollide(const Vertices& vertices)
{
    const auto& [x, y, z] = vertices;
    const auto number_of_vertices = std::get<0>(vertices).size();
    for (std::size_t j{}; j < number_of_vertices; j++)
    {
        for (std::size_t k{}; k < number_of_vertices; k++)
        {
            if ((j != k) && (SquaredDistance(x.at(j), y.at(j), z.at(j), x.at(k), y.at(k), z.at(k)) < 0.9))
            {
                return false;
            }
        }
    }

    return true;
}

void PerformQuarterRotations(Cube& cube, const std::size_t index)
{
    if (index >= (cube.size() - 1UL))
    {
        const auto vertices = GenerateVertices(cube);
        if (VerticesFitIn3by3Box(vertices) && VerticesDontCollide(vertices))
        {
            Plot(cube);
        }
        return;
    }

    if (index == 1UL)
    {
        std::cout << "Done here ---> |" << std::endl;
    }

    for (std::size_t quarter_rotations{0}; quarter_rotations < 4; quarter_rotations++)
    {
        PerformQuarterRotations(cube, index + 1);
        cube.at(index).orientation = kQuarterRoll.AppendAsLocalRotationAfter(cube.at(index).orientation);
        if (index == 2UL)
        {
            std::cout << "." << std::flush;
        }
    }

    if (index == 1UL)
    {
        std::cout << std::endl;
    }
}

int main()
{
    std::cout << "Solving cube puzzle with brute force.\n";
    std::cout.imbue(std::locale(""));
    std::cout << "Attempting to calculate up to " << kTotalPossibleRotations << " solutions.\n";

    auto cube = CreateFlatCube();

    matplot::show();

    const auto t0 = std::chrono::steady_clock::now();
    PerformQuarterRotations(cube, 1);
    const auto t1 = std::chrono::steady_clock::now();

    std::cout << "Time spent: " << std::chrono::duration_cast<std::chrono::seconds>(t1 - t0).count() << "secs\n";

    return 0;
}