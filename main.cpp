#include "math_constants.h"
#include "quaternion.h"

#include <matplot/matplot.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <thread>

const math::Quaternion kOrigin{0.0, 0.0, 0.0};
const math::Quaternion kNullOrientation{1.0, 0.0, 0.0, 0.0};
const math::Quaternion kQuarterPitch{math::CreateRotation(math::TAU / 4.0, 0.0, 1.0, 0.0)};
const math::Quaternion kQuarterRoll{math::CreateRotation(math::TAU / 4.0, 1.0, 0.0, 0.0)};
const math::Quaternion kNextBlock{1.0, 0.0, 0.0};

struct Block
{
    math::Quaternion local_orientation{kNullOrientation};  // Relative to previous block.
    math::Quaternion global_orientation{kOrigin};
    math::Quaternion global_location{kOrigin};
    bool may_roll{false};
};

std::ostream& operator<<(std::ostream& ostream, const Block& block)
{
    ostream << "local_orientation: " << block.local_orientation << '\n';
    ostream << "global_orientation: " << block.global_orientation << '\n';
    ostream << "global_location: " << block.global_location << '\n';
    ostream << "may_roll: " << block.may_roll << '\n';
    return ostream;
}

using Cube = std::array<Block, 27>;

void LogCube(const Cube& cube)
{
    for (std::size_t index{0}; index < cube.size(); index++)
    {
        std::cout << index << '\n' << cube.at(index) << '\n';
    }
    std::cout << std::endl;
}

Cube CreateFlatCube()
{
    const math::Quaternion invalid{0.0, 0.0, 0.0};
    const auto start_block = Block{kNullOrientation, kNullOrientation, kOrigin, false};
    const auto passive_block = Block{kNullOrientation, invalid, invalid, false};
    const auto quarter_pitched_pos = Block{kQuarterPitch, invalid, invalid, true};
    const auto quarter_pitched_neg = Block{kQuarterPitch.Inverse(), invalid, invalid, true};
    const auto passive_quarter_pitched_pos = Block{kQuarterPitch, invalid, invalid, false};

    Cube cube{};

    // Fill local orientations. These won't change.
    cube.at(0) = start_block;  // 1 (Rolling would rotate entire cube.)
    cube.at(1) = passive_block;
    cube.at(2) = passive_block;
    cube.at(3) = quarter_pitched_neg;  // 2
    cube.at(4) = passive_block;
    cube.at(5) = quarter_pitched_pos;  // 3
    cube.at(6) = passive_block;
    cube.at(7) = quarter_pitched_neg;  // 4
    cube.at(8) = passive_block;
    cube.at(9) = quarter_pitched_pos;   // 5
    cube.at(10) = quarter_pitched_neg;  // 6
    cube.at(11) = quarter_pitched_pos;  // 7
    cube.at(12) = quarter_pitched_neg;  // 8
    cube.at(13) = passive_block;
    cube.at(14) = quarter_pitched_pos;  // 9
    cube.at(15) = passive_block;
    cube.at(16) = quarter_pitched_neg;  // 10
    cube.at(17) = quarter_pitched_pos;  // 11
    cube.at(18) = quarter_pitched_neg;  // 12
    cube.at(19) = passive_block;
    cube.at(20) = quarter_pitched_pos;  // 13
    cube.at(21) = quarter_pitched_neg;  // 14
    cube.at(22) = passive_block;
    cube.at(23) = quarter_pitched_pos;          // 15
    cube.at(24) = quarter_pitched_neg;          // 16
    cube.at(25) = passive_quarter_pitched_pos;  // 17 (Rolling would have no effect.)
    cube.at(26) = passive_block;

    // Calculate global poses.
    for (std::size_t index{1}; index < cube.size(); index++)
    {
        cube.at(index).global_orientation =
            cube.at(index).local_orientation.AppendAsLocalRotationAfter(cube.at(index - 1).global_orientation);
        cube.at(index).global_location =
            cube.at(index - 1).global_location + kNextBlock.RotateBy(cube.at(index).global_orientation);
    }

    return cube;
}

using Vertices = std::tuple<std::array<double, 27>, std::array<double, 27>, std::array<double, 27>>;

Vertices UnweaveCoordinates(const Cube& cube)
{
    Vertices x_y_z{};
    for (std::size_t index{0}; index < cube.size(); index++)
    {
        const auto current_coordinates = cube.at(index).global_location.GetVectorPart();
        std::get<0>(x_y_z).at(index) = current_coordinates.at(0);
        std::get<1>(x_y_z).at(index) = current_coordinates.at(1);
        std::get<2>(x_y_z).at(index) = current_coordinates.at(2);
    }

    return x_y_z;
}

void Plot(const Cube& cube)
{
    const auto [x, y, z] = UnweaveCoordinates(cube);

    static auto figure = matplot::figure();
    auto ax = matplot::gca();
    auto l = ax->plot3(x, y, z, "");
    l->line_width(3);
    ax->axis(matplot::equal);

    const float start_azimuth = ax->azimuth();
    int delta_azimuth{0};
    while (delta_azimuth < 720)
    {
        ax->azimuth(start_azimuth + (delta_azimuth % 360));
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        delta_azimuth++;
    }
}

