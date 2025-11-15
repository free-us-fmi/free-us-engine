#pragma once
#include <cstdint>

#include <cassert>

#include "primitive_types.h"
#include "id.h"


namespace ecs::entity{
	DEFINE_TYPED_ID(entity);
}

namespace ecs::components::transform{
	DEFINE_TYPED_ID(transform);
}

namespace ecs::components::point_light {
	DEFINE_TYPED_ID(point_light);
}

namespace ecs::components::geometry 
{
	DEFINE_TYPED_ID(geometry);
}

namespace ecs::components::instanced_geometry 
{
	DEFINE_TYPED_ID(instanced_geometry);
}

namespace data::framebuffer 
{
	DEFINE_TYPED_ID(framebuffer);
}

namespace programs
{
	DEFINE_TYPED_ID(program);
}

namespace materials 
{
	DEFINE_TYPED_ID(material);
}

namespace textures {
	DEFINE_TYPED_ID(texture);
}

namespace programs {
	DEFINE_TYPED_ID(program);
}

namespace shaders {
	DEFINE_TYPED_ID(shader);
}
