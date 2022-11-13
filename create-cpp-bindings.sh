#!/bin/sh

mkdir src
mkdir demo

git init
git submodule add -b 3.x https://github.com/godotengine/godot-cpp
cd godot-cpp
git submodule update --init
cd ..
git submodule update --init --recursive
cd godot-cpp/
godot --gdnative-generate-json-api api.json
scons platform=linux generate_bindings=yes -j4
cd ..
mv godot-cpp/godot-headers/ .
rm godot-cpp/api.json

