#ifndef BLOCKS_H
#define BLOCKS_H

#include <Vector2.hpp>
#include "Global.h"

namespace godot {
    class Blocks : public Node {
        GODOT_CLASS(Blocks, Node
        )

    private:
        void create_block_prototypes();

    public:
        enum block_side {
            TOP,
            BOTTOM,
            LEFT,
            RIGHT,
            FRONT,
            BACK
        };

        enum block_type {
            AIR,
            DIRT,
            GRASS,
            STONE,
            LENGTH
        };

        struct block_data {
            block_data(block_type kind);
            block_type kind;
            Vector2 sides[6];
            bool solid;
        };

        std::vector <block_data*> block_types;

        Blocks();
        ~Blocks();

    };
}
#endif //BLOCKS_H
