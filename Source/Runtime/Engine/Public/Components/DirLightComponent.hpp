#pragma once
#include "Components/SceneComponent.hpp"
#include "Interfaces/Light.hpp"

namespace oeng
{
	class OEAPI DirLightComponent : public SceneComponent, public IDirLight
	{
	public:
		explicit DirLightComponent(AActor& owner, int update_order = 100);
		~DirLightComponent();

		void Activate() const noexcept;
		
		const Data& GetData() const noexcept override { return data_; }
		
		void SetColor(const Vec3& color) noexcept { data_.color = color; }
		[[nodiscard]] const Vec3& GetColor() const noexcept { return data_.color; }

		DirLightComponent(const DirLightComponent&) = delete;
		DirLightComponent(DirLightComponent&&) = delete;
		DirLightComponent& operator=(const DirLightComponent&) = delete;
		DirLightComponent& operator=(DirLightComponent&&) = delete;
		
	private:
		void OnTrsfChanged() override { data_.dir = GetForward(); }
		
		Data data_;
	};
}