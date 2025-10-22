#pragma once
#include "core/GLCommon.h"
#include "utility/vector.h"
#include <string>
namespace textures 
{

class texture_2d
{
public:
	void initialize(std::string path);
	void bind(unsigned int slot);
	void unbind();
	
	unsigned int get_id() const { return _id; }
	unsigned int get_slot() const { return _slot; }
private:
	bool binded = false;
	unsigned int _slot;

	unsigned int _id;
	std::string _path = "";
};


void add_texture(std::string path);
unsigned int bind_texture(std::string path);
utl::vector<unsigned int> set_texture_list(utl::vector<std::string> list);
void unbind_all();
void unbind_one(const std::string& name);

unsigned int get_manual_slot();
void unbind_manual_slot(unsigned int slot);

bool exists(const std::string& name);

texture_2d* get_texture(std::string path);

}
