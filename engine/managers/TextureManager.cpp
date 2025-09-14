#include "TextureManager.h"
#include <array>
#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"
#include <unordered_map>
#include "assets/AssetsPath.h"
#include <filesystem>
#include "utility/path.h"
namespace textures 
{

namespace  {
	std::unordered_map<std::string, texture_2d> textures;
	utl::vector<int, false> free_slots;
}
void add_texture(std::string path)
{
	if ( textures.find(path) != textures.end() )
		return;

	if ( path[0] != '*' )
		utl::normalize_path(path);
	
	texture_2d new_tex;

	textures[path] = new_tex;
	textures[path].initialize(path);
}

texture_2d* get_texture(std::string path)
{
	if ( path[0] != '*' )
		utl::normalize_path(path);
	assert(textures.find(path) != textures.end());
	return &textures[path];
}

unsigned int bind_texture(std::string path)
{
	if ( path[0] != '*')
		utl::normalize_path(path);
	
	auto iter = textures.find(path);
	if ( iter == textures.end() )
	{
		spdlog::error( "Error! Texture with path: {0} doesn't exists!", path);
		return ~(0);
	}


	texture_2d* texture = &iter->second;

	unsigned int slot = free_slots.emplace_tombstone();

	assert( slot < 32 );

	if ( slot >= 32 )
	{
		spdlog::error( " Error! Too many texture binded at once!");
		return ~(0);
	}
	texture->bind(slot);	
	
	return slot;
}

void unbind_all()
{
	for ( auto& c : textures )
		c.second.unbind();
	free_slots.clear();
}

utl::vector<unsigned int> set_texture_list(utl::vector<std::string> list)
{
	unbind_all();
	utl::vector<unsigned int> slots;
	assert(list.size() <= 32 );

	for ( auto& texture_path : list )
	{
		unsigned int slot = bind_texture(texture_path);	
		slots.emplace_back(slot);
	}

	return slots;
}

void texture_2d::initialize(std::string path)
{
	glGenTextures(1, &_id);

	if ( path[0] == '*' )
		return;

	stbi_set_flip_vertically_on_load(true);
	int width, height, channels;

	utl::normalize_path(path);	

	_path = path;


	unsigned char* data = stbi_load((path).c_str(), &width, &height, &channels, 0);
	
	assert(data != nullptr);
	if ( data == nullptr ){
		spdlog::error( "Error! Failed to create texture with path: {0}", path);
		glDeleteTextures(1, &_id);
		return;
	}

	GLenum format, internal_format;
	glBindTexture(GL_TEXTURE_2D, _id);
	if ( channels == 4 )
		format = internal_format = GL_RGBA;
	else if ( channels == 3) 
		format = internal_format = GL_RGB;
	else format = internal_format = GL_ALPHA;

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

void texture_2d::bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _id);
	_slot = slot;
}

void texture_2d::unbind()
{
	_slot = ~(0);
}

}
