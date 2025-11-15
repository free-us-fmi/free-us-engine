#pragma once
#include <filesystem>
#include <string>
#include "core/GLCommon.h"
#include "core/common.h"

namespace shaders {

	enum class ShaderType
	{
		VERTEX = 0,
		FRAGMENT,
		GEOMETRY
	};

	class shader
	{
	public:
		shader(const std::filesystem::path &path, ShaderType shader_type, bool editor_visible = false);
		void compile();

		std::filesystem::path GetPath() const { return _path; }
		unsigned int GetId() const { return _id; }
		bool IsActive() const { return _active; }

		void Destroy();

		ShaderType _shader_type;

		bool _editor_visible = false;
	private:
		unsigned int _id = id::invalid_id;
		std::filesystem::path _path;

		bool _active = true;
	};
}