#pragma once
#include "engine/content/model.h"
#include "core/serializer.h"
#include <string>

namespace serializer::content::model {
    std::string get_detail(unsigned int model_id);
    std::string get_base(unsigned int model_id);

    void load_detail(reader& detail_reader, ::content::model::model& model);
    void load_base(reader& base_reader, ::content::model::model& model);
}