#pragma once
#include <filesystem>
#include "JsonFwd.hpp"

namespace oeng
{
	struct Path
	{
		Path() noexcept;
		Path(const std::filesystem::path& path);

		bool operator==(const Path&) const noexcept = default;
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
