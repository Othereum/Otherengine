#include "InputTypes.h"
#include <boost/container_hash/hash.hpp>

using namespace boost;

size_t std::hash<Game::FKeyData>::operator()(const Game::FKeyData& key) const noexcept
{
	auto seed = hash_value(key.key);
	hash_combine(seed, key.mod);
	hash_combine(seed, key.type);
	return seed;
}

size_t std::hash<Game::FInputEvent>::operator()(const Game::FInputEvent& key) const noexcept
{
	auto seed = hash<Game::FKeyData>{}(key.key);
	hash_combine(seed, key.bPressed);
	return seed;
}
