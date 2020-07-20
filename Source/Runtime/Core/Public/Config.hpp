#pragma once
#include "Json.hpp"
#include "Name.hpp"
#include "Templates/HashMap.hpp"

namespace oeng::core
{
	class CORE_API ConfigManager
	{
	public:
		ConfigManager();
		
		/**
		 * Find config or create if not found.
		 * @param name Config name
		 * @return Reference to non-const json config object
		 */
		[[nodiscard]] Json& Config(Name name);
		
		/**
		 * Save config as file into the user data directory.
		 * Existing files will be overwritten.
		 * @param name Config name
		 * @return true if successful
		 */
		bool SaveConfig(Name name);
		
	private:
		HashMap<Name, Json> configs_;
	};
}
