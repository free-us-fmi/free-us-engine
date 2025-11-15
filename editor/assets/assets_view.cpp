#include "assets_view.h"
#include "imgui.h"
#include "ECS/entity_view.h"
#include "materials/material_view.h"
#include "ECS/components/selected_entity_view.h"
#include "selected_asset_view.h"
#include "content/model_view.h"
#include "shaders/shader_view.h"

namespace editor::assets 
{

namespace 
{
	selected_assets_tab _selected_tab = selected_assets_tab::models;
}

selected_assets_tab get_selected_asset_type()
{
	return _selected_tab;
}

void update()
{
	ImGui::Begin("Assets");
	
	ImGui::Text("Tabs:");
	ImGui::SameLine();
	if (ImGui::Button("Models", ImVec2(80, 0)))
		_selected_tab = selected_assets_tab::models;
	ImGui::SameLine();
	if (ImGui::Button("Materials", ImVec2(80, 0)))
		_selected_tab = selected_assets_tab::materials;
	ImGui::SameLine();
	if (ImGui::Button("Shaders", ImVec2(80, 0)))
		_selected_tab = selected_assets_tab::shaders;

	ImGui::Separator();


	ImGui::BeginChild("asset view");

	if ( _selected_tab == selected_assets_tab::models )
		model::update();
	else if ( _selected_tab == selected_assets_tab::materials )
		materials::update();
	else if ( _selected_tab == selected_assets_tab::shaders )
		shaders::update();

	ImGui::EndChild();

	ImGui::End();
}

}