const int kTotalPossibleRotations{1073741824};  // = 4^15

double GetX(const Block& block)
{
    return std::get<0>(block.global_location.GetVectorPart());
}

double GetY(const Block& block)
{
    return std::get<1>(block.global_location.GetVectorPart());
}

double GetZ(const Block& block)
{
    return std::get<2>(block.global_location.GetVectorPart());
}

bool BlocksFitIn3by3Box(const Cube& cube)
{
    const auto x_minmax =
        std::minmax_element(cube.begin(), cube.end(), [](const Block& a, const Block& b) { return GetX(a) < GetX(b); });
    const auto x_diff = GetX(*x_minmax.second) - GetX(*x_minmax.first);
    if (x_diff > 2.01)
    {
        return false;
    }

    const auto y_minmax =
        std::minmax_element(cube.begin(), cube.end(), [](const Block& a, const Block& b) { return GetY(a) < GetY(b); });
    const auto y_diff = GetY(*y_minmax.second) - GetY(*y_minmax.first);
    if (y_diff > 2.01)
    {
        return false;
    }

    const auto z_minmax =
        std::minmax_element(cube.begin(), cube.end(), [](const Block& a, const Block& b) { return GetZ(a) < GetZ(b); });
    const auto z_diff = GetZ(*z_minmax.second) - GetZ(*z_minmax.first);
    if (z_diff > 2.01)
    {
        return false;
    }

    return true;
}

double GetX(const math::Quaternion& location)
{
    return std::get<0>(location.GetVectorPart());
}

double GetY(const math::Quaternion& location)
{
    return std::get<1>(location.GetVectorPart());
}

double GetZ(const math::Quaternion& location)
{
    return std::get<2>(location.GetVectorPart());
}

double SquaredDistance(const math::Quaternion q1, const math::Quaternion q2)
{
    const auto diff = q1 - q2;
    return GetX(diff) * GetX(diff) +  //
           GetY(diff) * GetY(diff) +  //
           GetZ(diff) * GetZ(diff);
}

bool BlocksDontCollide(const Cube& cube)
{
    for (std::size_t j{}; j < cube.size(); j++)
    {
        for (std::size_t k{}; k < cube.size(); k++)
        {
            if ((j != k) && (SquaredDistance(cube.at(j).global_location, cube.at(k).global_location) < 0.9))
            {
                return false;
            }
        }
    }

    return true;
}

void PrintProgressTitle(const std::size_t index)
{
    if (index == 1)
    {
        std::cout << "Done here ---> |" << std::endl;
    }
}
void PrintProgress(const std::size_t index)
{
    if (index == 5)
    {
        std::cout << "." << std::flush;
    }
}
void WrapUpProgressBar(const std::size_t index)
{
    if (index == 1)
    {
        std::cout << std::endl;
    }
}

void PerformQuarterRotations(Cube& cube, const std::size_t index)
{
    cube.at(index).global_orientation =
        cube.at(index).local_orientation.AppendAsLocalRotationAfter(cube.at(index - 1).global_orientation);
    cube.at(index).global_location =
        cube.at(index - 1).global_location + kNextBlock.RotateBy(cube.at(index).global_orientation);

    const bool is_last_block{index >= (cube.size() - 1)};
    if (is_last_block)
    {
        if (BlocksFitIn3by3Box(cube) && BlocksDontCollide(cube))
        {
            Plot(cube);
        }
        return;
    }

    PrintProgressTitle(index);

    if (cube.at(index).may_roll)
    {
        for (std::size_t quarter_rotations{0}; quarter_rotations < 4; quarter_rotations++)
        {
            PerformQuarterRotations(cube, index + 1);

            cube.at(index).local_orientation =
                kQuarterRoll.AppendAsLocalRotationAfter(cube.at(index).local_orientation);
            cube.at(index).global_orientation =
                cube.at(index).local_orientation.AppendAsLocalRotationAfter(cube.at(index - 1).global_orientation);

            PrintProgress(index);
        }
    }
    else
    {
        PerformQuarterRotations(cube, index + 1);
    }

    WrapUpProgressBar(index);
}

int main()
{
    std::cout << "Solving cube puzzle with brute force.\n";
    std::cout.imbue(std::locale(""));
    std::cout << "Attempting to calculate up to " << kTotalPossibleRotations << " solutions.\n";

    auto cube = CreateFlatCube();
    Plot(cube);

    const auto t0 = std::chrono::steady_clock::now();
    PerformQuarterRotations(cube, 1);
    const auto t1 = std::chrono::steady_clock::now();

    std::cout << "Time spent: " << std::chrono::duration_cast<std::chrono::seconds>(t1 - t0).count() << "secs\n";

    return 0;
}