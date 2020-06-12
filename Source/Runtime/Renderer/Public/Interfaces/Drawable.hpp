#pragma once
#include <optional>
#include "Math.hpp"

namespace oeng
{
	class Material;
	
	class ISpriteComponent
	{
	public:
		struct DrawInfo { Mat4 transform; };
	
		[[nodiscard]] virtual std::optional<DrawInfo> Draw() const noexcept = 0;
		[[nodiscard]] virtual int GetDrawOrder() const noexcept = 0;

		constexpr ISpriteComponent() noexcept = default;
		virtual ~ISpriteComponent() = default;
		
		ISpriteComponent(const ISpriteComponent&) = delete;
		ISpriteComponent(ISpriteComponent&&) = delete;
		ISpriteComponent& operator=(const ISpriteComponent&) = delete;
		ISpriteComponent& operator=(ISpriteComponent&&) = delete;
	};

	class IMeshComponent
	{
	public:
		struct DrawInfo { Mat4 transform; size_t vertices{}; };

		[[nodiscard]] virtual std::optional<DrawInfo> Draw() const noexcept = 0;
		[[nodiscard]] virtual Material& GetMaterial() const noexcept = 0;
		[[nodiscard]] virtual Mesh& GetMesh() const noexcept = 0;

		constexpr IMeshComponent() noexcept = default;
		virtual ~IMeshComponent() = default;
		
		IMeshComponent(const IMeshComponent&) = delete;
		IMeshComponent(IMeshComponent&&) = delete;
		IMeshComponent& operator=(const IMeshComponent&) = delete;
		IMeshComponent& operator=(IMeshComponent&&) = delete;
	};
}
