#ifndef GLOBAL_H
#define GLOBAL_H

#include <Godot.hpp>
#include <Node.hpp>
#include <Vector2.hpp>
#include <Vector3.hpp>
#include <vector>
#include <map>

namespace godot {

	class Global : public Node {
		GODOT_CLASS(Global, Node)

	//private:

	public:
		const Vector3 DIMENSION = Vector3(32, 32, 32);
		const int WORLD_HEIGHT = 64;
		const Vector2 TEXTURE_ATLAS_SIZE = Vector2(3,2);
    };

}

#endif