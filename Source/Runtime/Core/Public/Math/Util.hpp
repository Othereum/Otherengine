#pragma once
#include "Angle.hpp"
#include "Quat.hpp"
#include "Vector.hpp"

namespace otm
{
template <class Float>
Angle<RadR> V2HFov(Angle<RadR> vfov, Vector<Float, 2> scr)
{
    return 2 * Atan(Tan(vfov / 2) * (scr[0] / scr[1]));
}

template <class Float>
Angle<RadR> H2VFov(Angle<RadR> hfov, Vector<Float, 2> scr)
{
    return 2 * Atan(Tan(hfov / 2) * (scr[1] / scr[0]));
}

[[nodiscard]] Angle<RadR, CommonFloat<T>> ToAngle() const noexcept
{
    auto& v = static_cast<const Vector<T, 2>&>(*this);
    return Atan2(v[1], v[0]);
}

template <class F>
[[nodiscard]] Vector<std::common_type_t<T, F>, 3> RotatedBy(const Quaternion<F>& q) const noexcept
{
    using Tf = std::common_type_t<T, F>;
    auto& vr = static_cast<const Vector<T, 3>&>(*this);
    const auto p = Quaternion<Tf>{Vector<Tf, 3>{vr}, 0};
    return (q * p * ~q).v;
}

template <class F>
void RotateBy(const Quaternion<F>& q) noexcept
{
    static_assert(
        std::is_same_v<std::remove_cvref_t<decltype(this->RotatedBy())>, std::remove_cvref_t<decltype(*this)>>);
    *this = this->RotatedBy(q);
}
} // namespace otm
