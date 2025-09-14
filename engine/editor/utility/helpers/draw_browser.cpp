#include "draw_browser.h"

namespace editor::helpers::draw_browser 
{
std::string draw_browser(utl::vector<browser_element>& elements)
{
	std::string selected_element = "";

	ImGui::BeginGroup();
	static float border_x = 20.f;
	static float item_separation_size = 20.f;
	static float icon_size = 64.f;

	float next_offsetx = border_x;
	float next_offsety = 0.f;

	float oldCursorX = ImGui::GetCursorPosX(); 
	float oldCursorY = ImGui::GetCursorPosY();

	for ( auto& entry : elements )
	{	
		float text_size = ImGui::CalcTextSize(entry.name.c_str()).x;
		float offset_size = std::max(text_size, icon_size + item_separation_size);
		if ( next_offsetx + offset_size + border_x > ImGui::GetWindowSize().x )
			next_offsetx = border_x, next_offsety += 78.f + item_separation_size;

	
		float posx = oldCursorX + next_offsetx;
		float posy = oldCursorY + next_offsety;

		ImGui::SetCursorPos(ImVec2(posx, posy + icon_size));
		ImGui::Text("%s",  entry.name.c_str());

		ImGui::SetCursorPos(ImVec2(posx, posy));
		ImGui::Selectable(("##" + entry.name ).c_str(), false, ImGuiSelectableFlags_Disabled, ImVec2(78, 78));

		if ( ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) )
		{

			if ( ImGui::IsMouseDoubleClicked(0))
				selected_element = entry.name;
			ImGui::SetTooltip("%s" , entry.name.c_str());
		}

		textures::texture_2d* tex = textures::get_texture(entry.icon_texture);
		ImGui::SetCursorPos(ImVec2(posx, posy));
		ImGui::Image((ImTextureRef)tex->get_id(), ImVec2(icon_size, icon_size), ImVec2(0, 1), ImVec2(1, 0));

		next_offsetx += (offset_size + item_separation_size);
	}
	ImGui::EndGroup();
	return selected_element;
}
}
