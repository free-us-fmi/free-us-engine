#include "TextureManager.h"
#include <array>
#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"
#include <unordered_map>
#include "assets/AssetsPath.h"
#include <filesystem>
#include "utility/path.h"
#include "thread/main_thread_dispatcher.h"


namespace textures 
{

namespace  {
	std::unordered_map<std::string, texture_2d> textures;
	utl::vector<int, false> free_slots;

	unsigned int get_free_slot()
	{
		unsigned int slot = free_slots.emplace_tombstone();

		assert( slot < 32 );

		if ( slot >= 32 )
		{
			return ~(0);
		}

		return slot;
	}
}

bool exists(const std::string& name)
{
	return textures.find(name) != textures.end();
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
		return ~(0);
	}


	texture_2d* texture = &iter->second;
	
	unsigned int slot = get_free_slot();

	texture->bind(slot);	
	
	return slot;
}

unsigned int get_manual_slot()
{
	unsigned int slot = get_free_slot();
	return slot;
}

void unbind_manual_slot(unsigned int slot)
{
	if ( slot >= free_slots.size() or free_slots.is_tombstone(free_slots.internal_begin() + slot) )
		return;
	free_slots.erase(free_slots.internal_begin() + slot);
}

void unbind_one(const std::string& name)
{
	if ( textures.find(name) != textures.end() )
	{
		unsigned int slot_to_unbind = textures[name].get_slot();
		textures[name].unbind();
		free_slots.erase(free_slots.internal_begin() + slot_to_unbind);
	}
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
	if ( path[0] == '*' )
		return;
	stbi_set_flip_vertically_on_load(true);
	int width, height, channels;
	utl::normalize_path(path);	
	_path = path;

	unsigned char* data = stbi_load((path).c_str(), &width, &height, &channels, 0);

	auto gl_call = [&](){

		assert(data != nullptr);
		if ( data == nullptr ){
			return;
		}

		glGenTextures(1, &_id);

		GLenum format, internal_format;
		glBindTexture(GL_TEXTURE_2D, _id);
		if ( channels == 4 )
			format = GL_RGBA, internal_format = GL_SRGB_ALPHA;
		else if ( channels == 3) 
			format = GL_RGB, internal_format = GL_SRGB;
		else format = internal_format = GL_ALPHA;

		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	};
	
	thread::main_thread::add_event_and_wait(gl_call);

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
