#ifndef CUBICCHUNK_H
#define CUBICCHUNK_H

#include <Godot.hpp>
#include <StaticBody.hpp>
#include <Vector3.hpp>

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

	
	public:
		static void _register_methods();

		CubicChunk();
		~CubicChunk();

		void _init(); // our initializer called by Godot

    	void _process(float delta);

	};

}

#endif
