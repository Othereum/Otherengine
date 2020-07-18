#pragma once
#include "Path.hpp"

namespace oeng::core
{
	class CORE_API Asset
	{
	public:
		~Asset() = default;
		Asset() noexcept = default;
		Asset(const Asset&) = default;
		Asset(Asset&& r) noexcept :path_{r.path_}, stem_{r.stem_} { r.path_ = {}; r.stem_ = {}; }
		explicit Asset(Path path) noexcept :path_{path}, stem_{path->stem().u8string()} {}

		Asset& operator=(const Asset&) = default;
		Asset& operator=(Asset&& r) noexcept { Asset{std::move(r)}.swap(*this); return *this; }

		void swap(Asset& r) noexcept { using std::swap; swap(path_, r.path_); }

		[[nodiscard]] Path GetPath() const noexcept { return path_; }
		[[nodiscard]] Name GetStem() const noexcept { return stem_; }
		
	private:
		Path path_;
		Name stem_;
	};

	inline void swap(Asset& a, Asset& b) noexcept { a.swap(b); }
}
