#include "pointlight_view.h"
#include "imgui.h"
namespace editor::pointlight 
{

void update(ecs::components::point_light::point_light* point_light)
{
	if ( point_light == nullptr )
		return;

	ImGui::BeginGroup();
	ImGui::Text("point light");

	glm::vec3 ambient = point_light->get_ambient();
	glm::vec3 diffuse = point_light->get_diffuse();
	glm::vec3 specular = point_light->get_specular();
	
	if ( ImGui::DragFloat3("ambient", glm::value_ptr(ambient), 0.1f, 0.f, 1000.f))
		point_light->set_ambient(ambient);
	if (ImGui::DragFloat3("diffuse", glm::value_ptr(diffuse), 0.1f, 0.f, 1000.f))
		point_light->set_diffuse(diffuse);
	if (ImGui::DragFloat3("specular", glm::value_ptr(specular), 0.1f, 0.f, 1000.f))
		point_light->set_specular(specular);

	ImGui::EndGroup();

}

}
