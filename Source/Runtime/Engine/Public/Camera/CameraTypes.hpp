#pragma once

namespace oeng
{
inline namespace engine
{
struct ViewInfo
{
    Vec3 origin;
    UVec3 direction = UVec3::Forward();
    Rad vfov = 60_deg;
    Float near = 10;
    Float far = 10000;
};
}
}
