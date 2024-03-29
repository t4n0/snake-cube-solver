#ifndef QUATERNION_H
#define QUATERNION_H

#include <array>
#include <ostream>

namespace math
{

class Quaternion
{
  public:
    Quaternion(const double w, const double i, const double j, const double k);
    Quaternion(const double x, const double y, const double z);

    Quaternion Inverse() const;
    Quaternion RotateBy(const Quaternion& rotation) const;
    Quaternion AppendAsLocalRotationAfter(const Quaternion& preceding_rotation) const;
    double GetScalarPart() const;
    double GetX() const;
    double GetY() const;
    double GetZ() const;

    Quaternion& operator+=(const Quaternion& rhs) noexcept;
    Quaternion& operator-=(const Quaternion& rhs) noexcept;

  private:
    double w_;
    double i_;
    double j_;
    double k_;

    friend std::ostream& operator<<(std::ostream& stream, const Quaternion& value);
    friend bool operator==(const Quaternion& lhs, const Quaternion& rhs) noexcept;
    friend bool operator!=(const Quaternion& lhs, const Quaternion& rhs) noexcept;
    friend Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);
};

std::ostream& operator<<(std::ostream& stream, const Quaternion& value);
bool operator==(const Quaternion& lhs, const Quaternion& rhs) noexcept;
bool operator!=(const Quaternion& lhs, const Quaternion& rhs) noexcept;
Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);
Quaternion operator+(Quaternion lhs, const Quaternion& rhs) noexcept;
Quaternion operator-(Quaternion lhs, const Quaternion& rhs) noexcept;

Quaternion CreateRotation(const double angle, const double x, const double y, const double z);

}  // namespace math

#endif  // QUATERNION_H