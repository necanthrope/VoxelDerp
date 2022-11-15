#include "CubicChunk.h"

using namespace godot;

void CubicChunk::_register_methods() {
    register_method("_process", &CubicChunk::_process);
}

CubicChunk::CubicChunk() {
}

CubicChunk::~CubicChunk() {
    //cleanup code here
}

void CubicChunk::_init() {
    //initialize variables here
}

void CubicChunk::_process(float delta) {
    
}