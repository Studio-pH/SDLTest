#!/bin/bash

cd extern/SDL

cmake .

make install

cd ../SDL_image

cmake .

make install

cd ../box2d 

./build.sh

