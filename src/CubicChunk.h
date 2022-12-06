#ifndef CUBICCHUNK_H
#define CUBICCHUNK_H

#include <Godot.hpp>
#include <Node.hpp>
#include <StaticBody.hpp>
#include <Vector3.hpp>
#include <PoolArrays.hpp>
#include <SpatialMaterial.hpp>
#include <ResourceLoader.hpp>
#include <Ref.hpp>
#include <Texture.hpp>
#include <OpenSimplexNoise.hpp>
#include <ArrayMesh.hpp>
#include <Mesh.hpp>
#include <MeshInstance.hpp>
#include <MeshDataTool.hpp>
#include <SurfaceTool.hpp>
#include <iostream>
#include "./Global.h"

namespace godot {

	class CubicChunk : public StaticBody {
		GODOT_CLASS(CubicChunk, StaticBody)

	private:

		const Vector3 vertices [8] = {
        	Vector3(0, 0, 0), //0 
        	Vector3(1, 0, 0), //1
	    	Vector3(0, 1, 0), //2
    		Vector3(1, 1, 0), //3
    		Vector3(0, 0, 1), //4
	    	Vector3(1, 0, 1), //5
	    	Vector3(0, 1, 1), //6
    		Vector3(1, 1, 1)  //7
    	};

		const int TOP    [4] = {2, 3, 7, 6};
    	const int BOTTOM [4] = {0, 4, 5, 1};
    	const int LEFT   [4] = {6, 4, 0, 2};
    	const int RIGHT  [4] = {3, 1, 5, 7};
    	const int FRONT  [4] = {7, 5, 4, 6};
   		const int BACK   [4] = {2, 0, 1, 3};

		Global global_;
		Ref<OpenSimplexNoise> noise_;
		Ref<SpatialMaterial> material_;
		MeshInstance* mesh_instance_ = nullptr;
		Ref<SurfaceTool> st_;
		Vector3 chunk_position_;
		MeshDataTool* meshDataTool_;
		
		int create_block(int, int, int);
		void create_face(const int[4], int, int, int, Vector2);
		bool check_transparent(int, int, int);
		bool getSolidity(Global::block_type);
		godot::Global::block_type get_block_type(Vector3);

	public:
		
		std::string chunk_tag_;
		godot::Global::block_type blocks_[32][32][32];
		
		static void _register_methods();
		void generate();
		void update();

		CubicChunk();
		~CubicChunk();

		void _ready();

		void _init(); // our initializer called by Godot

    	void _process(float delta);

		void set_chunk_position(Vector3);

		Vector3 get_chunk_position();

		std::string make_chunk_tag(Vector3);

		//std::map<std::string, CubicChunk*>* chunk_map_;

	};

}

#endif
