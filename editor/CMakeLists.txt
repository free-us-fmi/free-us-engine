project(editor)

add_executable(editor main.cpp 
		editor.cpp
		scene_view.cpp

		ECS/entity_view.cpp
		ECS/components/selected_entity_view.cpp
		ECS/components/transform_view.cpp
		ECS/components/geometry_view.cpp
		ECS/components/lights/pointlight_view.cpp

		utility/EditorPath.cpp
		utility/helpers/draw_browser.cpp
		utility/helpers/progress_bar/progess_popup.cpp
		utility/helpers/progress_bar/progress_bar.cpp
		utility/helpers/string_browser.cpp

		files/file_popup.cpp
		files/file_browser.cpp

		content/model_pupup.cpp
		content/model_browser.cpp
		content/model_view.cpp
		content/selected_model_view.cpp

		materials/selected_material_view.cpp
		materials/material_view.cpp
		materials/new_material_popup.cpp
		materials/material_browser.cpp
		materials/material_popup.cpp

		assets/assets_view.cpp
		assets/selected_asset_view.cpp
)

target_include_directories(editor PUBLIC ../glfw/include/)
target_include_directories(editor PUBLIC ../glad/include/)
target_include_directories(editor PUBLIC ../assimp/include/ )
target_include_directories(editor PUBLIC ../editor/ )
target_include_directories(editor PUBLIC ../imgui/ )
target_include_directories(editor PUBLIC ../ )

target_link_libraries(editor engine)

