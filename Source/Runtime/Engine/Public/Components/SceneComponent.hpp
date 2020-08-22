#pragma once
#include "ActorComponent.hpp"

namespace oeng::engine
{
enum class AttachRule
{
    kKeepRelative,
    kKeepWorld
};

class ENGINE_API SceneComponent : public ActorComponent
{
public:
    explicit SceneComponent(AActor& owner, int update_order = 100);

    void AttachTo(SceneComponent* new_parent, AttachRule rule);

    void SetRelTrsf(const Transform& trsf) noexcept
    {
        rel_trsf_ = trsf;
        RecalcWorldTrsf();
    }

    void SetRelPos(const Vec3& pos) noexcept
    {
        rel_trsf_.pos = pos;
        RecalcWorldTrsf();
    }

    void SetRelRot(const Quat& rot) noexcept
    {
        rel_trsf_.rot = rot;
        RecalcWorldTrsf();
    }

    void SetRelScale(const Vec3& scale) noexcept
    {
        rel_trsf_.scale = scale;
        RecalcWorldTrsf();
    }

    void SetWorldTrsf(const Transform& trsf) noexcept
    {
        world_trsf_ = trsf;
        RecalcRelTrsf();
    }

    void SetWorldPos(const Vec3& pos) noexcept
    {
        world_trsf_.pos = pos;
        RecalcRelTrsf();
    }

    void SetWorldRot(const Quat& rot) noexcept
    {
        world_trsf_.rot = rot;
        RecalcRelTrsf();
    }

    void SetWorldScale(const Vec3& scale) noexcept
    {
        world_trsf_.scale = scale;
        RecalcRelTrsf();
    }

    [[nodiscard]] const Transform& GetRelTrsf() const noexcept
    {
        return rel_trsf_;
    }

    [[nodiscard]] const Vec3& GetRelPos() const noexcept
    {
        return rel_trsf_.pos;
    }

    [[nodiscard]] const Quat& GetRelRot() const noexcept
    {
        return rel_trsf_.rot;
    }

    [[nodiscard]] const Vec3& GetRelScale() const noexcept
    {
        return rel_trsf_.scale;
    }

    [[nodiscard]] const Transform& GetWorldTrsf() const noexcept
    {
        return world_trsf_;
    }

    [[nodiscard]] const Vec3& GetWorldPos() const noexcept
    {
        return world_trsf_.pos;
    }

    [[nodiscard]] const Quat& GetWorldRot() const noexcept
    {
        return world_trsf_.rot;
    }

    [[nodiscard]] const Vec3& GetWorldScale() const noexcept
    {
        return world_trsf_.scale;
    }

    [[nodiscard]] UVec3 GetForward() const noexcept
    {
        return UVec3::forward.RotatedBy(world_trsf_.rot);
    }

    [[nodiscard]] UVec3 GetBackward() const noexcept
    {
        return UVec3::backward.RotatedBy(world_trsf_.rot);
    }

    [[nodiscard]] UVec3 GetRight() const noexcept
    {
        return UVec3::right.RotatedBy(world_trsf_.rot);
    }

    [[nodiscard]] UVec3 GetLeft() const noexcept
    {
        return UVec3::left.RotatedBy(world_trsf_.rot);
    }

    [[nodiscard]] UVec3 GetUp() const noexcept
    {
        return UVec3::up.RotatedBy(world_trsf_.rot);
    }

    [[nodiscard]] UVec3 GetDown() const noexcept
    {
        return UVec3::down.RotatedBy(world_trsf_.rot);
    }

    [[nodiscard]] const Mat4& GetWorldTrsfMatrix() const noexcept
    {
        return world_mat_;
    }

protected:
    virtual void OnTrsfChanged()
    {
    }

private:
    // Recalculate world transform with relative transform
    void RecalcWorldTrsf() noexcept;

    // Recalculate relative transform with world transform
    void RecalcRelTrsf() noexcept;

    SceneComponent* parent_;
    std::vector<std::reference_wrapper<SceneComponent>> children_;
    Transform rel_trsf_;
    Transform world_trsf_;
    Mat4 world_mat_ = Mat4::identity;
};
}
