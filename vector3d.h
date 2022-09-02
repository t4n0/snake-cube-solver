#ifndef VECTOR3D_H
#define VECTOR3D_H

namespace math
{

class Vector3D
{
  public:
    Vector3D() : x_{}, y_{}, z_{} {}
    Vector3D(const double x, const double y, const double z) : x_{x}, y_{y}, z_{z} {}

    Vector3D& operator+=(const Vector3D& rhs)
    {
        x_ += rhs.x_;
        y_ += rhs.y_;
        z_ += rhs.z_;
        return *this;
    }

    Vector3D& operator-=(const Vector3D& rhs)
    {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        z_ -= rhs.z_;
        return *this;
    }

  private:
    double x_;
    double y_;
    double z_;

    friend bool operator==(const Vector3D& lhs, const Vector3D& rhs);
    friend Vector3D operator*(Vector3D lhs, const double factor);
    friend Vector3D operator*(const double factor, Vector3D rhs);
};

inline bool operator==(const Vector3D& lhs, const Vector3D& rhs)
{
    return (lhs.x_ == rhs.x_) &&  //
           (lhs.y_ == rhs.y_) &&  //
           (lhs.z_ == rhs.z_);
}

inline bool operator!=(const Vector3D& lhs, const Vector3D& rhs)
{
    return !(lhs == rhs);
}

inline Vector3D operator+(Vector3D lhs, const Vector3D& rhs)
{
    lhs += rhs;
    return lhs;
}

inline Vector3D operator-(Vector3D lhs, const Vector3D& rhs)
{
    lhs -= rhs;
    return lhs;
}

inline Vector3D operator*(Vector3D lhs, const double factor)
{
    lhs.x_ *= factor;
    lhs.y_ *= factor;
    lhs.z_ *= factor;
    return lhs;
}

inline Vector3D operator*(const double factor, Vector3D rhs)
{
    return rhs * factor;
}

}  // namespace math

#endif  // VECTOR3D_H