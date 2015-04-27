#!/bin/bash

./bin/voxel -i input/torus.obj -x 512 -y 512 -z 512 -n 128 -p c -d 8 -t 10
./scripts/createVideo.sh
