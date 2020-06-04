#pragma once
#include <compare>
#include <filesystem>
#include "API.hpp"
#include "JsonFwd.hpp"

namespace oeng
{
	/**
	 * \brief Lightweight representation of filepath
	 * \brief Very fast O(1) copy and comparison
	 * \brief No heap allocation on copy
	 * \brief Good to use as key for HashMap
	 * \note Case-insensitive on any platform
	 */
	struct OEAPI Path
	{
		Path() noexcept;
		Path(const char* path);
		Path(const std::filesystem::path& path);

		std::strong_ordering operator<=>(const Path&) const noexcept = default;

		operator const std::filesystem::path&() const noexcept { return *p; }
		const std::filesystem::path* operator->() const noexcept { return p; }
		[[nodiscard]] const std::filesystem::path& Get() const noexcept { return *p; }

	private:
		friend std::hash<Path>;
		const std::filesystem::path* p;
	};
	
	void to_json(Json& json, const Path& path);
	void from_json(const Json& json, Path& path);
}

template <>
struct std::hash<oeng::Path>
{
	size_t operator()(const oeng::Path& key) const noexcept
	{
		return reinterpret_cast<size_t>(key.p);
	}
};
