#pragma once

class Vector3d
{
public:
    constexpr Vector3d() noexcept = default;

    constexpr Vector3d(
        double x,
        double y,
        double z) noexcept
        : x_(x),
          y_(y),
          z_(z)
    {
    }

    [[nodiscard]]
    constexpr double x() const noexcept
    {
        return x_;
    }

    [[nodiscard]]
    constexpr double y() const noexcept
    {
        return y_;
    }

    [[nodiscard]]
    constexpr double z() const noexcept
    {
        return z_;
    }

private:
    double x_{};
    double y_{};
    double z_{};
};
