#include "CubicWorld.h"

using namespace godot;

void CubicWorld::_register_methods() {
    register_method("_process", &CubicWorld::_process);
    register_method("_ready", &CubicWorld::_ready);
    register_method("_thread_process", &CubicWorld::_thread_process);
}

CubicWorld::CubicWorld() {}

CubicWorld::~CubicWorld() {}

void CubicWorld::_init() {

    load_thread_stray_ = Thread::_new();
    engine_ = Engine::get_singleton();
    block_library_ = new Blocks();
    //input_ = Ref<Input>(Input::_new());

}

// https://stackoverflow.com/questions/1082917/mod-of-negative-number-is-melting-my-brain
real_t PosMod(real_t a, real_t b) {
    real_t c = fmod(a,b);
    if ((c < 0 && b > 0) || (c > 0 && b < 0)) 
    {
        c += b;
    }
    return c;
}


/*
 * Chunk generation functions
 */

Vector3 CubicWorld::_get_player_chunk_coords(Vector3 chunk_position) {
    float cx = chunk_position.x;
	float cy = chunk_position.y;
	float cz = chunk_position.z;
		
	float px = floor(player_->get_translation().x / global_.DIMENSION.x);
	float py = floor(player_->get_translation().y / global_.DIMENSION.y);
	float pz = floor(player_->get_translation().z / global_.DIMENSION.z);
	
	// offset by chunk position player is in, this will be 0 at game start
	float new_x = PosMod(cx - px + load_radius_.x/2, load_radius_.x) + px - load_radius_.x/2;
	float new_y = PosMod(cy - py + load_radius_.y/2, load_radius_.y) + py - load_radius_.y/2;
	float new_z = PosMod(cz - pz + load_radius_.z/2, load_radius_.z) + pz - load_radius_.z/2;
	
	Vector3 chunk_coords = Vector3(new_x, new_y, new_z);
	return chunk_coords;
}

void CubicWorld::_update_chunk(CubicChunk* c) {
    float cx = c->get_chunk_position().x;
	float cy = c->get_chunk_position().y;
	float cz = c->get_chunk_position().z;
	
	Vector3 player_chunk_coords = _get_player_chunk_coords(c->get_chunk_position());
	float new_x = player_chunk_coords.x;
	float new_y = player_chunk_coords.y;
	float new_z = player_chunk_coords.z;
	
	if (new_x != cx or new_y != cy or new_z != cz) {
		c->set_chunk_position(Vector3(int(new_x), int(new_y), int(new_z)));
        c->block_library_ = block_library_;
		c->generate();
		c->update();
    }
}

void CubicWorld::_thread_process() {
	while(true) {
        int child_count = chunks_->get_child_count();

        //int child_count = chunks_->get_children().size();

		for (int c = 0; c < child_count; c++) {
			godot::CubicChunk* chunk = Object::cast_to<CubicChunk>(chunks_->get_children()[c]);
            _update_chunk(chunk);
        }
    }
}


/*
 * Block placement functions
 */

// Fix this with a hash look up by chunk tag
CubicChunk* CubicWorld::get_chunk(Vector3 chunk_position) {
    std::string __chunk_tag = (std::to_string(static_cast<int>(chunk_position.x)) + ":" + 
		std::to_string(static_cast<int>(chunk_position.y)) + ":" + 
		std::to_string(static_cast<int>(chunk_position.z)));
    int child_count = chunks_->get_child_count();
		for (int c = 0; c < child_count; c++) {
            godot::CubicChunk* chunk = Object::cast_to<CubicChunk>(chunks_->get_children()[c]);
		    if (chunk->get_chunk_position() == chunk_position) {
			    return chunk;
            }
        }
	return nullptr;
}

void CubicWorld::_on_Player_place_block(Vector3 pos, Blocks::block_type t) {
	float cx = int(floor(pos.x / global_.DIMENSION.x));
	float cy = int(floor(pos.y / global_.DIMENSION.y));
	float cz = int(floor(pos.z / global_.DIMENSION.z));
	
	int bx = PosMod(floor(pos.x), global_.DIMENSION.x);
	int by = PosMod(floor(pos.y), global_.DIMENSION.y);
    int bz = PosMod(floor(pos.z), global_.DIMENSION.z);
	
	CubicChunk* c = get_chunk(Vector3(cx, cy, cz));
	if (c != nullptr) {
		c->blocks_[bx][by][bz] = t;
		c->update();
    }
}

void CubicWorld::_on_Player_break_block(Vector3 pos) {
	_on_Player_place_block(pos, block_library_->AIR);
}

void CubicWorld::_ready() {

    if (!engine_->is_editor_hint()) {
    //     input_->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
    }

    player_ = Object::cast_to<KinematicBody>(get_node("Player"));
    chunks_ = Object::cast_to<Spatial>(get_node("Chunks"));

    for (int i = 0; i < load_radius_.x; i++) {
        for (int j = 0; j < load_radius_.y; j++) {
            for (int k = 0; k < load_radius_.z; k++) {

                godot::CubicChunk* chunk = CubicChunk::_new();
                Vector3 player_chunk_coords = _get_player_chunk_coords(Vector3(i, j, k));

                chunk->set_chunk_position(player_chunk_coords);
                chunk->block_library_ = block_library_;
                //chunk->chunk_map_ = &chunk_map_;
                chunks_->add_child(chunk);

                chunk_map_.insert(std::make_pair(chunk->chunk_tag_, chunk));

            }
        }
    }

    std::string kids = std::to_string(chunks_->get_child_count());
    std::cout << "chunk kids: " << kids << std::endl;

    load_thread_stray_->start(this, "_thread_process");

    player_->connect("place_block", this, "_on_Player_place_block");
    player_->connect("break_block", this, "_on_Player_break_block");

    if (!engine_->is_editor_hint()) {
    //    input_->set_mouse_mode(Input::MOUSE_MODE_VISIBLE);
    }

}

void CubicWorld::_process(float delta) {

}



