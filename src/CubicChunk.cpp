#include "CubicChunk.h"
#include "CubicWorld.h"
#include <chrono>

using namespace godot;

void CubicChunk::_register_methods() {
    register_method("_process", &CubicChunk::_process);
    register_method("_ready", &CubicChunk::_ready);
    register_method("generate", &CubicChunk::generate);
    register_method("update", &CubicChunk::update);
    register_method("set_chunk_position", &CubicChunk::set_chunk_position);
    register_method("get_chunk_position", &CubicChunk::get_chunk_position);
    register_method("_init", &CubicChunk::_init);
    register_property("chunk_position_", &CubicChunk::chunk_position_, Vector3(),
        GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RESOURCE_TYPE, String("Vector3 coords"));
    // register_property("chunk_tag_", &CubicChunk::chunk_tag_, std::string(),
    //     GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RESOURCE_TYPE, String("string coords"));
}

CubicChunk::CubicChunk() {
}

CubicChunk::~CubicChunk() {
    //cleanup code here
}

void CubicChunk::_init() {
    //initialize variables here

    ResourceLoader* loader = ResourceLoader::get_singleton();
	material_ = loader->load("res://assets/new_spatialmaterial.tres");
    meshDataTool_ = MeshDataTool::_new();
    noise_ = Ref<OpenSimplexNoise>(OpenSimplexNoise::_new());
    st_ = Ref<SurfaceTool>(SurfaceTool::_new());

}

godot::Global::block_type CubicChunk::get_block_type(Vector3 global_pos) {
    
    float elev = noise_->get_noise_3dv(global_pos);
    int height = ceil(int((elev + 1)/2 * global_.WORLD_HEIGHT));
                
    int block_elevation = global_pos.y;
    godot::Global::block_type block = global_.AIR;
    if (block_elevation < height / 2) {
        block = global_.STONE;
    } else if (block_elevation < height) {
        block = global_.DIRT;
    } else if (block_elevation == height) {
        block = global_.GRASS;
    }

    return block;
}

void CubicChunk::generate() {
    int dim_x = global_.DIMENSION.x;
    int dim_y = global_.DIMENSION.y;
    int dim_z = global_.DIMENSION.z;
    
    for (int i = 0; i < dim_x; i++) {
        for (int j = 0; j < dim_y; j++) {
            for (int k = 0; k < dim_z; k++) {
                Vector3 global_pos = chunk_position_ * global_.DIMENSION + Vector3(i, j, k);
                godot::Global::block_type block = get_block_type(global_pos);
                blocks_[i][j][k] = block;
            }
        }
    }

}

void CubicChunk::create_face(const int i[4], int x, int y, int z, Vector2 texture_atlas_offset) {
    Vector3 offset = Vector3(x, y, z);
    
	Vector3 a = godot::CubicChunk::vertices[i[0]] + offset;
	Vector3 b = godot::CubicChunk::vertices[i[1]] + offset;
	Vector3 c = godot::CubicChunk::vertices[i[2]] + offset;
	Vector3 d = godot::CubicChunk::vertices[i[3]] + offset;
	
	Vector2 uv_offset = texture_atlas_offset / global_.TEXTURE_ATLAS_SIZE;
	float height = 1.0 / global_.TEXTURE_ATLAS_SIZE.y;
	float width = 1.0 / global_.TEXTURE_ATLAS_SIZE.x;
	
	Vector2 uv_a = uv_offset + Vector2(0, 0);
	Vector2 uv_b = uv_offset + Vector2(0, height);
	Vector2 uv_c = uv_offset + Vector2(width, height);
	Vector2 uv_d = uv_offset + Vector2(width, 0);

    
    PoolVector3Array vertices1;
    vertices1.append(a);
    vertices1.append(b);
    vertices1.append(c);
    PoolVector2Array uvs1;
    uvs1.append(uv_a);
    uvs1.append(uv_b);
    uvs1.append(uv_c);

    PoolVector3Array vertices2;
    vertices2.append(a);
    vertices2.append(c);
    vertices2.append(d);
    PoolVector2Array uvs2;
    uvs2.append(uv_a);
    uvs2.append(uv_c);
    uvs2.append(uv_d);

	st_->add_triangle_fan(vertices1, uvs1);
	st_->add_triangle_fan(vertices2, uvs2);
}

