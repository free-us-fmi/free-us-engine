#include "textures.h"
#include <fstream>

#include "file/temporary_files.h"
#include "core/serializer.h"
#include "engine/utility/uid.h"
#include "engine/managers/TextureManager.h"

namespace serializer::textures {
    void save_to_tmp(const std::string& uid, unsigned char* data, unsigned int width, unsigned int height, unsigned int channels) {
        std::filesystem::path tmp_path = file::temp::get_tmp_path_from_uid(uid);
        writer writer;
        writer.open(tmp_path);

        //uid
        writer.write(uid);

        //texture data details
        writer.write(width);
        writer.write(height);
        writer.write(channels);

        //data
        writer.write(data, width * height * channels);

        writer.flush();
        writer.close();
    }

    std::string retrieve_from_tmp(const std::string& uid) {
        std::filesystem::path tmp_path = file::temp::get_tmp_path_from_uid(uid);
        if ( tmp_path.empty() or !std::filesystem::exists(tmp_path) )
            return {};

        reader reader;
        reader.open_and_store(tmp_path);
        file::temp::delete_temporary_file(tmp_path);
        return reader.get_buffer();
    }

    ::textures::texture_id load_from_data(reader& reader) {

        std::string uid;
        reader.read(uid);

        unsigned int width, height, channels;
        reader.read(width); reader.read(height); reader.read(channels);

        utl::vector<unsigned char> data;
        data.resize(width * height * channels);
        reader.read(data.data());

        ::textures::texture_id id = ::textures::add_empty_texture();
        ::textures::texture_2d* tex = ::textures::get_texture(id);
        tex->initialize_from_data(uid, data.data(), width, height, channels);

        return id;
    }
}