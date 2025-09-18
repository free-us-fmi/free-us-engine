#include "scene_view.h"
#include "managers/FramebufferManager.h"
#include "data/Framebuffer.h"
#include "utility/util.h"
#include "editor_common.h"

namespace editor::scene 
{

namespace {
	data::framebuffer::framebuffer_id _framebuffer_id;		
	int width, height;
}

void initialize(const editor_init_data& data)
{
	_framebuffer_id = data::framebuffer::AddFramebuffer(data._frame_buffer_width, data._frame_buffer_height);
	data::framebuffer::framebuffer* _framebuffer = data::framebuffer::GetFramebuffer(_framebuffer_id);

	_framebuffer->add_texture_2d("color_texture", GL_RGB, GL_COLOR_ATTACHMENT0);
	_framebuffer->add_renderbuffer("depth_stencil_buffer", GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);
}

void bind_framebuffer()
{
	data::framebuffer::BindFramebuffer(_framebuffer_id);
}

unsigned int get_framebuffer()
{
	return data::framebuffer::GetFramebuffer(_framebuffer_id)->get_id();	
}

void clear()
{
	data::framebuffer::framebuffer* _framebuffer = data::framebuffer::GetFramebuffer(_framebuffer_id);
	_framebuffer->clear();
}
void update()
{
	data::framebuffer::framebuffer* _framebuffer = data::framebuffer::GetFramebuffer(_framebuffer_id);
	_framebuffer->bind();

	ImGui::Begin("scene view", NULL, ImGuiWindowFlags_NoTitleBar);
	ImVec2 size = ImGui::GetWindowSize();

	width = size.x;
	height = size.y;
		
	utl::keep_aspect_ratio(width, height, _framebuffer->get_aspect_ratio());

	ImGui::SetWindowSize(ImVec2(width, height));
	
	ImVec2 scale = ImGui::GetIO().DisplayFramebufferScale;

	int off_x = (ImGui::GetWindowWidth() - _framebuffer->get_width()) * 0.5;
	int off_y = (ImGui::GetWindowHeight() - _framebuffer->get_height()) * 0.5;

	ImGui::Image((ImTextureID)_framebuffer->get_texture_2d("color_texture"), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}

}
