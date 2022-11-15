#ifndef CUBICCHUNK_H
#define CUBICCHUNK_H

#include <Godot.hpp>
#include <StaticBody.hpp>

namespace godot {

	class CubicChunk : public StaticBody {
		GODOT_CLASS(CubicChunk, StaticBody)
	
	public:
		static void _register_methods();

		CubicChunk();
		~CubicChunk();

		void _init(); // our initializer called by Godot

    	void _process(float delta);

	};

}

#endif
