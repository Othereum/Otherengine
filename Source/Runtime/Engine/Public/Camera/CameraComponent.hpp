#pragma once
#include "Components/SceneComponent.hpp"

namespace oeng
{
inline namespace engine
{
class ENGINE_API CameraComponent final : public SceneComponent
{
CLASS_BODY(CameraComponent)

public:
    struct Data
    {
        Rad vfov = 60_deg;
        Float near = 10, far = 10000;
    };

    explicit CameraComponent(AActor& owner, int update_order = 100);
    ~CameraComponent();

    void SetData(const Data& data) noexcept
    {
        data_ = data;
        RecalcProj();
    }

    [[nodiscard]] const Data& GetData() const noexcept
    {
        return data_;
    }

    [[nodiscard]] const Vec3& GetPos() const noexcept;
    [[nodiscard]] const Mat4& GetViewProj() const noexcept;

    void OnScreenSizeChanged(Vec2u16 scr) noexcept;

protected:
    void OnTrsfChanged() noexcept override;
    void OnActivated() override;
    void OnDeactivated() override;

private:
    void RecalcView() noexcept;
    void RecalcProj() noexcept;

    Mat4 view_, proj_;
    Mat4 view_proj_;
    Data data_;
    Vec2 scr_;
};
}
}
