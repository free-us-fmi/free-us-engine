#include "TextureManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"
#include <unordered_map>
#include <filesystem>
#include "utility/path.h"
#include "thread/main_thread_dispatcher.h"
#include <cstring>

#include "materials/textures.h"
#include "serializer/materials/materials.h"
#include "utility/uid.h"

namespace textures 
{

namespace  {
	utl::vector<texture_2d, false> textures;
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

bool exists(texture_id id)
{
	if ( id >= textures.size() )
		return false;
	return !textures.is_tombstone(textures.internal_begin() + id);
}

texture_id add_empty_texture() {
	texture_id id = textures.emplace_tombstone();
	return id;
}

texture_id add_texture(std::filesystem::path path)
{
	utl::normalize_path(path);
	
	texture_2d new_tex;

	texture_id id = add_empty_texture();
	textures[id].initialize(path);

	return id;
}

void remove_texture(texture_id id) {
	if ( !exists(id) )
		return;
	textures[id].free();
	textures.erase(textures.internal_begin() + id);
}

texture_2d* get_texture(texture_id id)
{
	if ( !exists(id) )
		return nullptr;

	return &textures[id];
}

unsigned int bind_texture(texture_id id)
{
	assert(exists(id));
	if ( !exists(id) )
		return ~(0);

	texture_2d* texture = get_texture(id);
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

void unbind_one(texture_id id)
{
	if ( !exists(id) )
		return;

	unsigned int slot_to_unbind = textures[id].get_slot();
	textures[id].unbind();
	free_slots.erase(free_slots.internal_begin() + slot_to_unbind);
}

void unbind_all()
{
	for ( auto& c : textures )
		c.unbind();
	free_slots.clear();
}

utl::vector<unsigned int> set_texture_list(utl::vector<texture_id> list)
{
	unbind_all();
	utl::vector<unsigned int> slots;
	assert(list.size() <= 32 );

	for ( auto id : list )
	{
		unsigned int slot = bind_texture(id);
		slots.emplace_back(slot);
	}

	return slots;
}

void texture_2d::initialize_from_data(const std::string& uid, unsigned char* data, int width, int height, int channels) {
	_uid = uid;

	auto gl_call = [&](){

		assert(data != nullptr);
		if ( data == nullptr ){
			return;
		}

		glGenTextures(1, &_internal_id);

		GLenum format, internal_format;
		glBindTexture(GL_TEXTURE_2D, _internal_id);
		if ( channels == 4 )
			format = GL_RGBA, internal_format = GL_SRGB_ALPHA;
		else if ( channels == 3)
			format = GL_RGB, internal_format = GL_SRGB;
		else format = internal_format = GL_ALPHA;

		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	};

	thread::main_thread::add_event_and_wait(gl_call);
}

void texture_2d::initialize(std::filesystem::path path)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, channels;
	utl::normalize_path(path);	
	std::string uid = utl::uid::get_prefix_uid("tex");

	unsigned char* data = stbi_load((path).string().c_str(), &width, &height, &channels, 0);
	serializer::textures::save_to_tmp(uid, data, width, height, channels);

	initialize_from_data(uid, data, width, height, channels);

	stbi_image_free(data);

}

void texture_2d::bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _internal_id);
	_slot = slot;
}

void texture_2d::unbind()
{
	_slot = ~(0);
}

void texture_2d::free() {
	if ( _internal_id == id::invalid_id )
		return;

	glDeleteTextures(1, &_internal_id);
	_internal_id = id::invalid_id;
}

}
