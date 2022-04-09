#ifndef QUATERNION_H
#define QUATERNION_H

#include <ostream>

namespace math
{

struct Quaternion
{
    double w;
    double i;
    double j;
    double k;
};

std::ostream& operator<<(std::ostream& stream, const Quaternion& value);
bool operator==(const Quaternion& lhs, const Quaternion& rhs) noexcept;
bool operator!=(const Quaternion& lhs, const Quaternion& rhs) noexcept;
Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);

}  // namespace math

#endif  // QUATERNION_H