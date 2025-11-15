#pragma once
#include "core/common.h"

namespace ecs::components::raymarched_geometry {

    enum class model_types {
        ellipsoid,
        rectangular_prism,
        elliptic_cone,
        elliptic_capped_cylinder,
        pyramid,
        tetrahedron,

        count
    };

    class raymarched_geometry {
    public:
        explicit raymarched_geometry(entity::entity_id entity) : _entity_id{entity}{}
        entity::entity_id get_entity_id() const { return _entity_id; }
        void set_model_type(const model_types& model_type) { _model_type = model_type; }
        model_types get_model_type() const { return _model_type; }
    private:
        model_types _model_type = model_types::ellipsoid;
        entity::entity_id _entity_id;
    };

    raymarched_geometry_id create_raymarched_geometry(entity::entity_id entity);
    void remove_raymarched_geometry(raymarched_geometry_id id);
    raymarched_geometry* get_raymarched_geometry(raymarched_geometry_id id );
    bool is_valid(raymarched_geometry_id id);
    void update();

}
