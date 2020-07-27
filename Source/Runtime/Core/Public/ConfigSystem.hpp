#pragma once
#include "Json.hpp"
#include "Name.hpp"
#include "Templates/HashMap.hpp"

namespace oeng::core
{
	class CORE_API ConfigSystem
	{
	public:
		[[nodiscard]] static ConfigSystem& Get() noexcept;
		
		/**
		 * Find config or create if not found.
		 * @param name Config name
		 * @return Reference to non-const json config object
		 */
		[[nodiscard]] Json& operator()(Name name) { return configs_[name]; }
		
		/**
		 * Save config as file into the user data directory.
		 * Existing files will be overwritten.
		 * If a config with the given name cannot be found, it will fails silently.
		 * If failed to serialize/write to a file, an error will be logged.
		 * @param name Config name
		 * @return true if successful
		 */
		bool Save(Name name) const;
		
	private:
		ConfigSystem();
		friend class EngineBase;
		HashMap<Name, Json> configs_;
	};
}
