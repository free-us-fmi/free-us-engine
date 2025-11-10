project(serializer)

add_library(serializer STATIC
        core/serializer.cpp

        content/scene.cpp
        content/model.cpp
        content/mesh.cpp

        project/active_project.cpp
        project/scene.cpp

        materials/textures.cpp
        materials/materials.cpp

        file/temporary_files.cpp

        ecs/ecs.cpp
        ecs/components/geometry.cpp
        ecs/components/instanced_geometry.cpp
        ecs/components/point_light.cpp
        ecs/components/transform.cpp
)

target_include_directories(engine PUBLIC ../ )
target_include_directories(engine PUBLIC ../serializer )

target_link_libraries(serializer engine)