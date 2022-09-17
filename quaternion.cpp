#include "quaternion.h"

#include <cmath>

namespace math
{

Quaternion::Quaternion(const double w, const double i, const double j, const double k) : w_{w}, i_{i}, j_{j}, k_{k} {}

Quaternion::Quaternion(const double x, const double y, const double z) : w_{0.0}, i_{x}, j_{y}, k_{z} {}

Quaternion Quaternion::Inverse() const
{
    const double normalization = w_ * w_ + i_ * i_ + j_ * j_ + k_ * k_;
    return {w_ / normalization, -i_ / normalization, -j_ / normalization, -k_ / normalization};
}

Quaternion Quaternion::RotateBy(const Quaternion& rotation) const
{
    return rotation * (*this) * rotation.Inverse();
}

Quaternion Quaternion::AppendAsLocalRotationAfter(const Quaternion& preceding_rotation) const
{
    const auto global_rotation = (*this).RotateBy(preceding_rotation);
    return global_rotation * preceding_rotation;
}

double Quaternion::GetScalarPart() const
{
    return w_;
}

double Quaternion::GetX() const
{
    return i_;
}
double Quaternion::GetY() const
{
    return j_;
}
double Quaternion::GetZ() const
{
    return k_;
}

Quaternion& Quaternion::operator+=(const Quaternion& rhs) noexcept
{
    w_ += rhs.w_;
    i_ += rhs.i_;
    j_ += rhs.j_;
    k_ += rhs.k_;
    return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& rhs) noexcept
{
    w_ -= rhs.w_;
    i_ -= rhs.i_;
    j_ -= rhs.j_;
    k_ -= rhs.k_;
    return *this;
}

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

Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{
    return {lhs.w_ * rhs.w_ - lhs.i_ * rhs.i_ - lhs.j_ * rhs.j_ - lhs.k_ * rhs.k_,
            lhs.w_ * rhs.i_ + lhs.i_ * rhs.w_ + lhs.j_ * rhs.k_ - lhs.k_ * rhs.j_,
            lhs.w_ * rhs.j_ + lhs.j_ * rhs.w_ + lhs.k_ * rhs.i_ - lhs.i_ * rhs.k_,
            lhs.w_ * rhs.k_ + lhs.k_ * rhs.w_ + lhs.i_ * rhs.j_ - lhs.j_ * rhs.i_};
}

Quaternion operator+(Quaternion lhs, const Quaternion& rhs) noexcept
{
    lhs += rhs;
    return lhs;
}

Quaternion operator-(Quaternion lhs, const Quaternion& rhs) noexcept
{
    lhs -= rhs;
    return lhs;
}

Quaternion CreateRotation(const double angle, const double x, const double y, const double z)
{
    const double half_angle = angle / 2.0;
    const double scalar_factor = std::cos(half_angle);
    const double normalization = std::sqrt(x * x + y * y + z * z);
    const double vector_factor = std::sin(half_angle) / normalization;
    return {scalar_factor, x * vector_factor, y * vector_factor, z * vector_factor};
}

}  // namespace math