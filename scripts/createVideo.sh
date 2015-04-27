#!/bin/bash

for img in *.tga; do
    filename=${img%.*}
    convert "$filename.tga" "$filename.png"
done

ffmpeg -framerate 15 -pattern_type glob -i '*.png' -vf fps=25  -c:v libx264 -pix_fmt yuv420p out.mp4

#rm *.tga
rm *.png
