#include "Blocks.h"

using namespace godot;

Blocks::Blocks() {
    create_block_prototypes();
}

Blocks::~Blocks() {}

Blocks::block_data::block_data(block_type kind) {
    this->kind = kind;
}

void Blocks::create_block_prototypes() {
    block_data* air_block = new block_data(AIR);
    air_block->solid = false;
    block_types.push_back(air_block);

    block_data* dirt_block = new block_data(DIRT);
    dirt_block->sides[TOP] = Vector2(2, 0);
    dirt_block->sides[BOTTOM] = Vector2(2, 0);
    dirt_block->sides[LEFT] = Vector2(2, 0);
    dirt_block->sides[RIGHT] = Vector2(2,0);
    dirt_block->sides[FRONT] = Vector2(2, 0);
    dirt_block->sides[BACK] = Vector2(2, 0);
    dirt_block->solid = true;
    block_types.push_back(dirt_block);

    block_data* grass_block = new block_data(GRASS);
    grass_block->sides[TOP] = Vector2(0, 0);
    grass_block->sides[BOTTOM] = Vector2(2, 0);
    grass_block->sides[LEFT] = Vector2(1, 0);
    grass_block->sides[RIGHT] = Vector2(1,0);
    grass_block->sides[FRONT] = Vector2(1, 0);
    grass_block->sides[BACK] = Vector2(1, 0);
    grass_block->solid = true;
    block_types.push_back(grass_block);

    block_data* stone_block = new block_data(STONE);
    stone_block->sides[TOP] = Vector2(0, 1);
    stone_block->sides[BOTTOM] = Vector2(0, 1);
    stone_block->sides[LEFT] = Vector2(0, 1);
    stone_block->sides[RIGHT] = Vector2(0, 1);
    stone_block->sides[FRONT] = Vector2(0, 1);
    stone_block->sides[BACK] = Vector2(0, 1);
    stone_block->solid = true;
    block_types.push_back(stone_block);

}


