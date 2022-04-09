#include "quaternion.h"

namespace math
{

std::ostream& operator<<(std::ostream& stream, const Quaternion& value)
{
    stream << "w: " << value.w << ", i: " << value.i << ", j: " << value.j << ", k: " << value.k;
    return stream;
}

bool operator==(const Quaternion& lhs, const Quaternion& rhs) noexcept
{
    return (lhs.w == rhs.w) &&  //
           (lhs.i == rhs.i) &&  //
           (lhs.j == rhs.j) &&  //
           (lhs.k == rhs.k);
}

bool operator!=(const Quaternion& lhs, const Quaternion& rhs) noexcept
{
    return !(lhs == rhs);
}

// Basic rules of quaternion multiplication:
// i^2 = j^2 = k^2 = -1
// ij = -ji = k
// ki = -ik = j
// jk = -kj = i
Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{
    return {lhs.w * rhs.w - lhs.i * rhs.i - lhs.j * rhs.j - lhs.k * rhs.k,
            lhs.w * rhs.i + lhs.i * rhs.w + lhs.j * rhs.k - lhs.k * rhs.j,
            lhs.w * rhs.j + lhs.j * rhs.w + lhs.k * rhs.i - lhs.i * rhs.k,
            lhs.w * rhs.k + lhs.k * rhs.w + lhs.i * rhs.j - lhs.j * rhs.i};
}

}  // namespace math