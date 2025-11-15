project(engine)

add_library(engine STATIC ../glad/src/glad.c
		shaders/src/ShadersPath.cpp
		application.cpp
		shaders/shader.cpp
		shaders/program.cpp
		shaders/program_uniforms.cpp

		managers/ProgramManager.cpp
		managers/TextureManager.cpp
		managers/FramebufferManager.cpp
		managers/MaterialManager.cpp
		managers/ShaderManager.cpp

		assets/AssetsPath.cpp
		core/camera.cpp

		ECS/components/lights/point_light.cpp
		ECS/components/transform.cpp
		ECS/components/geometry.cpp
		ECS/components/instanced_geometry.cpp
		ECS/components/raymarched_geometry.cpp
		ECS/ecs.cpp
		ECS/entity.cpp

		utility/path.cpp
		content/content_loader.cpp
		content/mesh.cpp 
		content/model.cpp 
		content/scene.cpp 
		content/primitives/primitive_loader.cpp

		materials/materials.cpp

		data/Framebuffer.cpp

		thread/main_thread_dispatcher.cpp

		raymarcher/renderer.cpp
)


target_include_directories(engine PUBLIC ../glfw/include/)
target_include_directories(engine PUBLIC ../glad/include/)
target_include_directories(engine PUBLIC ../assimp/include/ )
target_include_directories(engine PUBLIC ../engine/ )
target_include_directories(engine PUBLIC ../imgui/ )
target_include_directories(engine PUBLIC ../ )

target_link_libraries(engine glfw)
target_link_libraries(engine assimp)
target_link_libraries(engine imgui)
target_link_libraries(engine serializer)

find_package(OpenGL REQUIRED)

target_link_libraries(engine OpenGL::GL)

