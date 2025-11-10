#pragma once
#include <filesystem>

#include "core/GLCommon.h"
#include "utility/vector.h"
#include <string>

namespace textures 
{

class texture_2d
{
public:
	void initialize_from_data(const std::string& uid, unsigned char* data, int width, int height, int channels);
	void initialize(std::filesystem::path path);
	void bind(unsigned int slot);
	void unbind();

	unsigned int get_internal_id() const { return _internal_id; }
	unsigned int get_slot() const { return _slot; }

	void free();

	std::string get_uid() const { return _uid;}
private:

	bool binded = false;
	unsigned int _slot;

	unsigned int _internal_id;
	std::string _uid;
};

texture_id add_empty_texture();
texture_id add_texture(std::filesystem::path path);
void remove_texture(texture_id id);

unsigned int bind_texture(texture_id id);
utl::vector<unsigned int> set_texture_list(utl::vector<texture_id> list);
void unbind_all();
void unbind_one(texture_id id);

unsigned int get_manual_slot();
void unbind_manual_slot(unsigned int slot);

bool exists(texture_id id);

texture_2d* get_texture(texture_id id);

}
