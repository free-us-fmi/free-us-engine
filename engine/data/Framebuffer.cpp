#include "Framebuffer.h"
#include "managers/TextureManager.h"

namespace data::framebuffer
{

framebuffer::framebuffer(int width, int height) : _width(width), _height(height)	
{
	glGenFramebuffers(1, &_id);
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	glViewport(0, 0, width, height);
}
	

void framebuffer::add_renderbuffer(const std::string& name, GLenum format, GLenum attachment)
{
	if ( _render_buffers.find(name) != _render_buffers.end() )
	{
		return;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, _id);	

	_render_buffers[name]._format = format;
	_render_buffers[name]._attachment = attachment;

	glGenRenderbuffers(1, &_render_buffers[name]._id);

	glBindRenderbuffer(GL_RENDERBUFFER, _render_buffers[name]._id);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 1, GL_DEPTH24_STENCIL8, _width, _height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);	

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, _render_buffers[name]._id);
}

void framebuffer::bind() const { 
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	glViewport(0, 0, _width, _height);
}
void framebuffer::update_renderbuffers()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _id);	

	for ( auto buffer : _render_buffers )
	{
		glBindRenderbuffer(GL_RENDERBUFFER, buffer.second._id);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 1, GL_DEPTH24_STENCIL8, _width, _height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);	

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, buffer.second._attachment, GL_RENDERBUFFER, buffer.second._id);
	}
}

void framebuffer::add_texture_2d(const std::string& name, GLenum format, GLenum attachment, bool ms, GLenum type )
{
	if ( _textures_2d.find(name) != _textures_2d.end() )
	{
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _id);	

	_textures_2d[name]._format = format;
	_textures_2d[name]._attachment = attachment;
	_textures_2d[name]._multi_sampling = ms;


	GLenum tex_target = ms? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

	glGenTextures(1, &_textures_2d[name]._id);
	glBindTexture(tex_target, _textures_2d[name]._id);

	if ( ms )
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 1, format, _width, _height, GL_TRUE);
	else glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, type, NULL);

	glTexParameteri(tex_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri(tex_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, tex_target, _textures_2d[name]._id, 0);
}

void framebuffer::update_textures()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _id);	

	for ( auto& texture : _textures_2d )
	{
		bool ms = texture.second._multi_sampling;
		GLenum tex_target = ms? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture.second._id);
		if ( ms )
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 1, texture.second._format, _width, _height, GL_TRUE);
		else glTexImage2D(GL_TEXTURE_2D, 0, texture.second._format, _width, _height, 0, texture.second._format, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(tex_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri(tex_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


		glFramebufferTexture2D(GL_FRAMEBUFFER, texture.second._attachment, tex_target, texture.second._id, 0);
	}

}

unsigned int framebuffer::get_texture_2d(const std::string& name)
{
	if ( _textures_2d.find(name) == _textures_2d.end() )
	{
		return 0;
	}

	return _textures_2d[name]._id;
}

unsigned int framebuffer::get_renderbuffer(const std::string& name) 
{
	if ( _render_buffers.find(name) == _render_buffers.end() )
	{
		return 0;
	}

	return _render_buffers[name]._id;
}


unsigned int framebuffer::bind_texture(const std::string& name)
{
	if ( _textures_2d.find(name) == _textures_2d.end() )
	{
		return 0;
	}
	
	unsigned int slot = textures::get_manual_slot();
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _textures_2d[name]._id);
	return slot;
}

void framebuffer::set_size(int width, int height)
{
	if ( width == _width or height == _height)
		return; 

	_width = width;
	_height = height;

	update_renderbuffers();
	update_textures();
	clear();
	glViewport(0, 0, width, height);
}

void framebuffer::clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _id);	
	glClearColor(_clear_color.r, _clear_color.g, _clear_color.b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void framebuffer::resolve(unsigned int target)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _id);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void framebuffer::destroy()
{
	for ( auto& texture : _textures_2d )
		glDeleteTextures(1, &texture.second._id );

	for ( auto& renderbuffer : _render_buffers )
		glDeleteRenderbuffers(1, &renderbuffer.second._id );

	glDeleteFramebuffers(1, &_id);
}


}
