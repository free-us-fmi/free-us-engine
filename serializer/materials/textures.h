#pragma once
#include <string>
#include <filesystem>
#include "engine/core/common.h"
#include "core/serializer.h"

namespace serializer::textures {
    void save_to_tmp(const std::string& uid, unsigned char* data, unsigned int width, unsigned int height, unsigned int channels);
    std::string retrieve_from_tmp(const std::string& uid);
    ::textures::texture_id load_from_data(reader& reader);
}