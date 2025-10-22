#include "scene_view.h"
#include "managers/FramebufferManager.h"
#include "data/Framebuffer.h"
#include "utility/util.h"
#include "editor_common.h"

namespace editor::scene 
{

namespace {
	data::framebuffer::framebuffer_id _ms_framebuffer_id;		
	data::framebuffer::framebuffer_id _framebuffer_id;		
	int width, height;
}

void initialize(const editor_init_data& data)
{
	_ms_framebuffer_id = data::framebuffer::AddFramebuffer(data._frame_buffer_width, data._frame_buffer_height);
	_framebuffer_id = data::framebuffer::AddFramebuffer(data._frame_buffer_width, data._frame_buffer_height);

	data::framebuffer::framebuffer* _ms_framebuffer = data::framebuffer::GetFramebuffer(_ms_framebuffer_id);
	data::framebuffer::framebuffer* _framebuffer = data::framebuffer::GetFramebuffer(_framebuffer_id);
	height = data._frame_buffer_height;
	width = data._frame_buffer_width;

	_framebuffer->add_texture_2d("color_texture", GL_RGB, GL_COLOR_ATTACHMENT0);

	_ms_framebuffer->add_texture_2d("color_texture", GL_RGB8, GL_COLOR_ATTACHMENT0, true);
	_ms_framebuffer->add_renderbuffer("depth_stencil_buffer", GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);
}

void bind_framebuffer()
{
	data::framebuffer::BindFramebuffer(_ms_framebuffer_id);
}

unsigned int get_framebuffer()
{
	return _ms_framebuffer_id;	
}

void clear()
{
	data::framebuffer::framebuffer* _ms_framebuffer = data::framebuffer::GetFramebuffer(_ms_framebuffer_id);
	data::framebuffer::framebuffer* _framebuffer = data::framebuffer::GetFramebuffer(_framebuffer_id);
	_framebuffer->clear();
	_ms_framebuffer->clear();
}
void update()
{
	ImGui::Begin("world view", NULL, ImGuiWindowFlags_NoScrollbar);

		data::framebuffer::framebuffer* _ms_framebuffer = data::framebuffer::GetFramebuffer(_ms_framebuffer_id);
		data::framebuffer::framebuffer* _framebuffer = data::framebuffer::GetFramebuffer(_framebuffer_id);
		_ms_framebuffer->bind();


		ImVec2 size = ImGui::GetWindowSize();
		ImVec2 off = ImVec2((size.x - (float)width) * 0.5, 
		      			(size.y - (float)height) * 0.5);	

		ImGui::SetCursorPos(off);
		ImGui::SetNextWindowSize(ImVec2(width, height));
		ImGui::BeginChild("scene view");

			width = size.x;
			height = size.y;
	
			utl::keep_aspect_ratio(width, height, _ms_framebuffer->get_aspect_ratio());

			ImGui::SetWindowSize(ImVec2(width, height));
			
			int off_x = (ImGui::GetWindowWidth() - _framebuffer->get_width()) * 0.5;
			int off_y = (ImGui::GetWindowHeight() - _framebuffer->get_height()) * 0.5;
			_ms_framebuffer->resolve(_framebuffer->get_id());
			ImGui::Image((ImTextureID)_framebuffer->get_texture_2d("color_texture"), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
		_ms_framebuffer->bind();
		ImGui::EndChild();

	ImGui::End();
}

}
