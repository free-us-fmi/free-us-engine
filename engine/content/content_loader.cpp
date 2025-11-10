#include "content_loader.h"
#include "core/GLCommon.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "scene.h"
#include "managers/TextureManager.h"
#include "utility/path.h"
#include "utility/assimp_glm.h"
#include <filesystem>
#include <fstream>
#include <format>
#include "materials/materials.h"
#include "managers/MaterialManager.h"
#include "utility/uid.h"

namespace content
{

void create_scene(aiNode* node, utl::vector<unsigned int>& mesh_ids, scene::scene& scene_data)
{
	if (  node -> mNumMeshes )
	{
		model::model _model;
		for ( unsigned int i = 0; i < node->mNumMeshes; ++i )
		{
			unsigned int mesh_id = mesh_ids[node->mMeshes[i]];
			_model._meshes.emplace_back(mesh_id);	
		}
		_model._local_model = AssimpGLMHelpers::ConvertMatrixToGLMFormat(node->mTransformation);
		unsigned int id = model::add_model(_model);
		scene_data._models.emplace_back(id);
	}

	for ( unsigned int i = 0; i < node->mNumChildren; ++i )
		create_scene(node->mChildren[i], mesh_ids, scene_data);
}

std::filesystem::path create_and_reister_texture_type(aiMaterial* material, aiTextureType type, const std::string& dir_path,const aiScene* scene)
{

	if ( !material->GetTextureCount(type))
		return std::filesystem::path();

	aiString str;
	material->GetTexture(type, 0, &str);

	std::string path_str(str.C_Str());

	if ( path_str[0] == '*' )
	{
		unsigned int id = std::stoi(path_str.substr(1));
		const aiTexture* texture = scene->mTextures[id];

		std::string extension = texture->achFormatHint;

		path_str = std::format("texture_{}.{}", id, extension);
		path_str = dir_path + path_str;

		std::ofstream File(path_str, std::ios::binary);

		if ( texture->mHeight == 0 )
		{
			File.write(reinterpret_cast<const char*>(texture->pcData), texture->mWidth);
			File.close();
		} else{
			std::cout << "GLB textures must be compressed!" << std::endl;
		}
	} else {
		path_str = dir_path + path_str;
		std::filesystem::path path(path_str);
		utl::normalize_path(path);
		path_str = path.string();
	}

	return path_str;
}

unsigned int create_and_register_material(aiMaterial* material,const std::string& path,const aiScene* scene, const std::string& name)
{
	unsigned int mat_id = materials::AddMaterial(name);
	materials::material* material_data = materials::GetMaterial(name);

	materials::change_texture(materials::material::texture_type::diffuse, mat_id, create_and_reister_texture_type(material, aiTextureType_HEIGHT, path, scene));
	materials::change_texture(materials::material::texture_type::diffuse, mat_id, create_and_reister_texture_type(material, aiTextureType_DIFFUSE, path, scene));
	materials::change_texture(materials::material::texture_type::specular, mat_id, create_and_reister_texture_type(material, aiTextureType_SPECULAR, path, scene));
	
	material_data->editor_visible = false;
	return mat_id;
}

unsigned int create_and_register_mesh(const std::string& name, unsigned int mesh_id, aiMesh* mesh,utl::vector<unsigned int>& materials)
{
	mesh::mesh mesh_data;
	for ( unsigned int i = 0; i < mesh->mNumVertices; ++i )
	{
		vertex vertex_data;
		vertex_data.position.x = mesh->mVertices[i].x;
		vertex_data.position.y = mesh->mVertices[i].y;
		vertex_data.position.z = mesh->mVertices[i].z;
		
		vertex_data.normal.x = mesh->mNormals[i].x;
		vertex_data.normal.y = mesh->mNormals[i].y;
		vertex_data.normal.z = mesh->mNormals[i].z;

		vertex_data.uv = {0.f, 0.f};
		if ( mesh->mTextureCoords[0] ){
			vertex_data.uv.x = mesh->mTextureCoords[0][i].x;
			vertex_data.uv.y = mesh->mTextureCoords[0][i].y;
		}
		mesh_data._vertices.emplace_back(vertex_data);
	}

	for ( unsigned int i = 0; i < mesh->mNumFaces; ++i )
	{
		aiFace face = mesh->mFaces[i];
		for ( unsigned int j = 0; j < face.mNumIndices; ++j )
			mesh_data._indices.emplace_back(face.mIndices[j]);
	}

	assert(mesh->mMaterialIndex < materials.size());
	
	mesh_data.set_material(materials[mesh->mMaterialIndex]);

	unsigned int id = mesh::add_mesh(mesh_data);
	return id;
}

scene::scene create_scene_from_file(std::string path, bool uv_flipped)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.c_str(),aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_ValidateDataStructure | (aiProcess_FlipUVs * uv_flipped));
	
	utl::vector<unsigned int> materials;
	std::filesystem::path fp(path);
	fp.remove_filename();

	for ( unsigned int i = 0; i < scene->mNumMaterials; ++i) {
		materials.emplace_back(create_and_register_material(scene->mMaterials[i], fp.string(), scene, path + "_" + std::to_string(i)));
	}
	utl::vector<unsigned int> mesh_ids;
	
	for ( unsigned int i = 0; i < scene->mNumMeshes; ++i )
		mesh_ids.emplace_back(create_and_register_mesh(path, i, scene->mMeshes[i], materials));

	scene::scene scene_data;
	create_scene(scene->mRootNode, mesh_ids, scene_data);
	return scene_data;
}	

}