bool CubicChunk::getSolidity(godot::Global::block_type block) {
    std::pair<godot::Global::side_map, bool> block_pair = global_.block_types[block];
    godot::Global::side_map block_info = block_pair.first;
    const bool solid = block_pair.second; 
    return solid;
}

bool CubicChunk::check_transparent(int x, int y, int z) {
    Vector3 global_pos = chunk_position_ * global_.DIMENSION + Vector3(x, y, z);
    godot::Global::block_type block = get_block_type(global_pos);
    return !getSolidity(block);
}


int CubicChunk::create_block(int x, int y, int z) {

    godot::Global::block_type block = blocks_[x][y][z];
	if (block == godot::Global::AIR) {
		return 0;
    }

	std::pair<godot::Global::side_map, bool> block_pair = global_.block_types[block];
    godot::Global::side_map block_info = block_pair.first;
	
	if (check_transparent(x, y + 1, z)) {
		create_face(TOP, x, y, z, block_info[global_.TOP]);
    }
	
	if (check_transparent(x, y - 1, z)) {
		create_face(BOTTOM, x, y, z, block_info[global_.BOTTOM]);
    }
	
	if (check_transparent(x - 1, y, z)) {
		create_face(LEFT, x, y, z, block_info[global_.LEFT]);
    }
		
	if (check_transparent(x + 1, y, z)) {
		create_face(RIGHT, x, y, z, block_info[global_.RIGHT]);
    }
		
	if (check_transparent(x, y, z - 1)) {
		create_face(BACK, x, y, z, block_info[global_.BACK]);
    }
		
	if (check_transparent(x, y, z + 1)) {
		create_face(FRONT, x, y, z, block_info[global_.FRONT]);
    }
    return 1;
}

void CubicChunk::update() {

    //Unload
	if (mesh_instance_) {
        mesh_instance_->call_deferred("queue_free", godot::Array());
        //remove_child(mesh_instance_);
        //mesh_instance_->queue_free();
	}

    godot::Ref<godot::ArrayMesh> mesh = Ref<ArrayMesh>(ArrayMesh::_new());
    mesh_instance_ = MeshInstance::_new();


    st_->begin(Mesh::PRIMITIVE_TRIANGLES);

    int dim_x = global_.DIMENSION.x;
    int dim_y = global_.DIMENSION.y;
    int dim_z = global_.DIMENSION.z;

    int block_count = 0;
    for (int x = 0; x < dim_x; x++) {
        for (int y = 0; y < dim_y; y++) {
            for (int z = 0; z < dim_z; z++) {
                block_count += create_block(x, y, z);
            }
        }
    }

    if (block_count > 0) {
        st_->generate_normals(false);
	    st_->set_material(material_);
	    st_->commit(mesh);
	    mesh_instance_->set_mesh(mesh);
    
	    add_child(mesh_instance_);
        mesh_instance_->create_trimesh_collision();
        set_visible(true);
    }
    
}

void CubicChunk::set_chunk_position(Vector3 pos) {
    chunk_position_ = pos;
	Vector3 translation = Vector3(pos.x, pos.y, pos.z) * global_.DIMENSION;
    set_translation(translation);
	set_visible(false);
	chunk_tag_ = make_chunk_tag(chunk_position_);
}

Vector3 CubicChunk::get_chunk_position() {
    return chunk_position_;
}

void CubicChunk::_ready() {
    material_->get_texture(SpatialMaterial::TEXTURE_ALBEDO)->set_flags(Texture::FLAG_REPEAT);
    generate();
    update();
}

void CubicChunk::_process(float delta) {

}

std::string CubicChunk::make_chunk_tag(Vector3 coords) {
    return (
        std::to_string(static_cast<int>(coords.x)) + ":" + 
		std::to_string(static_cast<int>(coords.y)) + ":" + 
		std::to_string(static_cast<int>(coords.z))
    );
}
