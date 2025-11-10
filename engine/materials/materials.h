#pragma once 
#include "utility/vector.h"
#include "core/common.h"
#include <filesystem>
#include <unordered_map>
#include <array>
#include "shaders/program.h"

namespace materials 
{

class material
{
public:
	enum texture_type {
		diffuse = 0,
		specular,

		count
	};
	static constexpr std::array<std::string, texture_type::count> type_name = {"diffuse", "specular"};

	std::string _name;
	std::string _uid;

	bool editor_visible = true;

	bool has_texture(texture_type type) const;
	void set_texture(texture_type type, const std::filesystem::path& path);
	void set_texture_from_data(const std::string& uid, texture_type type, unsigned char* data, unsigned int width, unsigned int height, unsigned int channels);
	void remove_texture(texture_type type);

	void bind_uniforms(texture_type type, programs::program* prog);
	void unbind_uniforms(texture_type type);

	textures::texture_id get_texture(texture_type type) const {
		if ( textures.contains(type) )
			return textures.at(type);
		return id::invalid_id;
	}
private:
	std::unordered_map<texture_type, textures::texture_id> textures;
};

}
