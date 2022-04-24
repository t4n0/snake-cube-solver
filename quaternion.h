#ifndef QUATERNION_H
#define QUATERNION_H

#include <ostream>

namespace math
{

class Quaternion
{
  public:
    Quaternion(const double w, const double i, const double j, const double k);
    Quaternion(const double i, const double j, const double k);

    Quaternion Inverse() const;

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

}  // namespace math

#endif  // QUATERNION_H