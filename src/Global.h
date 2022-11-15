#ifndef GLOBAL_H
#define GLOBAL_H

#include <Godot.hpp>
#include <Node.hpp>
#include <Vector2.hpp>
#include <Vector3.hpp>
#include <map>

namespace godot {

	class Global : public Node {
		GODOT_CLASS(Global, Node)

	//private:

	public:
		const Vector3 DIMENSION = Vector3(16, 16, 16);
		const int WORLD_HEIGHT = 64;
		const Vector2 TEXTURE_ATLAS_SIZE = Vector2(3,2);

		enum block_side {
			TOP,
			BOTTOM,
			LEFT,
			RIGHT,
			FRONT,
			BACK,
			SOLID
		};

		enum block_type {
			AIR,
			DIRT,
			GRASS,
			STONE
		};

		void _init(); // our initializer called by Godot

		std::map<
			godot::Global::block_side, 
			Vector2, 
			std::less<
				godot::Global::block_side
			>, 
			std::allocator<
				std::pair<
					godot::Global::block_side, 
					Vector2
				>
			>
		> side_map;

		std::map<
			godot::Global::block_type, 
			std::map<
				godot::Global::block_side, 
				Vector2,
				std::less<
					godot::Global::block_side
				>, 
				std::allocator<
					std::pair<
						godot::Global::block_side, 
						Vector2
					>
				>
			>,
			std::less<
				godot::Global::block_type
			>, 
			std::allocator<
				std::pair<
					godot::Global::block_type,
					std::map<
						godot::Global::block_side, 
						Vector2,
						std::less<
							godot::Global::block_side
						>, 
						std::allocator<
							std::pair<
								godot::Global::block_side, 
								Vector2
							>
						>
					>
				>
			>
		> block_types = {
			{AIR, {}},
			{DIRT, 
				{
					{TOP, Vector2(2, 0)}, {BOTTOM, Vector2(2, 0)}, {LEFT, Vector2(2, 0)},
					{RIGHT, Vector2(2,0)}, {FRONT, Vector2(2, 0)}, {BACK, Vector2(2, 0)}
				}
			},
			
			{GRASS, 
				{
					{TOP, Vector2(0, 0)}, {BOTTOM, Vector2(2, 0)}, {LEFT, Vector2(1, 0)},
					{RIGHT, Vector2(1,0)}, {FRONT, Vector2(1, 0)}, {BACK, Vector2(1, 0)}
				}
			},
			{STONE,
				{
					{TOP, Vector2(0, 1)}, {BOTTOM, Vector2(0, 1)}, {LEFT, Vector2(0, 1)},
					{RIGHT, Vector2(0, 1)}, {FRONT, Vector2(0, 1)}, {BACK, Vector2(0, 1)}
				}
			}
			
		};

    };

}

#endif