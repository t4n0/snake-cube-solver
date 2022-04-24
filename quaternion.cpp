#include "quaternion.h"

namespace math
{

Quaternion::Quaternion(const double w, const double i, const double j, const double k) : w_{w}, i_{i}, j_{j}, k_{k} {}

Quaternion::Quaternion(const double i, const double j, const double k) : w_{0.0}, i_{i}, j_{j}, k_{k} {}

std::ostream& operator<<(std::ostream& stream, const Quaternion& value)
{
    stream << "w: " << value.w_ << ", i: " << value.i_ << ", j: " << value.j_ << ", k: " << value.k_;
    return stream;
}

bool operator==(const Quaternion& lhs, const Quaternion& rhs) noexcept
{
    return (lhs.w_ == rhs.w_) &&  //
           (lhs.i_ == rhs.i_) &&  //
           (lhs.j_ == rhs.j_) &&  //
           (lhs.k_ == rhs.k_);
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
    return {lhs.w_ * rhs.w_ - lhs.i_ * rhs.i_ - lhs.j_ * rhs.j_ - lhs.k_ * rhs.k_,
            lhs.w_ * rhs.i_ + lhs.i_ * rhs.w_ + lhs.j_ * rhs.k_ - lhs.k_ * rhs.j_,
            lhs.w_ * rhs.j_ + lhs.j_ * rhs.w_ + lhs.k_ * rhs.i_ - lhs.i_ * rhs.k_,
            lhs.w_ * rhs.k_ + lhs.k_ * rhs.w_ + lhs.i_ * rhs.j_ - lhs.j_ * rhs.i_};
}

}  // namespace math