#include "renderer.h"
#include "core/GLCommon.h"
#include "data/Framebuffer.h"
#include "ECS/components/raymarched_geometry.h"
#include "managers/ProgramManager.h"
#include "shaders/program.h"
#include "managers/FramebufferManager.h"
#include "ECS/components/transform.h"
namespace raymarching {

    namespace {
        unsigned int vbo;
        unsigned int vao;
        unsigned int ebo;

        data::framebuffer::framebuffer_id framebuffer_id;
        std::string program_name;
    }
    float vertices[] = {
        1.f, 1.f, 0.0f, // top right
        1.f, -1.f, 0.0f, // bottom right
        -1.f, -1.f, 0.0f, // bottom left
        -1.f, 1.f, 0.0f // top left
    };

    unsigned int indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    void initialize() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    void render() {
        programs::program* program = programs::GetProgram(program_name);
        if ( !program or !program->linked() )
            program = programs::GetProgram("default_ray");

        assert( program->linked() );
        ecs::components::raymarched_geometry::update();
        glBindVertexArray(vao);
        program->Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void set_program(std::string name) {
        program_name = name;
    }

    std::string get_program() {
        return program_name;
    }
}
