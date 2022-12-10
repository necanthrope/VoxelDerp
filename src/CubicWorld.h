#ifndef CUBIC_WORLD_H
#define CUBIC_WORLD_H

#include <Godot.hpp>
#include <Variant.hpp>
#include <Ref.hpp>
#include <Spatial.hpp>
#include <Array.hpp>
#include <Node.hpp>
#include <KinematicBody.hpp>
#include <SceneTree.hpp>
#include <Vector3.hpp>
#include <Math.hpp>
#include <Thread.hpp>
#include <Engine.hpp>
#include <Input.hpp>
#include "Global.h"
#include "CubicChunk.h"
#include "Blocks.h"
#include <limits.h>
#include <iostream>

namespace godot {

	class CubicWorld : public Spatial {
		GODOT_CLASS(CubicWorld, Spatial)

    private:
        Global global_;
        //Ref<Input> input_;
        Vector3 load_radius_ = Vector3(7, 7, 7);
        Thread* load_thread_stray_;
        Engine* engine_;
        Blocks* block_library_;

        Vector3 _get_player_chunk_coords(Vector3);
    
        int positive_mod(int, int);

        void _update_chunk(CubicChunk*);

        CubicChunk* get_chunk(Vector3);    


    public:

        Spatial* world_; 
        KinematicBody* player_;
        Spatial* chunks_;
        std::map<std::string, CubicChunk*> chunk_map_;

        static void _register_methods();

        CubicWorld();
        ~CubicWorld();

        void _ready();

        void _init(); // our initializer called by Godot

        void _process(float delta);
        
        void _on_Player_place_block(Vector3, Blocks::block_type);

        void _on_Player_break_block(Vector3);
        
        void _thread_process();

    };
}
#endif