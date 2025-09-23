#include "transform_view.h"
#include "editor/editor_common.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace editor::transform 
{

void update(ecs::components::transform::transform* transform)
{
	if ( transform == nullptr )
		return;

	ImGui::BeginGroup();
	ImGui::Text("transform");

	glm::vec3 position = transform->get_position();
	glm::vec3 rotation = transform->get_rotation();
	glm::vec3 scale = transform->get_scale();
	
	if ( ImGui::DragFloat3("position", glm::value_ptr(position), 0.1f, -1000000.f, 1000000.f))
		transform->set_position(position);
	if (ImGui::DragFloat3("rotation", glm::value_ptr(rotation), 1.f, -360.f, 360.f))
		transform->set_rotation(rotation);
	if (ImGui::DragFloat3("scale", glm::value_ptr(scale), 0.1f, -1000000.f, 1000000.f))
		transform->set_scale(scale);

	ImGui::EndGroup();
}

}
