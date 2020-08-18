#include "Serialization/ArchiveFileReader.hpp"

namespace oeng::core
{
Archive& ArchiveFileReader::operator<<(Json& json)
{
    json = Json::parse(stream_, nullptr, true, true);
    return *this;
}
}
