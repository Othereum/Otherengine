#pragma once

namespace oeng
{
inline namespace engine
{
class AActor;
class World;

class ENGINE_API ActorComponent : public Object
{
CLASS_BODY(ActorComponent)

public:
    ActorComponent() = default;

    /**
     * Activate the component.
     * It also enables the component update.
     */
    void Activate();

    /**
     * Deactivate the component.
     * It also disables the component update.
     */
    void Deactivate();

    /**
     * Check if the component is active.
     * It also indicates whether the component is being updated.
     */
    [[nodiscard]] bool IsActive() const noexcept
    {
        return is_active_;
    }

    [[nodiscard]] bool HasBegunPlay() const noexcept
    {
        return begun_play_;
    }

    [[nodiscard]] int GetUpdateOrder() const noexcept
    {
        return update_order_;
    }

    [[nodiscard]] AActor& GetOwner() const noexcept
    {
        return *owner_;
    }

    [[nodiscard]] World& GetWorld() const noexcept;

    /**
     * If true, the component will be activated automatically on begin play.
     * `true` by default.
     */
    bool auto_activate : 1 = true;

protected:
    /**
     * Used to override the default update order.
     */
    explicit ActorComponent(int update_order)
        : update_order_{update_order}
    {
    }

    virtual void OnBeginPlay()
    {
    }

    virtual void OnEndPlay()
    {
    }

    virtual void OnUpdate([[maybe_unused]] Float delta_seconds)
    {
    }

    virtual void OnActivated()
    {
    }

    virtual void OnDeactivated()
    {
    }

private:
    friend AActor;

    void BeginPlay();
    void EndPlay();
    void Update(Float delta_seconds);

    AActor* owner_ = nullptr;
    int update_order_ = 100;

    bool is_active_ : 1 = false;
    bool begun_play_ : 1 = false;
};
}
}
