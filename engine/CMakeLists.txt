project(engine)

add_executable(engine main.cpp ../glad/src/glad.c
		shaders/src/ShadersPath.cpp
		application.cpp
		shaders/shader.cpp
		shaders/program.cpp

		managers/ProgramManager.cpp
		managers/TextureManager.cpp
		managers/FramebufferManager.cpp

		assets/AssetsPath.cpp
		core/camera.cpp

		ECS/components/lights/point_light.cpp
		ECS/components/transform.cpp
		ECS/components/geometry.cpp
		ECS/ecs.cpp
		ECS/entity.cpp

		utility/path.cpp
		content/content_loader.cpp
		content/mesh.cpp 
		content/model.cpp 
		content/scene.cpp 

		editor/editor.cpp
		editor/scene_view.cpp
		editor/ECS/entity_view.cpp
		editor/ECS/components/selected_entity_view.cpp
		editor/ECS/components/transform_view.cpp
		editor/utility/EditorPath.cpp
		editor/utility/helpers/draw_browser.cpp
		editor/files/file_popup.cpp
		editor/files/file_browser.cpp
		editor/content/model_pupup.cpp
		editor/content/model_browser.cpp
		editor/content/model_view.cpp

		data/Framebuffer.cpp

)


target_include_directories(engine PUBLIC ../glfw/include/)
target_include_directories(engine PUBLIC ../glad/include/)
target_include_directories(engine PUBLIC ../assimp/include/ )
target_include_directories(engine PUBLIC ../spdlog/include/ )
target_include_directories(engine PUBLIC ../engine/ )
target_include_directories(engine PUBLIC ../imgui/ )
target_include_directories(engine PUBLIC ../ )

target_link_libraries(engine glfw)
target_link_libraries(engine assimp)
target_link_libraries(engine imgui)

find_package(OpenGL REQUIRED)

target_link_libraries(engine OpenGL::GL)

